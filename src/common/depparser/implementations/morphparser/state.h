// Copyright (C) University of Oxford 2010
#ifndef _ENGLISH_DEPENDENCY_PARSER_STATEITEM
#define _ENGLISH_DEPENDENCY_PARSER_STATEITEM

/*===============================================================
 *
 * CStateItem - the search state item, representing a partial
 *              candidate with shift reduce. 
 *
 * Note about members: there are two types of item properties;
 * The first is the stack, which changes during the process; 
 * The second is the incoming sentence, including m_lHeads, 
 * m_lDepsL and m_lDepsR, m_lDepNumL, m_lDepNumR etc, which 
 * records the properties of each input word so far.
 *
 * A state item is partial and do not include information about
 * all words from the input sentence. Though m_lHeads, m_lDepsL 
 * and m_lDepsR are fixed arrays with MAX_SENTENCE_SIZE length, 
 * not all the elements from the arrays are used. The ACTIVE 
 * elements are from the input index 0 to m_nNextWord, inclusive. 
 * And a state item only captures information about the active 
 * sub section from input.
 *
 * The property for each input word need to be initialised. 
 * m_lHeads m_lDepsL etc could be initialised within the 
 * clear() method. However, because the parsing process is
 * incremental, they can also be initialised lasily. 
 * Apart from the avoidance of unecessary assignments, one 
 * benefit of lazy initialisation is that we only need to copy
 * the active indexies when doing a copy operation. Therefore, 
 * this implementation takes the approach. 
 * The initialisation of these values are in clear(), shift()
 * and arcright()
 *
 *==============================================================*/

#include "morph/morph.h"
#include "morph/aux_lexicon.h"
#include "morph/penn_lexicon.h"

class CStateItem {

public:
   enum STACK_STATUS { OFF_STACK=0, ON_STACK_SHIFT, ON_STACK_ARCRIGHT } ;

protected:
   std::vector<int> m_Stack;                     // stack of words that are currently processed
   std::vector<int> m_HeadStack;
   int m_nNextWord;                         // index for the next word
   int m_lHeads[MAX_SENTENCE_SIZE];         // the lexical head for each word
   int m_lDepsL[MAX_SENTENCE_SIZE];         // the leftmost dependency for each word (just for cache, temporary info)
   int m_lDepsR[MAX_SENTENCE_SIZE];         // the rightmost dependency for each word (just for cache, temporary info)
   int m_lDepNumL[MAX_SENTENCE_SIZE];       // the number of left dependencies
   int m_lDepNumR[MAX_SENTENCE_SIZE];       // the number of right dependencies
   CSetOfTags<CDependencyLabel> m_lDepTagL[MAX_SENTENCE_SIZE]; // the set of left tags
   CSetOfTags<CDependencyLabel> m_lDepTagR[MAX_SENTENCE_SIZE]; // the set of right tags
   int m_lSibling[MAX_SENTENCE_SIZE];       // the sibling towards head
#ifdef LABELED
   unsigned long m_lLabels[MAX_SENTENCE_SIZE];   // the label of each dependency link
#endif
   unsigned long m_nLastAction;                  // the last stack action

   //const std::vector < CTaggedWord<CTag, TAG_SEPARATOR> >* m_lCache;
   const std::vector < CWord >* m_lCache;

   CMorph m_lMorph[MAX_SENTENCE_SIZE]; //the morphological information for each word
   CLemma m_lLemma[MAX_SENTENCE_SIZE]; //the lemma for each word

   int m_nNextUncachedWord; //the first word that is not in the cache (hasn't been morphologically analyzed yet). The difference m_nNextUncachedWord - m_nNextWord must be <= MORPH_CACHE_LIMIT.

public:
   SCORE_TYPE score;                        // score of stack - predicting how potentially this is the correct one

public:
   // constructors and destructor
   CStateItem(const std::vector < CWord >*cache=0) : m_lCache(cache) { clear(); }
   ~CStateItem() { }
   CStateItem(CStateItem& item) : m_lCache(0) { std::cerr<<"CStateItem does not support copy constructor!"; std::cerr.flush(); assert(1==0); }

public:
   std::string debugstring() const
   {
	   std::stringstream ss;
	   ss << "[[[stacksize " << m_Stack.size();
	   ss << "nextword " << m_nNextWord;
	   ss << "nextuncached " << m_nNextUncachedWord;
	   for ( int i=0; i<m_nNextUncachedWord; ++i )
	   {
		   ss << (*m_lCache)[i].str() << " ";
		   ss << CMorph(m_lMorph[i]).str() << " || ";
	   }
	   ss << "]]]";
	   return ss.str();
   }

public:
   // comparison
   inline bool operator < (const CStateItem &item) const { return score < item.score; }
   inline bool operator > (const CStateItem &item) const { return score > item.score; }
   inline bool operator == (const CStateItem &item) const {
      int i;
      if ( m_nNextWord != item.m_nNextWord )
         return false;
      if ( m_nNextUncachedWord != item.m_nNextUncachedWord )
         return false;
      for ( i=0; i<m_nNextWord; ++i ) {
         if ( m_lHeads[i] != item.m_lHeads[i] )
            return false;
      }
#ifdef LABELED
      for ( i=0; i<m_nNextWord; ++i ) 
         if ( m_lLabels[i] != item.m_lLabels[i] )
            return false;
#endif
      for ( i=0; i<m_nNextUncachedWord; ++i ) {
         if ( m_lMorph[i] != item.m_lMorph[i] )
         {
        	//std::cout << "False by morph of word " << i << ": " << CMorph(m_lMorph[i]).str() << " vs. " << CMorph(item.m_lMorph[i]).str() << "\n";
        	//if ( i > 0 ) std::cout << "Previous were: " << CMorph(m_lMorph[i-1]).str() << " vs. " << CMorph(item.m_lMorph[i-1]).str() << "\n";
            return false;
         }
      }
      for ( i=0; i<m_nNextUncachedWord; ++i ) {
         if ( m_lLemma[i] != item.m_lLemma[i] )
            return false;
      }

      if ( m_Stack.size() != item.m_Stack.size() )
         return false;
      if ( m_Stack.size()>0 && m_Stack.back()!=item.m_Stack.back() )
         return false;
      // I think that the stacks don't have to be compared
      // might be proved by translating tree to stack
      assert( m_Stack == item.m_Stack );
      assert( m_HeadStack == item.m_HeadStack );
      return true;
   }
   inline bool operator != (const CStateItem &item) const {
      return ! ((*this)==item);
   }

   // propty
   inline int stacksize() const { return m_Stack.size(); }
   inline bool stackempty() const { return m_Stack.empty(); }
   inline int stacktop() const { assert(!m_Stack.empty()); return m_Stack.back(); }
   inline int stacksecond() const { assert(m_Stack.size() > 1); return m_Stack.rbegin()[1]; }
   inline int stackbottom() const { assert(!m_Stack.empty()); return m_Stack.front(); }
   inline int stackitem( const unsigned &index ) const { assert(index<m_Stack.size()); return m_Stack[index]; }

   inline int cachesize() const { return m_nNextUncachedWord - m_nNextWord; }
   inline int morphanalyzed() const { return m_nNextUncachedWord; }

   inline bool headstackempty() const { return m_HeadStack.empty(); }
   inline int headstacktop() const { assert(!m_HeadStack.empty()); return m_HeadStack.back(); }
   inline int headstackitem( const unsigned &index ) const { assert(index<m_HeadStack.size()); return m_HeadStack[index]; }
   inline int headstacksize() const { return m_HeadStack.size(); }

   inline bool afterreduce() const { 
#ifdef LABELED
      return action::getUnlabeledAction(m_nLastAction)==action::REDUCE;
#else
      return m_nLastAction==action::REDUCE; 
#endif
}

   inline int head( const int &index ) const { assert(index<=m_nNextWord); return m_lHeads[index]; }
   inline int leftdep( const int &index ) const { assert(index<=m_nNextWord); return m_lDepsL[index]; }
   inline int rightdep( const int &index ) const { assert(index<=m_nNextWord); return m_lDepsR[index]; }
   inline int sibling( const int &index ) const { assert(index<=m_nNextWord); return m_lSibling[index]; }
   inline int size( ) const { return m_nNextWord ; }
#ifdef LABELED
   inline int label( const int &index ) const { assert(index<=m_nNextWord); return m_lLabels[index]; }
#endif

   inline int leftarity( const int &index ) const { assert(index<=m_nNextWord); return m_lDepNumL[index]; }
   inline int rightarity( const int &index ) const { assert(index<=m_nNextWord); return m_lDepNumR[index]; }

   inline const CSetOfTags<CDependencyLabel> &lefttagset( const int &index ) const { assert(index<=m_nNextWord); return m_lDepTagL[index]; }
   inline const CSetOfTags<CDependencyLabel> &righttagset( const int &index ) const { assert(index<=m_nNextWord); return m_lDepTagR[index]; }

   inline CMorph morph ( const int & index ) const { assert(index<=m_nNextUncachedWord); return m_lMorph[index]; }
   inline CLemma lemma ( const int & index ) const { assert(index<=m_nNextUncachedWord); return m_lLemma[index]; }
   inline CWord lemmaascword ( const int & index ) const { assert(index<=m_nNextUncachedWord); return CWord(m_lLemma[index].str()); }

   void clear() { 
      m_nNextWord = 0; m_nNextUncachedWord = 0; m_Stack.clear(); m_HeadStack.clear();
      score = 0; 
      m_nLastAction = action::NO_ACTION;
      ClearNext();
   }

   void operator = ( const CStateItem &item ) {
      m_Stack = item.m_Stack;
      m_HeadStack = item.m_HeadStack;
      m_nNextWord = item.m_nNextWord;
      m_nNextUncachedWord = item.m_nNextUncachedWord;
      m_nLastAction = item.m_nLastAction;
      m_lCache = item.m_lCache;
      score = item.score; 
      for ( int i=0; i<=m_nNextWord; ++i ){ // only copy active word (including m_nNext)
         m_lHeads[i] = item.m_lHeads[i];  
         m_lDepsL[i] = item.m_lDepsL[i]; 
         m_lDepsR[i] = item.m_lDepsR[i];
         m_lDepNumL[i] = item.m_lDepNumL[i];
         m_lDepNumR[i] = item.m_lDepNumR[i];
         m_lDepTagL[i] = item.m_lDepTagL[i];
         m_lDepTagR[i] = item.m_lDepTagR[i];
         m_lSibling[i] = item.m_lSibling[i];
#ifdef LABELED
         m_lLabels[i] = item.m_lLabels[i];
#endif
      }
      for ( int i = 0 ; i <= m_nNextUncachedWord ; ++i )
      {
          m_lMorph[i] = item.m_lMorph[i];
          m_lLemma[i] = item.m_lLemma[i];
      }
   }

//-----------------------------------------------------------------------------

public:
   // the arc left action links the current stack top to the next word with popping
#ifdef LABELED
   void ArcLeft(unsigned long lab) {
#else
   void ArcLeft() { 
#endif
      assert( m_Stack.size() > 0 ) ;
      assert( m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD ) ;
      static int left ;
      left = m_Stack.back() ;
      m_Stack.pop_back() ;
      m_HeadStack.pop_back() ;
      m_lHeads[left] = m_nNextWord;
#ifdef LABELED
      m_lLabels[left] = lab;
      m_lDepTagL[m_nNextWord].add(lab) ;
#endif
      m_lSibling[left] = m_lDepsL[m_nNextWord];
      m_lDepsL[m_nNextWord] = left ;
      m_lDepNumL[m_nNextWord] ++ ;
#ifdef LABELED
      m_nLastAction=action::encodeAction(action::ARC_LEFT, lab);
#else
      m_nLastAction=action::encodeAction(action::ARC_LEFT);
#endif
   }

   // the arc right action links the next word to the current stack top with pushing
#ifdef LABELED
   void ArcRight(unsigned long lab) {
#else
   void ArcRight() { 
#endif
      assert( m_Stack.size() > 0 ) ;
      static int left ;
      left = m_Stack.back() ;
      m_Stack.push_back( m_nNextWord ) ;
      m_lHeads[m_nNextWord] = left ;
#ifdef LABELED
      m_lLabels[m_nNextWord] = lab ;
      m_lDepTagR[left].add(lab) ;
#endif
      m_lSibling[m_nNextWord] = m_lDepsR[left];
      m_lDepsR[left] = m_nNextWord ;
      m_lDepNumR[left] ++ ;
      m_nNextWord ++;
      ClearNext();
#ifdef LABELED
      m_nLastAction=action::encodeAction(action::ARC_RIGHT, lab);
#else
      m_nLastAction=action::encodeAction(action::ARC_RIGHT);
#endif
   }

   // the shift action does pushing
   void Shift() {
      assert( cachesize() > 0 ); //must have at least a word in the cache to shift it into the stack
	  m_Stack.push_back( m_nNextWord );
      m_HeadStack.push_back( m_nNextWord );
      m_nNextWord ++ ;
      ClearNext();
      m_nLastAction=action::encodeAction(action::SHIFT);
   }

   // the shift cache action does morphological analysis
   void ShiftCache( unsigned long morphInfo ) {
      assert( cachesize() < MORPH_CACHE_LIMIT );
	  m_lMorph[m_nNextUncachedWord] = morphInfo;
	  //std::cout << "Added morph: " << CMorph(morphInfo).str() << "\n";
      m_lLemma[m_nNextUncachedWord] = getLemma( (*m_lCache)[m_nNextUncachedWord].str() , CMorph(m_lMorph[m_nNextUncachedWord]) );
      m_nNextUncachedWord ++ ;
      ClearNext();
      m_nLastAction=action::encodeAction(action::SHIFT_CACHE,morphInfo);
   }
 
   // the reduce action does popping
   void Reduce() {
      assert( m_lHeads[m_Stack.back()] != DEPENDENCY_LINK_NO_HEAD ) ;
      m_Stack.pop_back() ;
      m_nLastAction=action::encodeAction(action::REDUCE);
   }

   // this is used for the convenience of scoring and updating
   void PopRoot() {
#ifndef FRAGMENTED_TREE
      assert( m_Stack.size() == 1 && m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD ) ; // make sure only one root item in stack 
#else
      assert( m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD ) ;
#endif
#ifdef LABELED
      m_lLabels[m_Stack.back()] = CDependencyLabel::ROOT;
#endif
      m_nLastAction = action::encodeAction(action::POP_ROOT);
      m_Stack.pop_back() ; // pop it
   }

   // the clear next action is used to clear the next word, used with forwarding the next word index
   void ClearNext() {
      m_lHeads[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD ;
      m_lDepsL[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD ;
      m_lDepsR[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD ;
      m_lDepNumL[m_nNextWord] = 0 ;
      m_lDepTagL[m_nNextWord].clear() ;
      m_lDepNumR[m_nNextWord] = 0 ;
      m_lDepTagR[m_nNextWord].clear() ;
      m_lSibling[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD ;
      m_lLemma[m_nNextUncachedWord] = CLemma(); //empty lemma
      m_lMorph[m_nNextUncachedWord] = CMorph().code(); //empty morph
#ifdef LABELED
      m_lLabels[m_nNextWord] = CDependencyLabel::NONE;
#endif
   }

   // the move action is a simple call to do action according to the action code
   void Move ( const unsigned long &ac ) {
#ifdef LABELED
      switch (action::getUnlabeledAction(ac)) {
#else
      switch (ac) {
#endif
      case action::NO_ACTION:
         return;
      case action::SHIFT:
    	 //std::cout << "Shifting. " << debugstring() << "\n";
         Shift();
         //std::cout << "Result: " << debugstring() << "\n";
         return;
      case action::SHIFT_CACHE:
    	  //std::cout << "Shifting cache. " << debugstring() << "\n";
    	  //std::cout << "With: " << CMorph(action::getLabelOrMorph(ac)).str() << "\n";
    	  ShiftCache( action::getLabelOrMorph(ac) );
    	  //std::cout << "Result: " << debugstring() << "\n";
    	  return;
      case action::REDUCE:
    	  //std::cout << "Reducing. " << size() << " " << stacksize() << " " << m_nNextWord << " " << m_nNextUncachedWord << "\n";
         Reduce();
         return;
      case action::ARC_LEFT:
    	  //std::cout << "Arc left. " << size() << " " << stacksize() << " " << m_nNextWord << " " << m_nNextUncachedWord << "\n";
#ifdef LABELED
         ArcLeft(action::getLabelOrMorph(ac));
#else
         ArcLeft();
#endif
         return;
      case action::ARC_RIGHT:
    	  //std::cout << "Arc right. " << size() << " " << stacksize() << " " << m_nNextWord << " " << m_nNextUncachedWord << "\n";
#ifdef LABELED
         ArcRight(action::getLabelOrMorph(ac));
#else
         ArcRight();
#endif
         return;
      case action::POP_ROOT:
         PopRoot();
         return;
      default:
         THROW("unknown action: " << ac << '.');
      }
   }

//-----------------------------------------------------------------------------

public:

   // returns true is the next word advances -- by shift or arcright. 
#ifdef LABELED
   bool StandardMoveStep( const CDependencyParse &tree, const std::vector<CDependencyLabel>&m_lCacheLabel , const std::vector<CMorph>&m_lCacheMorph ) {
#else
   bool StandardMoveStep( const CDependencyParse &tree , const std::vector<CMorph>&m_lCacheMorph ) {
#endif
      static int top;
      // when the next word is tree.size() it means that the sentence is done already
      if ( m_nNextWord == static_cast<int>(tree.size()) ) {
         assert( m_Stack.size() > 0 );
         if ( m_Stack.size() > 1 ) {
            Reduce();
            return false;
         }
         else {
            PopRoot();
            return false;
         }
      }
      // the zeroth case is if the morphological cache isn't full and there are words in the buffer to shift into it, then we shift cache
      if ( cachesize() < MORPH_CACHE_LIMIT && m_nNextUncachedWord < static_cast<int>(tree.size()) )
      {
    	  //std::cout << "Shifting cache with word " << (*m_lCache)[m_nNextUncachedWord].str() << " morph " << m_lCacheMorph[m_nNextUncachedWord] << ": " << m_lCacheMorph[m_nNextUncachedWord].str() << "\n";
    	  ShiftCache(m_lCacheMorph[m_nNextUncachedWord].code());
    	  //std::cout << "Result of shifting cache: " << debugstring();
    	  return false;
      }

      // the first case is that there is some words on the stack linking to nextword
      if ( m_Stack.size() > 0 ) {
         top = m_Stack.back();
         while ( !(m_lHeads[top] == DEPENDENCY_LINK_NO_HEAD) )
            top = m_lHeads[top];
         if ( tree[top].head == m_nNextWord ) {    // if a local head deps on nextword first
            if ( top == m_Stack.back() ) {
#ifdef LABELED
               assert(m_lCacheLabel[top].str() == tree[top].label);
               ArcLeft(m_lCacheLabel[top].code()); // link it to the next word
#else
               ArcLeft();                          // link it to the next word
#endif
               return false;
            }
            else {
               Reduce();
               return false;
            }
         }
      }
      // the second case is that no words on the stack links nextword, and nextword does not link to stack word
      if ( tree[m_nNextWord].head == DEPENDENCY_LINK_NO_HEAD || // the root or
           tree[m_nNextWord].head > m_nNextWord ) { // head on the right
         Shift();
         return true;
      }
      // the last case is that the next words links to stack word
      else {                                        // head on the left 
         assert( m_Stack.size() > 0 );
         top = m_Stack.back(); 
         if ( tree[m_nNextWord].head == top ) {     // the next word deps on stack top
#ifdef LABELED
            assert(m_lCacheLabel[m_nNextWord].str()==tree[m_nNextWord].label);
            ArcRight(m_lCacheLabel[m_nNextWord].code());
#else            
            ArcRight();
#endif            
            return true;
         }
         else {                                     // must depend on non-immediate h
            Reduce();
            return false;
         }
      }
   }

   // we want to pop the root item after the whole tree done
   // on the one hand this seems more natural
   // on the other it is easier to score
   void StandardFinish() {
      assert( m_Stack.size() == 0 );
   }

   //item does not need to be a direct successor!
   unsigned long FollowMove( const CStateItem *item ) {
      static int top;

	  //std::cout << "FollowMove to get from " << debugstring() << " to " << item->debugstring() << "\n";

	  //do a shift cache if needed
	  if ( cachesize() < MORPH_CACHE_LIMIT && m_nNextUncachedWord < item->m_nNextUncachedWord )
	  {
		  //std::cout << "Going to do shift cache with morph " << (item->morph(m_nNextUncachedWord)).str() << "\n";
		  //we need to do a shift cache before anything else
		  return action::encodeAction(action::SHIFT_CACHE,item->morph(m_nNextUncachedWord).code());
	  }

      // if the next words are same then don't check head because it might be a finished sentence (m_nNextWord==sentence.sz)
      if ( m_nNextWord == item->m_nNextWord ) {
//std::cout << "this" << std::endl; for (int i=0; i<m_Stack.size(); ++i) std::cout << m_Stack[i] << " "; std::cout << std::endl;
//std::cout << "that" << std::endl; for (int i=0; i<item->m_Stack.size(); ++i) std::cout << item->m_Stack[i] << " "; std::cout << std::endl;
//         if ( m_Stack.size() == item->m_Stack.size() ) {
//std::cout << "this heads" << std::endl; for (int i=0; i<=m_nNextWord; ++i) std::cout << m_lHeads[i] << " "; std::cout << std::endl;
//std::cout << "that heads" << std::endl; for (int i=0; i<=item->m_nNextWord; ++i) std::cout << item->m_lHeads[i] << " "; std::cout << std::endl;
//std::cout << "this dtags" << std::endl; for (int i=0; i<=m_nNextWord; ++i) std::cout << m_lLabels[i] << " "; std::cout << std::endl;
//std::cout << "that dtags" << std::endl; for (int i=0; i<=item->m_nNextWord; ++i) std::cout << item->m_lLabels[i] << " "; std::cout << std::endl;
//         }


			 assert( m_Stack.size() > item->m_Stack.size() );

			 top = m_Stack.back();
			 if ( item->m_lHeads[top] == m_nNextWord )
#ifdef LABELED
				return action::encodeAction(action::ARC_LEFT, item->m_lLabels[top]);
#else
				return action::ARC_LEFT;
#endif
			 else if ( item->m_lHeads[top] != DEPENDENCY_LINK_NO_HEAD )
				return action::encodeAction(action::REDUCE);
			 else
				return action::encodeAction(action::POP_ROOT);

      }
      // the first case is that there is some words on the stack linking to nextword
      if ( m_Stack.size() > 0 ) {
         top = m_Stack.back();
         while ( !(m_lHeads[top] == DEPENDENCY_LINK_NO_HEAD) )
            top = m_lHeads[top];
         if ( item->head(top) == m_nNextWord ) {    // if a local head deps on nextword first
            if ( top == m_Stack.back() ) {
#ifdef LABELED
               return action::encodeAction(action::ARC_LEFT, item->m_lLabels[top]);
#else
               return action::ARC_LEFT;
#endif
            }
            else {
               return action::encodeAction(action::REDUCE);
            }
         }
      }
      // the second case is that no words on the stack links nextword, and nextword does not link to stack word
      if ( item->head(m_nNextWord) == DEPENDENCY_LINK_NO_HEAD || // the root or
           item->head(m_nNextWord) > m_nNextWord ) { // head on the right
         return action::encodeAction(action::SHIFT);
      }
      // the last case is that the next words links to stack word
      else {                                        // head on the left 
         assert( m_Stack.size() > 0 );
         top = m_Stack.back(); 
         if ( item->head(m_nNextWord) == top ) {    // the next word deps on stack top
#ifdef LABELED
            return action::encodeAction(action::ARC_RIGHT, item->m_lLabels[m_nNextWord]);
#else
            return action::ARC_RIGHT;
#endif
         }
         else {                                     // must depend on non-immediate h
            return action::encodeAction(action::REDUCE);
         }
      }
   }

   //TODO perfect this changing CDependencyParse to hold morph, lemma, etc.
   void GenerateTree( const CStringVector &input, CDependencyParse &output ) const {
      output.clear();
      for ( int i=0; i<size(); ++i ) 

#ifdef LABELED
    	  output.push_back( CExtendedLabeledDependencyTreeNode( input.at(i) , morphToPenn(m_lMorph[i]).str() , m_lHeads[i], "pending_feats" , m_lLemma[i].str() , CDependencyLabel(m_lLabels[i]).str() ) ) ;
         //output.push_back( CExtendedLabeledDependencyTreeNode( input.at(i) , m_lLemma[i].str() , m_lHeads[i] , CDependencyLabel(m_lLabels[i]).str() ) ) ;
#else
      	  output.push_back( CExtendedDependencyTreeNode( input.at(i) , morphToPenn(m_lMorph[i]).str() , m_lHeads[i], "pending_feats" , m_lLemma[i].str() ) ) ;
#endif
   }




};

#endif
