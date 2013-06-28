// Copyright (C) University of Oxford 2010
/*===============================================================
 *
 * stanford dependencies in ZPar  
 * Authors: Miguel Ballesteros and Yue Zhang
 * UPF-Spain and SUTD-Singapore
 *
 *==============================================================*/

#ifndef _COMMON_CON_PARSER_STATEITEM
#define _COMMON_CON_PARSER_STATEITEM

#include "action.h"
#include "linkclass.h"

/*===============================================================
 *
 * CStateNode - tree nodes 
 *
 *==============================================================*/

class CStateNode {
public:
   enum NODE_TYPE {LEAF=0, SINGLE_CHILD, HEAD_LEFT, HEAD_RIGHT};
public:
   int id;
   NODE_TYPE type;
   bool temp;
   // fields for constituents
   CConstituent constituent;
   const CStateNode* left_child;
   const CStateNode* right_child;
   // fields for tokens and constituents
   int lexical_head;
   int lexical_start;
   int lexical_end; 
 
   const CLink* stanfordTreeCollapsed; //miguel
   const CLink* stanfordTree; //miguel

public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const int &lexical_head, const int &lexical_start, const int &lexical_end) : id(id), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head), lexical_start(lexical_start), lexical_end(lexical_end) {
	   this->stanfordTree = 0; //miguel
	   this->stanfordTreeCollapsed = 0; //miguel. I think this is better since they are generated on the fly and they will be never like this.
   }
   CStateNode() : id(-1), type(), temp(0), constituent(), left_child(0), right_child(0), lexical_head(0), lexical_start(0), lexical_end(0), stanfordTreeCollapsed(0), stanfordTree(0) {}
   virtual ~CStateNode() {}
public:

   bool valid() const { return id!=-1; }
   void clear() { 
      this->id = -1;
      this->type = static_cast<NODE_TYPE>(0); 
      this->temp = 0; 
      this->constituent.clear(); 
      this->left_child = 0; 
      this->right_child = 0; 
      this->lexical_head = 0; 
      this->lexical_start = 0; 
      this->lexical_end = 0; 

      this->stanfordTree = 0; //miguel
      this->stanfordTreeCollapsed = 0; //miguel
   }
   void set(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const CStateNode *left_child, const CStateNode *right_child, const int &lexical_head, const int &lexical_start, const int &lexical_end) { 
      this->id = id;
      this->type = type; 
      this->temp = temp; 
      this->constituent = constituent; 
      this->left_child = left_child; 
      this->right_child = right_child; 
      this->lexical_head = lexical_head; 
      this->lexical_start = lexical_start; 
      this->lexical_end = lexical_end;
      
       //the stanford links are missing.  Let's see whether there are necessary or not. (Miguel)
   }//{}

   bool operator == (const CStateNode &nd) const {
      return id == nd.id &&
             type == nd.type && 
             temp == nd.temp && 
             constituent == nd.constituent && 
             left_child == nd.left_child && 
             right_child == nd.right_child &&
             lexical_head == nd.lexical_head;
             lexical_start == nd.lexical_start;
             lexical_end == nd.lexical_end;
             
             //the stanford links are missing. Let's see whether there are necessary or not. (Miguel)
   }
   void operator = (const CStateNode &nd) {
      id = nd.id;
      type = nd.type;
      temp = nd.temp;
      constituent.copy(nd.constituent);
      left_child = nd.left_child;
      right_child = nd.right_child;
      lexical_head = nd.lexical_head;
      lexical_start = nd.lexical_start;
      lexical_end = nd.lexical_end;
      
      //the stanford links are missing. Let's see whether there are necessary or not. (Miguel)
   }
public:
   void toCCFGTreeNode(CCFGTreeNode &node) const {
//      node.parent = parent->id;
      node.is_constituent = is_constituent();
      node.temp = temp;
#ifdef NO_TEMP_CONSTITUENT
      ASSERT(!node.temp, "Internal error: this version does not temporary constituents but state items have them.")
      node.constituent = constituent.code();
#else
      node.constituent = constituent.extractConstituentCode();
#endif
      node.single_child = single_child();
      node.head_left = head_left();
      node.left_child = left_child ? left_child->id : -1;
      node.right_child = right_child ? right_child->id : -1;
      node.token = lexical_head;
   }

public: //Miguel. added the set method for Stanford links.
   void setStanfordTree(CLink* stanfordTree) { 
   	this->stanfordTree = stanfordTree;	   
   }
   
   void setStanfordTreeCollapse(CLink* stanfordTreeCollapsed) { 
      	this->stanfordTreeCollapsed = stanfordTreeCollapsed;	   
      }
   
   void generateStanford(bool collapse, bool ccProcess, bool includeExtras, bool lemmatize) {
	   
	   //TODO MIGUEL
	   //this method should include the stanford dependencies from the left child and the right child
	   
	   //1st, create grammaticalstructure.
	   //2nd. use semantic head finder.
	   //3rd. add transformations.
	   //if a head is not included in the subtree just forget about it... so far, so good. Speak with yue about this.
	   //ASK TO YUE HOW TO DEBUG THIS WITH A SMALL CORPUS.
	   
	   
   }
   
  
};

/*===============================================================
 * CHeadFinder 
 * Implements a 'semantic head' variant of the the HeadFinder found
 * in Michael Collins' 1999 thesis.
 *  See SemanticHeadFinder in Stanford parser.
 ===============================================================*/
 
 class CHeadFinder {
	 
	 /*
	  * 
	  *  private static final String[] auxiliaries = {"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" };
  private static final String[] beGetVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"};
  private static final String[] copulaVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"};

  private static final String[] verbTags = {"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"};
	  */
 public:
	 
	 //collinsheadfinder and abstractcollinsheadfinder and ModCollinsHeadFinder
	 std::map<std::string, std::string**> nonTerminalInfo;
	 
	 
	 //SemanticHeadFinder
	 std::string verbTags[10];
	 std::string auxiliaries[32];
	 std::string copulateVerbs[32];
	 std::string beGetVerbs[18];
	 
	 /*
	  *private static final String[] auxiliaries = {"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" };
	   private static final String[] beGetVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"};
	   private static final String[] copulaVerbs = {"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"}
	   private static final String[] verbTags = {"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"};
	  */
	 
 	public:
 		int id;
 		
 		CHeadFinder() : verbTags({"TO", "MD", "VB", "VBD", "VBP", "VBZ", "VBG", "VBN", "AUX", "AUXG"}), 
 					auxiliaries({"will", "wo", "shall", "sha", "may", "might", "should", "would", "can", "could", "ca", "must", "has", "have", "had", "having", "get", "gets", "getting", "got", "gotten", "do", "does", "did", "to", "'ve", "ve", "'d", "d", "'ll", "ll", "na" }),
 					copulateVerbs({"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"}),
 					beGetVerbs({"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "get", "getting", "gets", "got"})
 		{
 			
 			//MIGUEL: there is no implementation of the Constructor yet. THERE MUST BE.
 			
 			//This is from ModCollinsHeadFinder (Stanford Parser code)
 			//nonTerminalInfo.put("ADJP", ....hecho {"rightdis", "JJP", "JJR", "JJS", "DT", "RB", "RBR", "CD", "IN", "VBD"}, {"left", "ADVP", "NP"}});
 			nonTerminalInfo["ADJP"]=new std::string*[5];
 			nonTerminalInfo["ADJP"][0]=new std::string[2];
 			nonTerminalInfo["ADJP"][0][0]="left";
 			nonTerminalInfo["ADJP"][0][1]="$";
 			nonTerminalInfo["ADJP"][1]=new std::string[7];
 			nonTerminalInfo["ADJP"][1][0]="rightdis";
 			nonTerminalInfo["ADJP"][1][1]="NNS";
 			nonTerminalInfo["ADJP"][1][2]="NN";
 			nonTerminalInfo["ADJP"][1][3]="JJ";
 			nonTerminalInfo["ADJP"][1][4]="QP";
 			nonTerminalInfo["ADJP"][1][5]="VBN";
 			nonTerminalInfo["ADJP"][1][6]="VBG";
 			nonTerminalInfo["ADJP"][2]=new std::string[2];
 			nonTerminalInfo["ADJP"][2][0]="left";
 			nonTerminalInfo["ADJP"][2][1]="ADJP";
 			nonTerminalInfo["ADJP"][3]=new std::string[10];
 			nonTerminalInfo["ADJP"][3][0]="rightdis";
 			nonTerminalInfo["ADJP"][3][1]="JJP";
 			nonTerminalInfo["ADJP"][3][2]="JJR";
 			nonTerminalInfo["ADJP"][3][3]="JJS";
 			nonTerminalInfo["ADJP"][3][4]="DT";
 			nonTerminalInfo["ADJP"][3][5]="RB";
 			nonTerminalInfo["ADJP"][3][6]="RBR";
 			nonTerminalInfo["ADJP"][3][7]="CD";
 			nonTerminalInfo["ADJP"][3][8]="IN";
 			nonTerminalInfo["ADJP"][3][9]="VBD";
 			nonTerminalInfo["ADJP"][4]=new std::string[3];
 			nonTerminalInfo["ADJP"][4][0]="left";
 			nonTerminalInfo["ADJP"][4][1]="ADVP";			
 			nonTerminalInfo["ADJP"][4][1]="NP";
 			
 			//nonTerminalInfo.put("JJP", new String[][]{{"left", "NNS", "NN", "$", "QP", "JJ", "VBN", "VBG", "ADJP", "JJP", "JJR", "NP", "JJS", "DT", "FW", "RBR", "RBS", "SBAR", "RB"}});  // JJP is introduced for NML-like adjective phrases in Vadas' treebank; Chris wishes he hadn't used JJP which should be a POS-tag. 			
 			//The people from stanford think that perhaps this should not be included.
 			nonTerminalInfo["JJP"]=new std::string*[1];
 			nonTerminalInfo["JJP"][0]=new std::string[19];
 			nonTerminalInfo["JJP"][0][0]="left";
 			nonTerminalInfo["JJP"][0][1]="NNS";
 			nonTerminalInfo["JJP"][0][2]="NN";
 			nonTerminalInfo["JJP"][0][3]="$";
 			nonTerminalInfo["JJP"][0][4]="QP";
 			nonTerminalInfo["JJP"][0][5]="JJ";
 			nonTerminalInfo["JJP"][0][6]="VBN";
 			nonTerminalInfo["JJP"][0][7]="VBG";
 			nonTerminalInfo["JJP"][0][8]="ADJP";
 			nonTerminalInfo["JJP"][0][9]="JJP";
 			nonTerminalInfo["JJP"][0][10]="JJR";
 			nonTerminalInfo["JJP"][0][11]="NP";
 			nonTerminalInfo["JJP"][0][12]="JJS";
 			nonTerminalInfo["JJP"][0][13]="DT";
 			nonTerminalInfo["JJP"][0][14]="FW";
 			nonTerminalInfo["JJP"][0][15]="RBR";
 			nonTerminalInfo["JJP"][0][16]="RBS";
 			nonTerminalInfo["JJP"][0][17]="SBAR";
 			nonTerminalInfo["JJP"][0][18]="RB";
 			
 			//// ADVP rule rewritten by Chris in Nov 2010 to be rightdis.  This is right! JJ.* is often head and rightmost. //Comment in STF code.
 			//nonTerminalInfo.put("ADVP", new String[][]{{"left", "ADVP", "IN"},
 			// 			        {"rightdis", "RB", "RBR", "RBS", "JJ", "JJR", "JJS"},
 			// 			        {"rightdis", "RP", "DT", "NN", "CD", "NP", "VBN", "NNP", "CC", "FW", "NNS", "ADJP", "NML"}});
 			
 			nonTerminalInfo["ADVP"]=new std::string*[3];
 			nonTerminalInfo["ADVP"][0]=new std::string[3];
 			nonTerminalInfo["ADVP"][0][0]="left";
 			nonTerminalInfo["ADVP"][0][1]="ADVP";
 			nonTerminalInfo["ADVP"][0][2]="IN";
 			nonTerminalInfo["ADVP"][1]=new std::string[7];
 			nonTerminalInfo["ADVP"][1][0]="rightdis";
 			nonTerminalInfo["ADVP"][1][1]="RB";
 			nonTerminalInfo["ADVP"][1][2]="RBR";
 			nonTerminalInfo["ADVP"][1][3]="RBS";
 			nonTerminalInfo["ADVP"][1][4]="JJ";
 			nonTerminalInfo["ADVP"][1][5]="JJR";
 			nonTerminalInfo["ADVP"][1][6]="JJS";
 			nonTerminalInfo["ADVP"][2]=new std::string[13];
 			nonTerminalInfo["ADVP"][2][0]="rightdis";
 			nonTerminalInfo["ADVP"][2][1]="RP";
 			nonTerminalInfo["ADVP"][2][2]="DT";
 			nonTerminalInfo["ADVP"][2][3]="NN";
 			nonTerminalInfo["ADVP"][2][4]="CD";
 			nonTerminalInfo["ADVP"][2][5]="NP";
 			nonTerminalInfo["ADVP"][2][6]="VBN";
 			nonTerminalInfo["ADVP"][2][7]="NNP";
 			nonTerminalInfo["ADVP"][2][8]="CC";
 			nonTerminalInfo["ADVP"][2][9]="FW";
 			nonTerminalInfo["ADVP"][2][10]="NNS";
 			nonTerminalInfo["ADVP"][2][11]="ADJP";
 			nonTerminalInfo["ADVP"][2][12]="NML";
 			
 			//nonTerminalInfo.put("CONJP", new String[][]{{"right", "CC", "RB", "IN"}});
 			
 			/*nonTerminalInfo["CONJP"]=new std::string*[1];
 			nonTerminalInfo["CONJP"][0]=new std::string[4];
 			nonTerminalInfo["CONJP"][0][0]="right";
 			nonTerminalInfo["CONJP"][0][1]="CC";
 			nonTerminalInfo["CONJP"][0][2]="RB";
 			nonTerminalInfo["CONJP"][0][3]="IN";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["CONJP"]=new std::string*[1];
 			 			nonTerminalInfo["CONJP"][0]=new std::string[6];
 			 			nonTerminalInfo["CONJP"][0][0]="right";
 			 			nonTerminalInfo["CONJP"][0][1]="VB";
 			 			nonTerminalInfo["CONJP"][0][2]="JJ";
 			 			nonTerminalInfo["CONJP"][0][3]="RB";
 			 			nonTerminalInfo["CONJP"][0][4]="IN";
 			 			nonTerminalInfo["CONJP"][0][5]="CC";
 			
 			
 			//nonTerminalInfo.put("FRAG", new String[][]{{"right"}}); // crap
 			/*nonTerminalInfo["FRAG"]=new std::string*[1];
 			nonTerminalInfo["FRAG"][0]=new std::string[1];
 			nonTerminalInfo["FRAG"][0][0]="right";*/
 			 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			 			nonTerminalInfo["FRAG"]=new std::string*[4];
 			 			nonTerminalInfo["FRAG"][0]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][0][0]="left";
 			 			nonTerminalInfo["FRAG"][0][1]="IN";
 			 			nonTerminalInfo["FRAG"][1]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][1][0]="right";
 			 			nonTerminalInfo["FRAG"][1][1]="RB";
 			 			nonTerminalInfo["FRAG"][2]=new std::string[2];
 			 			nonTerminalInfo["FRAG"][2][0]="left";
 			 			nonTerminalInfo["FRAG"][2][1]="NP";
 			 			nonTerminalInfo["FRAG"][3]=new std::string[7];
 			 			nonTerminalInfo["FRAG"][3][0]="left";
 			 			nonTerminalInfo["FRAG"][3][1]="ADJP";
 			 			nonTerminalInfo["FRAG"][3][2]="ADVP";
 			 			nonTerminalInfo["FRAG"][3][3]="FRAG"; 			 			
 			 			nonTerminalInfo["FRAG"][3][4]="S";
 			 			nonTerminalInfo["FRAG"][3][5]="SBAR";
 			 			nonTerminalInfo["FRAG"][3][6]="VP";
 			 			
 			
			//nonTerminalInfo.put("INTJ", new String[][]{{"left"}});
 			nonTerminalInfo["INTJ"]=new std::string*[1];
 			nonTerminalInfo["INTJ"][0]=new std::string[1];
 			nonTerminalInfo["INTJ"][0][0]="left";
 			
 			//nonTerminalInfo.put("LST", new String[][]{{"right", "LS", ":"}});
 			nonTerminalInfo["LST"]=new std::string*[1];
 			nonTerminalInfo["LST"][0]=new std::string[3];
 			nonTerminalInfo["LST"][0][0]="right";
 			nonTerminalInfo["LST"][0][1]="LS";
 			nonTerminalInfo["LST"][0][2]=":";

 			// NML is head in: (NAC-LOC (NML San Antonio) (, ,) (NNP Texas))
 			// TODO: NNP should be head (rare cases, could be ignored):
 			//   (NAC (NML New York) (NNP Court) (PP of Appeals))
 			//   (NAC (NML Prudential Insurance) (NNP Co.) (PP Of America))
 			// Chris: This could maybe still do with more thought, but NAC is rare.
 			//nonTerminalInfo.put("NAC", new String[][]{{"left", "NN", "NNS", "NML", "NNP", "NNPS", "NP", "NAC", "EX", "$", "CD", "QP", "PRP", "VBG", "JJ", "JJS", "JJR", "ADJP", "JJP", "FW"}});
 			nonTerminalInfo["NAC"]=new std::string*[1];
 			nonTerminalInfo["NAC"][0]=new std::string[2];
 			nonTerminalInfo["NAC"][0][0]="left";
 			nonTerminalInfo["NAC"][0][1]="NN";
 			nonTerminalInfo["NAC"][0][2]="NNS";
 			nonTerminalInfo["NAC"][0][3]="NML";
 			nonTerminalInfo["NAC"][0][4]="NNP";
 			nonTerminalInfo["NAC"][0][5]="NNPS";
 			nonTerminalInfo["NAC"][0][6]="NP";
 			nonTerminalInfo["NAC"][0][7]="NAC";
 			nonTerminalInfo["NAC"][0][8]="EX";
 			nonTerminalInfo["NAC"][0][9]="$";
 			nonTerminalInfo["NAC"][0][10]="CD";
 			nonTerminalInfo["NAC"][0][11]="QP";
 			nonTerminalInfo["NAC"][0][12]="PRP";
 			nonTerminalInfo["NAC"][0][13]="VBG";
 			nonTerminalInfo["NAC"][0][14]="JJ";
 			nonTerminalInfo["NAC"][0][15]="JJS";
 			nonTerminalInfo["NAC"][0][16]="JJR";
 			nonTerminalInfo["NAC"][0][17]="ADJP";
 			nonTerminalInfo["NAC"][0][18]="JJP";
 			nonTerminalInfo["NAC"][0][19]="FW";
 			
			//nonTerminalInfo.put("NX", new String[][]{{"right", "NP", "NX"}});
 			nonTerminalInfo["NX"]=new std::string*[1];
 			nonTerminalInfo["NX"][0]=new std::string[3];
 			nonTerminalInfo["NX"][0][0]="right";
 			nonTerminalInfo["NX"][0][1]="NP";
 			nonTerminalInfo["NX"][0][2]="NX";
 			
 			//nonTerminalInfo.put("PP", new String[][]{{"right", "IN", "TO", "VBG", "VBN", "RP", "FW", "JJ", "SYM"}, {"left", "PP"}});
 			
 			nonTerminalInfo["PP"]=new std::string*[2];
 			nonTerminalInfo["PP"][0]=new std::string[9];
 			nonTerminalInfo["PP"][0][0]="right";
 			nonTerminalInfo["PP"][0][1]="IN";
 			nonTerminalInfo["PP"][0][2]="TO";
 			nonTerminalInfo["PP"][0][3]="VBG";
 			nonTerminalInfo["PP"][0][4]="VBN";
 			nonTerminalInfo["PP"][0][5]="RP";
 			nonTerminalInfo["PP"][0][6]="FW";
 			nonTerminalInfo["PP"][0][7]="JJ";
 			nonTerminalInfo["PP"][0][8]="IN";
 			nonTerminalInfo["PP"][1]=new std::string[2];
 			nonTerminalInfo["PP"][1][0]="left";
 			nonTerminalInfo["PP"][1][1]="PP";
 			
 			//nonTerminalInfo.put("PRN", new String[][]{{"left", "VP", "NP", "PP", "SQ", "S", "SINV", "SBAR", "ADJP", "JJP", "ADVP", "INTJ", "WHNP", "NAC", "VBP", "JJ", "NN", "NNP"}});
 			
 			/*nonTerminalInfo["PRN"]=new std::string*[1];
 			nonTerminalInfo["PRN"][0]=new std::string[18];
 			nonTerminalInfo["PRN"][0][0]="left";
 			nonTerminalInfo["PRN"][0][1]="VP";
 			nonTerminalInfo["PRN"][0][2]="NP";
 			nonTerminalInfo["PRN"][0][3]="PP";
 			nonTerminalInfo["PRN"][0][4]="SQ";
 			nonTerminalInfo["PRN"][0][5]="S";
 			nonTerminalInfo["PRN"][0][6]="SINV";
 			nonTerminalInfo["PRN"][0][7]="SBAR";
 			nonTerminalInfo["PRN"][0][8]="ADJP";
 			nonTerminalInfo["PRN"][0][9]="JJP";
 			nonTerminalInfo["PRN"][0][10]="ADVP";
 			nonTerminalInfo["PRN"][0][11]="INTJ";
 			nonTerminalInfo["PRN"][0][12]="WHNP";
 			nonTerminalInfo["PRN"][0][13]="NAC";
 			nonTerminalInfo["PRN"][0][14]="VBP";
 			nonTerminalInfo["PRN"][0][15]="JJ";
 			nonTerminalInfo["PRN"][0][16]="NN";
 			nonTerminalInfo["PRN"][0][17]="NNP";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			nonTerminalInfo["PRN"]=new std::string*[1];
 			nonTerminalInfo["PRN"][0]=new std::string[17];
 			nonTerminalInfo["PRN"][0][0]="left";
 			nonTerminalInfo["PRN"][0][1]="VP";
 			nonTerminalInfo["PRN"][0][2]="SQ";
 			nonTerminalInfo["PRN"][0][3]="S";
 			nonTerminalInfo["PRN"][0][4]="SINV";
 			nonTerminalInfo["PRN"][0][5]="SBAR";
 			nonTerminalInfo["PRN"][0][6]="NP";
 			nonTerminalInfo["PRN"][0][7]="ADJP";
 			nonTerminalInfo["PRN"][0][8]="PP";
 			nonTerminalInfo["PRN"][0][9]="ADVP";
 			nonTerminalInfo["PRN"][0][10]="INTJ";
 			nonTerminalInfo["PRN"][0][11]="WHNP";
 			nonTerminalInfo["PRN"][0][12]="NAC";
 			nonTerminalInfo["PRN"][0][13]="VBP";
 			nonTerminalInfo["PRN"][0][14]="JJ";
 			nonTerminalInfo["PRN"][0][15]="NN";
 			nonTerminalInfo["PRN"][0][16]="NNP";
 			
 			//nonTerminalInfo.put("PRT", new String[][]{{"right", "RP"}});
 			nonTerminalInfo["PRT"]=new std::string*[1];
 			nonTerminalInfo["PRT"][0]=new std::string[2];
 			nonTerminalInfo["PRT"][0][0]="right";
 			nonTerminalInfo["PRT"][0][1]="RP";
 			
 			// add '#' for pounds!! (this comment in stf code ???)
 			//nonTerminalInfo.put("QP", new String[][]{{"left", "$", "IN", "NNS", "NN", "JJ", "CD", "PDT", "DT", "RB", "NCD", "QP", "JJR", "JJS"}});
 			/*nonTerminalInfo["QP"]=new std::string*[1];
 			nonTerminalInfo["QP"][0]=new std::string[14];
 			nonTerminalInfo["QP"][0][0]="left";
 			nonTerminalInfo["QP"][0][1]="$";
 			nonTerminalInfo["QP"][0][2]="IN";
 			nonTerminalInfo["QP"][0][3]="NNS";
 			nonTerminalInfo["QP"][0][4]="NN";
 			nonTerminalInfo["QP"][0][5]="JJ";
 			nonTerminalInfo["QP"][0][6]="CD";
 			nonTerminalInfo["QP"][0][7]="PDT";
 			nonTerminalInfo["QP"][0][8]="DT";
 			nonTerminalInfo["QP"][0][9]="RB";
 			nonTerminalInfo["QP"][0][10]="NCD";
 			nonTerminalInfo["QP"][0][11]="QP";
 			nonTerminalInfo["QP"][0][12]="JJR";
 			nonTerminalInfo["QP"][0][13]="JJS";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["QP"]=new std::string*[1];
 			 			nonTerminalInfo["QP"][0]=new std::string[14];
 			 			nonTerminalInfo["QP"][0][0]="right";
 			 			nonTerminalInfo["QP"][0][1]="$";
 			 			nonTerminalInfo["QP"][0][2]="NNS";
 			 			nonTerminalInfo["QP"][0][3]="NN";
 			 			nonTerminalInfo["QP"][0][4]="CD";
 			 			nonTerminalInfo["QP"][0][5]="JJ";
 			 			nonTerminalInfo["QP"][0][6]="PDT";
 			 			nonTerminalInfo["QP"][0][7]="DT";
 			 			nonTerminalInfo["QP"][0][8]="IN";
 			 			nonTerminalInfo["QP"][0][9]="RB";
 			 			nonTerminalInfo["QP"][0][10]="NCD";
 			 			nonTerminalInfo["QP"][0][11]="QP";
 			 			nonTerminalInfo["QP"][0][12]="JJR";
 			 			nonTerminalInfo["QP"][0][13]="JJS";
 			
 			//nonTerminalInfo.put("RRC", new String[][]{{"right", "VP", "NP", "ADVP", "ADJP", "JJP", "PP"}});
 			nonTerminalInfo["RRC"]=new std::string*[1];
 			nonTerminalInfo["RRC"][0]=new std::string[7];
 			nonTerminalInfo["RRC"][0][0]="right";
 			nonTerminalInfo["RRC"][0][1]="VP";
 			nonTerminalInfo["RRC"][0][2]="NP";
 			nonTerminalInfo["RRC"][0][3]="ADVP";
 			nonTerminalInfo["RRC"][0][4]="ADJP";
 			nonTerminalInfo["RRC"][0][5]="JJP";
 			nonTerminalInfo["RRC"][0][6]="PP";
 			
 			 // delete IN -- go for main part of sentence; add FRAG ...this comment in stf code ??
 			
 			//nonTerminalInfo.put("S", new String[][]{{"left", "TO", "VP", "S", "FRAG", "SBAR", "ADJP", "JJP", "UCP", "NP"}});
 			/*nonTerminalInfo["S"]=new std::string*[1];
 			nonTerminalInfo["S"][0]=new std::string[10];
 			nonTerminalInfo["S"][0][0]="left";
 			nonTerminalInfo["S"][0][1]="TO";
 			nonTerminalInfo["S"][0][2]="VP";
 			nonTerminalInfo["S"][0][3]="S";
 			nonTerminalInfo["S"][0][4]="FRAG";
 			nonTerminalInfo["S"][0][5]="SBAR";
 			nonTerminalInfo["S"][0][6]="ADJP";
 			nonTerminalInfo["S"][0][7]="JJP";
 			nonTerminalInfo["S"][0][8]="UCP";
 			nonTerminalInfo["S"][0][9]="NP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			nonTerminalInfo["S"]=new std::string*[2];
 			nonTerminalInfo["S"][0]=new std::string[7];
 			nonTerminalInfo["S"][0][0]="left";
 			nonTerminalInfo["S"][0][1]="VP";
 			nonTerminalInfo["S"][0][1]="S";
 			nonTerminalInfo["S"][0][2]="FRAG";
 			nonTerminalInfo["S"][0][3]="SBAR";
 			nonTerminalInfo["S"][0][4]="ADJP";
 			nonTerminalInfo["S"][0][5]="UCP";
 			nonTerminalInfo["S"][0][6]="TO";
 			nonTerminalInfo["S"][0]=new std::string[2];
 			nonTerminalInfo["S"][1][0]="right";
 			nonTerminalInfo["S"][1][1]="NP";
 			
 			
 		   //nonTerminalInfo.put("SBAR", new String[][]{{"left", "WHNP", "WHPP", "WHADVP", "WHADJP", "IN", "DT", "S", "SQ", "SINV", "SBAR", "FRAG"}});
 			/*nonTerminalInfo["SBAR"]=new std::string*[1];
 			nonTerminalInfo["SBAR"][0]=new std::string[12];
 			nonTerminalInfo["SBAR"][0][0]="left";
 			nonTerminalInfo["SBAR"][0][1]="WHNP";
 			nonTerminalInfo["SBAR"][0][2]="WHPP";
 			nonTerminalInfo["SBAR"][0][3]="WHADVP";
 			nonTerminalInfo["SBAR"][0][4]="WHADJP";
 			nonTerminalInfo["SBAR"][0][5]="IN";
 			nonTerminalInfo["SBAR"][0][6]="DT";
 			nonTerminalInfo["SBAR"][0][7]="S";
 			nonTerminalInfo["SBAR"][0][8]="SQ";
 			nonTerminalInfo["SBAR"][0][9]="SINV";
 			nonTerminalInfo["SBAR"][0][10]="SBAR";
 			nonTerminalInfo["SBAR"][0][11]="FRAG";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["SBAR"]=new std::string*[1];
 			 			nonTerminalInfo["SBAR"][0]=new std::string[13];
 			 			nonTerminalInfo["SBAR"][0][0]="left";
 			 			nonTerminalInfo["SBAR"][0][1]="S";
 			 			nonTerminalInfo["SBAR"][0][2]="SQ";
 			 			nonTerminalInfo["SBAR"][0][3]="SINV";
 			 			nonTerminalInfo["SBAR"][0][4]="SBAR";
 			 			nonTerminalInfo["SBAR"][0][5]="FRAG";
 			 			nonTerminalInfo["SBAR"][0][6]="VP";
 			 			nonTerminalInfo["SBAR"][0][7]="WHNP";
 			 			nonTerminalInfo["SBAR"][0][8]="WHPP";
 			 			nonTerminalInfo["SBAR"][0][9]="WHADVP";
 			 			nonTerminalInfo["SBAR"][0][10]="WHADJP";
 			 			nonTerminalInfo["SBAR"][0][11]="IN";
 			 			nonTerminalInfo["SBAR"][0][12]="DT";
 			
 			
 			// nonTerminalInfo.put("SBARQ", new String[][]{{"left", "SQ", "S", "SINV", "SBARQ", "FRAG", "SBAR"}});
 			nonTerminalInfo["SBARQ"]=new std::string*[1];
 			nonTerminalInfo["SBARQ"][0]=new std::string[7];
 			nonTerminalInfo["SBARQ"][0][0]="left";
 			nonTerminalInfo["SBARQ"][0][1]="SQ";
 			nonTerminalInfo["SBARQ"][0][2]="S";
 			nonTerminalInfo["SBARQ"][0][3]="SINV";
 			nonTerminalInfo["SBARQ"][0][4]="SBARQ";
 			nonTerminalInfo["SBARQ"][0][5]="FRAG";
 			nonTerminalInfo["SBARQ"][0][6]="SBAR";

 			//This comment in stf code. cdm: if you have 2 VP under an SINV, you should really take the 2nd as syntactic head, because the first is a topicalized VP complement of the second, but for now I didn't change this, since it didn't help parsing.  (If it were changed, it'd need to be also changed to the opposite in SemanticHeadFinder.)
 			//nonTerminalInfo.put("SINV", new String[][]{{"left", "VBZ", "VBD", "VBP", "VB", "MD", "VBN", "VP", "S", "SINV", "ADJP", "JJP", "NP"}});
 			nonTerminalInfo["SINV"]=new std::string*[1];
 			nonTerminalInfo["SINV"][0]=new std::string[12];
 			nonTerminalInfo["SINV"][0][0]="left";
 			nonTerminalInfo["SINV"][0][1]="VBZ";
 			nonTerminalInfo["SINV"][0][2]="VBD";
 			nonTerminalInfo["SINV"][0][3]="VBP";
 			nonTerminalInfo["SINV"][0][4]="VB";
 			nonTerminalInfo["SINV"][0][5]="MD";
 			nonTerminalInfo["SINV"][0][6]="VP";
 			nonTerminalInfo["SINV"][0][7]="S";
 			nonTerminalInfo["SINV"][0][8]="SINV";
 			nonTerminalInfo["SINV"][0][9]="ADJP";
 			nonTerminalInfo["SINV"][0][10]="JJP";
 			nonTerminalInfo["SINV"][0][11]="NP";
 			
 			// nonTerminalInfo.put("SQ", new String[][]{{"left", "VBZ", "VBD", "VBP", "VB", "MD", "AUX", "AUXG", "VP", "SQ"}});  // TODO: Should maybe put S before SQ for tag questions. Check. This comment in stf dependencies.
 			/*nonTerminalInfo["SQ"]=new std::string*[1];
 			nonTerminalInfo["SQ"][0]=new std::string[10];
 			nonTerminalInfo["SQ"][0][0]="left";
 			nonTerminalInfo["SQ"][0][1]="VBZ";
 			nonTerminalInfo["SQ"][0][2]="VBD";
 			nonTerminalInfo["SQ"][0][3]="VBP";
 			nonTerminalInfo["SQ"][0][4]="VB";
 			nonTerminalInfo["SQ"][0][5]="MD";
 			nonTerminalInfo["SQ"][0][6]="AUX";
 			nonTerminalInfo["SQ"][0][7]="AUXG";
 			nonTerminalInfo["SQ"][0][8]="VP";
 			nonTerminalInfo["SQ"][0][9]="SQ";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["SQ"]=new std::string*[1];
 			 			nonTerminalInfo["SQ"][0]=new std::string[10];
 			 			nonTerminalInfo["SQ"][0][0]="left";
 			 			nonTerminalInfo["SQ"][0][1]="VP";
 			 			nonTerminalInfo["SQ"][0][2]="SQ";
 			 			nonTerminalInfo["SQ"][0][3]="ADJP";
 			 			nonTerminalInfo["SQ"][0][4]="VB";
 			 			nonTerminalInfo["SQ"][0][5]="VBZ";
 			 			nonTerminalInfo["SQ"][0][6]="VBD";
 			 			nonTerminalInfo["SQ"][0][7]="VBP";
 			 			nonTerminalInfo["SQ"][0][8]="MD";
 			 			nonTerminalInfo["SQ"][0][9]="AUX";
 			 			nonTerminalInfo["SQ"][0][10]="AUX";
 			 			nonTerminalInfo["SQ"][0][11]="AUXG";
 			
			//nonTerminalInfo.put("UCP", new String[][]{{"right"}});
 			/*nonTerminalInfo["UCP"]=new std::string*[1];
 			nonTerminalInfo["UCP"][0]=new std::string[1];
 			nonTerminalInfo["UCP"][0][0]="right";*/
 			 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["UCP"]=new std::string*[1];
 			 			nonTerminalInfo["UCP"][0]=new std::string[1];
 			 			nonTerminalInfo["UCP"][0][0]="left";
 			
 			// below is weird!! Make 2 lists, one for good and one for bad heads?? ANOTHER COMMENT IN STF CODE
		    // VP: added AUX and AUXG to work with Charniak tags AND ANOTHER ONE
 			
 			//nonTerminalInfo.put("VP", new String[][]{{"left", "TO", "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "AUX", "AUXG", "ADJP", "JJP", "NN", "NNS", "JJ", "NP", "NNP"}});
 			nonTerminalInfo["VP"]=new std::string*[1];
 			nonTerminalInfo["VP"][0]=new std::string[19];
 			nonTerminalInfo["VP"][0][0]="left";
 			nonTerminalInfo["VP"][0][1]="TO";
 			nonTerminalInfo["VP"][0][2]="VBD";
 			nonTerminalInfo["VP"][0][3]="VBN";
 			nonTerminalInfo["VP"][0][4]="MD";
 			nonTerminalInfo["VP"][0][5]="VBZ";
 			nonTerminalInfo["VP"][0][6]="VB";
 			nonTerminalInfo["VP"][0][7]="VBG";
 			nonTerminalInfo["VP"][0][8]="VBP";
 			nonTerminalInfo["VP"][0][9]="VP";
 			nonTerminalInfo["VP"][0][10]="AUX";
 			nonTerminalInfo["VP"][0][11]="AUXG";
 			nonTerminalInfo["VP"][0][12]="ADJP";
 			nonTerminalInfo["VP"][0][13]="JJP";
 			nonTerminalInfo["VP"][0][14]="NN";
 			nonTerminalInfo["VP"][0][15]="NNS";
 			nonTerminalInfo["VP"][0][16]="JJ";
 			nonTerminalInfo["VP"][0][17]="NP";
 			nonTerminalInfo["VP"][0][18]="NNP";
 			
 			//nonTerminalInfo.put("WHADJP", new String[][]{{"left", "WRB", "WHADVP", "RB", "JJ", "ADJP", "JJP", "JJR"}});
 			
 			/*nonTerminalInfo["WHADJP"]=new std::string*[1];
 			nonTerminalInfo["WHADJP"][0]=new std::string[8];
 			nonTerminalInfo["WHADJP"][0][0]="left";
 			nonTerminalInfo["WHADJP"][0][1]="WRB";
 			nonTerminalInfo["WHADJP"][0][2]="WHADVP";
 			nonTerminalInfo["WHADJP"][0][3]="RB";
 			nonTerminalInfo["WHADJP"][0][4]="JJ";
 			nonTerminalInfo["WHADJP"][0][5]="ADJP";
 			nonTerminalInfo["WHADJP"][0][6]="JJP";
 			nonTerminalInfo["WHADJP"][0][7]="JJR";*/
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["WHADJP"]=new std::string*[3];
 			 			nonTerminalInfo["WHADJP"][0]=new std::string[4];
 			 			nonTerminalInfo["WHADJP"][0][0]="left";
 			 			nonTerminalInfo["WHADJP"][0][1]="ADJP";
 			 			nonTerminalInfo["WHADJP"][0][2]="JJ";
 			 			nonTerminalInfo["WHADJP"][0][3]="JJR";
 			 			nonTerminalInfo["WHADJP"][1]=new std::string[2];
 			 			nonTerminalInfo["WHADJP"][1][0]="right";
 			 			nonTerminalInfo["WHADJP"][1][1]="RB";
 			 			nonTerminalInfo["WHADJP"][2]=new std::string[2];
 			 			nonTerminalInfo["WHADJP"][2][0]="right";
 			
 			
 			  //nonTerminalInfo.put("WHADVP", new String[][]{{"right", "WRB", "WHADVP"}}); 
 			/*nonTerminalInfo["WHADVP"]=new std::string*[1];
 			nonTerminalInfo["WHADVP"][0]=new std::string[3];
 			nonTerminalInfo["WHADVP"][0][0]="right";
 			nonTerminalInfo["WHADVP"][0][1]="WRB";
 			nonTerminalInfo["WHADVP"][0][2]="WHADVP";*/
 			 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 			 nonTerminalInfo["WHADVP"]=new std::string*[1];
 			 nonTerminalInfo["WHADVP"][0]=new std::string[5];
 			 nonTerminalInfo["WHADVP"][0][0]="rightdis";
 			 nonTerminalInfo["WHADVP"][0][1]="WRB";
 			 nonTerminalInfo["WHADVP"][0][2]="WHADVP";
 			nonTerminalInfo["WHADVP"][0][3]="RB";
 			nonTerminalInfo["WHADVP"][0][4]="JJ";
 			 			//nonTerminalInfo.put("WHNP", new String[][]{{"left", "WDT", "WP", "WP$", "WHADJP", "WHPP", "WHNP"}});
 			/*nonTerminalInfo["WHNP"]=new std::string*[1];
 			nonTerminalInfo["WHNP"][0]=new std::string[7];
 			nonTerminalInfo["WHNP"][0][0]="left";
 			nonTerminalInfo["WHNP"][0][1]="WDT";
 			nonTerminalInfo["WHNP"][0][2]="WP";
 			nonTerminalInfo["WHNP"][0][3]="WP$";
 			nonTerminalInfo["WHNP"][0][4]="WHADJP";
 			nonTerminalInfo["WHNP"][0][5]="WHPP";
 			nonTerminalInfo["WHNP"][0][6]="WHNP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 						nonTerminalInfo["WHNP"]=new std::string*[4];
 			 			nonTerminalInfo["WHNP"][0]=new std::string[8];
 			 			nonTerminalInfo["WHNP"][0][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][0][1]="NN";
 			 			nonTerminalInfo["WHNP"][0][2]="NNP";
 			 			nonTerminalInfo["WHNP"][0][3]="NNS";
 			 			nonTerminalInfo["WHNP"][0][4]="NX";
 			 			nonTerminalInfo["WHNP"][0][5]="NML";
 			 			nonTerminalInfo["WHNP"][0][6]="JJR";
 			 			nonTerminalInfo["WHNP"][0][7]="WP";
 			 			nonTerminalInfo["WHNP"][1]=new std::string[3];
 			 			nonTerminalInfo["WHNP"][1][0]="left";
 			 			nonTerminalInfo["WHNP"][1][1]="WHNP";
 			 			nonTerminalInfo["WHNP"][1][2]="NP";
 			 			nonTerminalInfo["WHNP"][2]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][2][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][2][1]="$";
 			 			nonTerminalInfo["WHNP"][2][2]="ADJP";
 			 			nonTerminalInfo["WHNP"][2][3]="PRN";
 			 			nonTerminalInfo["WHNP"][2][4]="FW";
 			 			nonTerminalInfo["WHNP"][3]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][3][0]="right";
 			 			nonTerminalInfo["WHNP"][3][1]="CD";
 			 			nonTerminalInfo["WHNP"][4]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][4][0]="rightdis";
 			 			nonTerminalInfo["WHNP"][4][1]="JJ";
 			 			nonTerminalInfo["WHNP"][4][2]="JJS";
 			 			nonTerminalInfo["WHNP"][4][3]="RB";
 			 			nonTerminalInfo["WHNP"][4][4]="QP";
 			 			nonTerminalInfo["WHNP"][5]=new std::string[5];
 			 			nonTerminalInfo["WHNP"][5][0]="left";
 			 			nonTerminalInfo["WHNP"][5][1]="WHPP";
 			 			nonTerminalInfo["WHNP"][5][2]="WHADJP";
 			 			nonTerminalInfo["WHNP"][5][3]="WP$";
 			 			nonTerminalInfo["WHNP"][5][4]="WDT";
 
 			 			
 			 			
 			//nonTerminalInfo.put("WHPP", new String[][]{{"right", "IN", "TO", "FW"}});
 			nonTerminalInfo["WHPP"]=new std::string*[1];
 			nonTerminalInfo["WHPP"][0]=new std::string[4];
 			nonTerminalInfo["WHPP"][0][0]="right";
 			nonTerminalInfo["WHPP"][0][1]="IN";
 			nonTerminalInfo["WHPP"][0][2]="TO";
 			nonTerminalInfo["WHPP"][0][3]="FW";
 			
 			//nonTerminalInfo.put("X", new String[][]{{"right", "S", "VP", "ADJP", "JJP", "NP", "SBAR", "PP", "X"}});
 			nonTerminalInfo["X"]=new std::string*[1];
 			nonTerminalInfo["X"][0]=new std::string[9];
 			nonTerminalInfo["X"][0][0]="right";
 			nonTerminalInfo["X"][0][1]="S";
 			nonTerminalInfo["X"][0][2]="VP";
 			nonTerminalInfo["X"][0][3]="ADJP";
 			nonTerminalInfo["X"][0][4]="JJP";
 			nonTerminalInfo["X"][0][5]="NP";
 			nonTerminalInfo["X"][0][6]="SBAR";
 			nonTerminalInfo["X"][0][7]="PP";
 			nonTerminalInfo["X"][0][8]="X";
 			
 			
 			//nonTerminalInfo.put("NP", new String[][]{{"rightdis", "NN", "NNP", "NNPS", "NNS", "NML", "NX", "POS", "JJR"}, {"left", "NP", "PRP"}, {"rightdis", "$", "ADJP", "JJP", "PRN", "FW"}, {"right", "CD"}, {"rightdis", "JJ", "JJS", "RB", "QP", "DT", "WDT", "RBR", "ADVP"}});
 			/*nonTerminalInfo["NP"]=new std::string*[5];
 			nonTerminalInfo["NP"][0]=new std::string[10];
 			nonTerminalInfo["NP"][0][0]="rightdis";
 			nonTerminalInfo["NP"][0][1]="NN";
 			nonTerminalInfo["NP"][0][2]="NNP";
 			nonTerminalInfo["NP"][0][3]="NNPS";
 			nonTerminalInfo["NP"][0][4]="NNS";
 			nonTerminalInfo["NP"][0][5]="NML";
 			nonTerminalInfo["NP"][0][6]="NX";
 			nonTerminalInfo["NP"][0][7]="NML";
 			nonTerminalInfo["NP"][0][8]="POS";
 			nonTerminalInfo["NP"][0][9]="JJR";
 			nonTerminalInfo["NP"][1]=new std::string[3];
 			nonTerminalInfo["NP"][1][0]="left";
 			nonTerminalInfo["NP"][1][1]="NP";
 			nonTerminalInfo["NP"][1][2]="PRP";
 			nonTerminalInfo["NP"][2]=new std::string[6];
 			nonTerminalInfo["NP"][2][0]="rightdis";
 			nonTerminalInfo["NP"][2][1]="$";
 			nonTerminalInfo["NP"][2][2]="ADJP";
 			nonTerminalInfo["NP"][2][3]="JJP";
 			nonTerminalInfo["NP"][2][4]="PRN";
 			nonTerminalInfo["NP"][2][5]="FW";
 			nonTerminalInfo["NP"][3]=new std::string[2];
 			nonTerminalInfo["NP"][3][0]="right";
 			nonTerminalInfo["NP"][3][1]="CD";
 			nonTerminalInfo["NP"][4]=new std::string[9];
 			nonTerminalInfo["NP"][4][0]="rightdis";
 			nonTerminalInfo["NP"][4][1]="JJ";			
 			nonTerminalInfo["NP"][4][2]="JJS";
 			nonTerminalInfo["NP"][4][3]="RB";
 			nonTerminalInfo["NP"][4][4]="QP";
 			nonTerminalInfo["NP"][4][5]="DT";
 			nonTerminalInfo["NP"][4][6]="WDT";
 			nonTerminalInfo["NP"][4][7]="RBR";
 			nonTerminalInfo["NP"][4][8]="ADVP";*/
 			
 			//From void ruleChanges() //makes modifications to better fit with semantic notions of heads
 					nonTerminalInfo["NP"]=new std::string*[6];
 			 		nonTerminalInfo["NP"][0]=new std::string[8];
 			 		nonTerminalInfo["NP"][0][0]="rightdis";
 			 		nonTerminalInfo["NP"][0][1]="NN";
 			 		nonTerminalInfo["NP"][0][2]="NNP";
 			 		nonTerminalInfo["NP"][0][3]="NNPS";
 			 		nonTerminalInfo["NP"][0][4]="NNS";
 			 		nonTerminalInfo["NP"][0][5]="NML";
 			 		nonTerminalInfo["NP"][0][6]="NX";
 			 		nonTerminalInfo["NP"][0][7]="NML";
 			 		//nonTerminalInfo["NP"][0][8]="POS";
 			 		nonTerminalInfo["NP"][0][8]="JJR";
 			 		nonTerminalInfo["NP"][1]=new std::string[3];
 			 		nonTerminalInfo["NP"][1][0]="left";
 			 		nonTerminalInfo["NP"][1][1]="NP";
 			 		nonTerminalInfo["NP"][1][2]="PRP";
 			 		nonTerminalInfo["NP"][2]=new std::string[4];
 			 		nonTerminalInfo["NP"][2][0]="rightdis";
 			 		nonTerminalInfo["NP"][2][1]="$";
 			 		nonTerminalInfo["NP"][2][2]="ADJP";
 			 		/*nonTerminalInfo["NP"][2][3]="JJP";
 			 		nonTerminalInfo["NP"][2][4]="PRN";*/
 			 		nonTerminalInfo["NP"][2][3]="FW";
 			 		nonTerminalInfo["NP"][3]=new std::string[2];
 			 		nonTerminalInfo["NP"][3][0]="right";
 			 		nonTerminalInfo["NP"][3][1]="CD";
 			 		nonTerminalInfo["NP"][4]=new std::string[11];
 			 		nonTerminalInfo["NP"][4][0]="rightdis";
 			 		nonTerminalInfo["NP"][4][1]="JJ";			
 			 		nonTerminalInfo["NP"][4][2]="JJS";
 			 		//nonTerminalInfo["NP"][4][3]="RB";
 			 		//nonTerminalInfo["NP"][4][4]="QP";
 			 		//nonTerminalInfo["NP"][4][5]="DT";
 			 		//nonTerminalInfo["NP"][4][6]="WDT";
 			 		//nonTerminalInfo["NP"][4][7]="RBR";
 			 		//nonTerminalInfo["NP"][4][8]="ADVP";
 			 		nonTerminalInfo["NP"][4][3]="QP";
 			 		nonTerminalInfo["NP"][4][4]="DT";
 			 		nonTerminalInfo["NP"][4][5]="WDT";
 			 		nonTerminalInfo["NP"][4][6]="NML";
 			 		nonTerminalInfo["NP"][4][7]="PRN";
 			 		nonTerminalInfo["NP"][4][8]="RB";
 			 		nonTerminalInfo["NP"][4][9]="RBR";
 			 		nonTerminalInfo["NP"][4][10]="ADVP";
 			 		nonTerminalInfo["NP"][5]=new std::string[2];
 			 		nonTerminalInfo["NP"][5][0]="left";
 			 		nonTerminalInfo["NP"][5][1]="POS";
 			
 			
 			// TODO: seems JJ should be head of NML in this case:
		    // (NP (NML (JJ former) (NML Red Sox) (JJ great)) (NNP Luis) (NNP Tiant)),
 		    //nonTerminalInfo.put("NML", new String[][]{{"rightdis", "NN", "NNP", "NNPS", "NNS", "NX", "NML", "POS", "JJR"}, {"left", "NP", "PRP"}, {"rightdis", "$", "ADJP", "JJP", "PRN"}, {"right", "CD"}, {"rightdis", "JJ", "JJS", "RB", "QP", "DT", "WDT", "RBR", "ADVP"}}); 			
 			nonTerminalInfo["NML"]=new std::string*[5];
 			nonTerminalInfo["NML"][0]=new std::string[9];
 			nonTerminalInfo["NML"][0][0]="rightdis";
 			nonTerminalInfo["NML"][0][1]="NN";
 			nonTerminalInfo["NML"][0][2]="NNP";
 			nonTerminalInfo["NML"][0][3]="NNPS";
 			nonTerminalInfo["NML"][0][4]="NNS";
 			nonTerminalInfo["NML"][0][5]="NX";
 			nonTerminalInfo["NML"][0][6]="NML";
 			nonTerminalInfo["NML"][0][7]="POS";
 			nonTerminalInfo["NML"][0][8]="JJR";
 			nonTerminalInfo["NML"][1]=new std::string[7];
 			nonTerminalInfo["NML"][1][0]="left";
 			nonTerminalInfo["NML"][1][1]="NP";
 			nonTerminalInfo["NML"][1][2]="PRP";
 			nonTerminalInfo["NML"][2]=new std::string[5];
 			nonTerminalInfo["NML"][2][0]="rightdis";
 			nonTerminalInfo["NML"][2][1]="$";
 			nonTerminalInfo["NML"][2][2]="ADJP";
 			nonTerminalInfo["NML"][2][3]="JJP";
 			nonTerminalInfo["NML"][2][4]="PRN";
 			nonTerminalInfo["NML"][3]=new std::string[2];
 			nonTerminalInfo["NML"][3][0]="right";
 			nonTerminalInfo["NML"][3][1]="CD";
 			nonTerminalInfo["NML"][4]=new std::string[9];
 			nonTerminalInfo["NML"][4][0]="rightdis";
 			nonTerminalInfo["NML"][4][1]="JJ";			
 			nonTerminalInfo["NML"][4][2]="JJS";
 			nonTerminalInfo["NML"][4][3]="RB";
 			nonTerminalInfo["NML"][4][4]="QP";
 			nonTerminalInfo["NML"][4][5]="DT";
 			nonTerminalInfo["NML"][4][6]="WDT";
 			nonTerminalInfo["NML"][4][7]="RBR";
 			nonTerminalInfo["NML"][4][8]="ADVP";
 			
			//nonTerminalInfo.put("POSSP", new String[][]{{"right", "POS"}});
 			nonTerminalInfo["POSSP"]=new std::string*[1];
 			nonTerminalInfo["POSSP"][0]=new std::string[2];
 			nonTerminalInfo["POSSP"][0][0]="right";
 			nonTerminalInfo["POSSP"][0][1]="POS";
 			
 			/* HJT: Adding the following to deal with oddly formed data in (for example) the Brown corpus COMMENT IN STF CODE*/
 			/* nonTerminalInfo.put("ROOT", new String[][]{{"left", "S", "SQ", "SINV", "SBAR", "FRAG"}});*/
 			
 			nonTerminalInfo["ROOT"]=new std::string*[1];
 			nonTerminalInfo["ROOT"][0]=new std::string[2];
 			nonTerminalInfo["ROOT"][0][0]="left";
 			nonTerminalInfo["ROOT"][0][1]="S";
 			nonTerminalInfo["ROOT"][0][2]="SQ";
 			nonTerminalInfo["ROOT"][0][3]="SINV";
 			nonTerminalInfo["ROOT"][0][4]="SBAR";
 			nonTerminalInfo["ROOT"][0][5]="FRAG";
 			

			//nonTerminalInfo.put("TYPO", new String[][]{{"left", "NN", "NP", "NML", "NNP", "NNPS", "TO",
			 //     "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "ADJP", "JJP", "FRAG"}}); // for Brown (Roger) COMMENT in STF CODE

 			nonTerminalInfo["TYPO"]=new std::string*[1];
 			nonTerminalInfo["TYPO"][0]=new std::string[2];
 			nonTerminalInfo["TYPO"][0][0]="left";
 			nonTerminalInfo["TYPO"][0][1]="NN";
 			nonTerminalInfo["TYPO"][0][2]="NP";
 			nonTerminalInfo["TYPO"][0][3]="NML";
 			nonTerminalInfo["TYPO"][0][4]="NNP";
 			nonTerminalInfo["TYPO"][0][5]="NNPS";
 			nonTerminalInfo["TYPO"][0][6]="TO";
 			nonTerminalInfo["TYPO"][0][7]="VBD";
 			nonTerminalInfo["TYPO"][0][8]="VBN";
 			nonTerminalInfo["TYPO"][0][9]="MD";
 			nonTerminalInfo["TYPO"][0][11]="VBZ";
 			nonTerminalInfo["TYPO"][0][12]="VB";
 			nonTerminalInfo["TYPO"][0][13]="VBG";
 			nonTerminalInfo["TYPO"][0][14]="VBP";
 			nonTerminalInfo["TYPO"][0][15]="VP";
 			nonTerminalInfo["TYPO"][0][16]="ADJP";
 			nonTerminalInfo["TYPO"][0][17]="JJP";
 			nonTerminalInfo["TYPO"][0][18]="FRAG";
 			
 			//nonTerminalInfo.put("ADV", new String[][]{{"right", "RB", "RBR", "RBS", "FW",
		      //"ADVP", "TO", "CD", "JJR", "JJ", "IN", "NP", "NML", "JJS", "NN"}});
 			nonTerminalInfo["ADV"]=new std::string*[1];
 			nonTerminalInfo["ADV"][0]=new std::string[15];
 			nonTerminalInfo["ADV"][0][0]="right";
 			nonTerminalInfo["ADV"][0][1]="RB";
 			nonTerminalInfo["ADV"][0][2]="RBR";
 			nonTerminalInfo["ADV"][0][3]="RBS";
 			nonTerminalInfo["ADV"][0][4]="FW";
 			nonTerminalInfo["ADV"][0][5]="ADVP";
 			nonTerminalInfo["ADV"][0][6]="TO";
 			nonTerminalInfo["ADV"][0][7]="CD";
 			nonTerminalInfo["ADV"][0][8]="JJR";
 			nonTerminalInfo["ADV"][0][9]="JJ";
 			nonTerminalInfo["ADV"][0][10]="IN";
 			nonTerminalInfo["ADV"][0][11]="NP";
 			nonTerminalInfo["ADV"][0][12]="NML";
 			nonTerminalInfo["ADV"][0][13]="JJS";
 			nonTerminalInfo["ADV"][0][14]="NN";
 			
 		   // SWBD
 		   //nonTerminalInfo.put("EDITED", new String[][] {{"left"}});  // crap rule for Switchboard (if don't delete EDITED nodes) Comment in stf code
 			nonTerminalInfo["EDITED"]=new std::string*[1];
 			nonTerminalInfo["EDITED"][0]=new std::string[1];
 			nonTerminalInfo["EDITED"][0][0]="left";
 			
 			
 			// in sw2756, a "VB". (copy "VP" to handle this problem, though should really fix it on reading)
		    //nonTerminalInfo.put("VB", new String[][]{{"left", "TO", "VBD", "VBN", "MD", "VBZ", "VB", "VBG", "VBP", "VP", "AUX", "AUXG", "ADJP", "JJP", "NN", "NNS", "JJ", "NP", "NNP"}});
 			nonTerminalInfo["VB"]=new std::string*[1];
 			nonTerminalInfo["VB"][0]=new std::string[19];
 			nonTerminalInfo["VB"][0][0]="left";
 			nonTerminalInfo["VB"][0][1]="TO";
 			nonTerminalInfo["VB"][0][2]="VBD";
 			nonTerminalInfo["VB"][0][3]="VBN";
 			nonTerminalInfo["VB"][0][4]="MD";
 			nonTerminalInfo["VB"][0][5]="VBZ";
 			nonTerminalInfo["VB"][0][6]="VB";
 			nonTerminalInfo["VB"][0][7]="VBG";
 			nonTerminalInfo["VB"][0][8]="VBP";
 			nonTerminalInfo["VB"][0][9]="VP";
 			nonTerminalInfo["VB"][0][10]="AUX";
 			nonTerminalInfo["VB"][0][11]="AUXG";
 			nonTerminalInfo["VB"][0][12]="ADJP";
 			nonTerminalInfo["VB"][0][13]="JJP";
 			nonTerminalInfo["VB"][0][14]="NN";
 			nonTerminalInfo["VB"][0][15]="NNS";
 			nonTerminalInfo["VB"][0][16]="JJ";
 			nonTerminalInfo["VB"][0][17]="NP";
 			nonTerminalInfo["VB"][0][18]="NNP";
 			
 			//nonTerminalInfo.put("META", new String[][] {{"left"}});  // rule for OntoNotes, but maybe should just be deleted in TreeReader?? //Comment in STF CODE
 			
 			nonTerminalInfo["META"]=new std::string*[1];
 			nonTerminalInfo["META"][0]=new std::string[1];
 			nonTerminalInfo["META"][0][0]="left";
 			
 			//nonTerminalInfo.put("XS", new String[][] {{"right", "IN"}});*/ // rule for new structure in QP, introduced by Stanford in QPTreeTransformer
 			/*nonTerminalInfo["XS"]=new std::string*[1];
 			nonTerminalInfo["XS"][0]=new std::string[2];
 			nonTerminalInfo["XS"][0][0]="right";
 			nonTerminalInfo["XS"][0][1]="IN";*/
 			//It didn't change in the ruleChanges() method, but still it was there
 			nonTerminalInfo["XS"]=new std::string*[1];
 			nonTerminalInfo["XS"][0]=new std::string[2];
 			nonTerminalInfo["XS"][0][0]="right";
 			nonTerminalInfo["XS"][0][1]="IN";
 			
 			nonTerminalInfo["EMBED"]=new std::string*[1];
 			nonTerminalInfo["EMBED"][0]=new std::string[2];
 			nonTerminalInfo["EMBED"][0][0]="right";
 			nonTerminalInfo["EMBED"][0][1]="INTJ";
 			
 			//Comments in stf code
 			// nonTerminalInfo.put(null, new String[][] {{"left"}});  // rule for OntoNotes from Michel, but it would be better to fix this in TreeReader or to use a default rule? 
 			// todo: Uncomment this line if we always want to take the leftmost if no head rule is defined for the mother category.
 			// defaultRule = defaultLeftRule; // Don't exception, take leftmost if no rule defined for a certain parent category
 			
 			
 			
 			
 			
 		}
 		

 		
 	public:
 		//Methods from semantic head finder
 		
 		/*
 		Tree determineNonTrivialHead(Tree t, Tree parent) {
 			...
 		}
 		*/
 		
 		/*int postOperationFix(int headIdz, Tree[] daughterTrees) {
 			
 		}*/
 		
 		/*boolean isExistential(Tree t, Tree parent) {
 			
 		}*/
 		/*boolean isWhQ(Tree t, Tree parent) {
 			
 		}*/
 		/*boolean hasPassiveProgressiveAuxiliary(Tree[] kids, Set<String> verbalSet--> String* verbalSet) {
 		 			
 		}*/
 		
 		/*boolean vpContainsParticiple(Tree t) {
 		  
 		 }
 		*/
 		
 		/*boolean isVerbalAuxiliary(Tree t) {
 			
 		}*/
    
 		/*boolean hasVerbalAuxiliary(Tree[] kids, Set<String> verbalSet) {
 			
 		}*/

 };
 







/*===============================================================
 *
 * CStateItem - the search state item, representing a partial
 *              candidate with shift reduce. 
 *
 *==============================================================*/

class CContext;

class CStateItem {
public:
   SCORE_TYPE score;
   CStateNode node; //head of the stack (top)
   const CStateItem *statePtr; //points to the state item before the action is applied, (chain of actions)
   const CStateItem *stackPtr; //tail of the stack
   int current_word;
   CAction action;
#ifdef TRAIN_LOSS
   CStack<CLabeledBracket> gold_lb;
   unsigned correct_lb;
   unsigned plost_lb;
   unsigned rlost_lb;
   bool bTrain;
#endif
#ifdef SCALE
   unsigned size;
#endif
   
public:
#ifdef TRAIN_LOSS
#define LOSS_CON , correct_lb(0), plost_lb(0), rlost_lb(0), bTrain(false)
#else
#define LOSS_CON 
#endif
#ifdef SCALE
#define SCALE_CON , size(0)
#else
#define SCALE_CON 
#endif
   CStateItem() : current_word(0), score(0), action(), stackPtr(0), statePtr(0), node() LOSS_CON SCALE_CON {}
   virtual ~CStateItem() {}
public:
   void clear() {
      statePtr = 0;
      stackPtr = 0;
      current_word = 0;
      node.clear();
      score = 0;
      action.clear();
#ifdef TRAIN_LOSS
      gold_lb.clear();
      correct_lb=0;
      plost_lb=0;
      rlost_lb=0;
      bTrain = false;
#endif
#ifdef SCALE
      size = 0;
#endif
   }
   bool empty() const {
      if (current_word==0) {
         assert(stackPtr == 0 && statePtr == 0 && score==0);
         return true;
      }
      return false;
   }
   unsigned stacksize() const {
      unsigned retval = 0;
      const CStateItem *current = this;
      while (current) {
         if (current->node.valid()) ++retval;// no node -> start/fini
         current = current->stackPtr;
      }
      return retval;
   }
   unsigned unaryreduces() const {
      unsigned retval = 0;
      const CStateItem *current = this;
      while (current) {
         if (current->action.type() == CActionType::REDUCE_UNARY)
            ++retval;
         else
            return retval;
         current = current->statePtr;
      }
      return retval;
   }
   int newNodeIndex() const { return node.id+1; }

public:
   bool operator < (const CStateItem &st) const { return score < st.score; }
   bool operator > (const CStateItem &st) const { return score > st.score; }
   bool operator == (const CStateItem &st) const {
      THROW("State items are not comparable with each other");
   }
   bool operator != (const CStateItem &st) const { return !( (*this) == st ); }

protected:
   // now this actions MUST BE called by Move
   void shift(CStateItem *retval, const unsigned long &constituent = CConstituent::NONE) const {
      //TRACE("shift");
      assert(!IsTerminated());
      retval->node.set(node.id+1, CStateNode::LEAF, false, constituent, 0, 0, current_word, current_word, current_word);
      retval->current_word = current_word+1;
      retval->stackPtr = this; ///  
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeShiftLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
      assert(!retval->IsTerminated());
   }
   void reduce(CStateItem *retval, const unsigned long &constituent, const bool &single_child, const bool &head_left, const bool &temporary) const {
      //TRACE("reduce");
      assert(!IsTerminated());
      const static CStateNode *l, *r;
      assert(stackPtr!=0);
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         l = &node;
         retval->node.set(node.id+1, CStateNode::SINGLE_CHILD, false, constituent, l, 0, l->lexical_head, l->lexical_start, l->lexical_end);
         retval->stackPtr = stackPtr;
#ifdef TRAIN_LOSS
         retval->bTrain = this->bTrain;
         computeReduceUnaryLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb, constituent);
#endif
      }
      else {

         static unsigned long fullconst; 
         assert(stacksize()>=2);
         r = &node;
         l = &(stackPtr->node);
#ifdef NO_TEMP_CONSTITUENT
         fullconst = constituent;
#else
         fullconst = CConstituent::encodeTmp(constituent, temporary);
#endif
         retval->node.set(node.id+1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, fullconst, l, r, (head_left?l->lexical_head:r->lexical_head), l->lexical_start, r->lexical_end);
        

         retval->stackPtr = stackPtr->stackPtr;
#ifdef TRAIN_LOSS
         retval->bTrain = this->bTrain;
         computeReduceBinaryLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb, fullconst);
#endif
	//SOMEWHERE in this else block we MAKE THE stanford LINKS (Miguel)
	 //By using: R and L. Which are the right child and left child. 
	 //R and L are state nodes. So, the idea is to make links between them. And store them in CLink* stanfordLinks; //miguel
	 // Take into account that we process it in Inorder (or left order)
	
         //retval->node.generateStanford(); //here we call the method that generates the stanford dependencies which is in CStateNode
         retval->node.generateStanford(false,false,false,false); //not collapsed
         retval->node.generateStanford(true,false,false,false); //collapsed
    

      }
      retval->current_word = current_word;

      assert(!IsTerminated());
   }

 



   void terminate(CStateItem *retval) const {
      //TRACE("terminate");
//      assert(IsComplete());
      assert(!IsTerminated());
      retval->node = this->node;
      retval->stackPtr=this->stackPtr;
      retval->current_word = current_word;
      // compute loss
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeTerminateLoss(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
      assert(retval->IsTerminated());
   }
   void noact(CStateItem *retval) const {
      //TRACE("noact");
      assert(IsTerminated());
      retval->node = this->node;
      retval->stackPtr=this->stackPtr;
      retval->current_word = current_word;
      // compute loss
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeIdleLoss(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
   }

protected:

#ifdef TRAIN_LOSS
   void computeShiftLB( CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      // compute shift
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      static CStack< CLabeledBracket >::const_iterator it;
      it = gold_lb.begin();
      while ( it != gold_lb.end() ) {
         if ( node.valid() && (*it).end == node.lexical_end ) {
            ++ (rlost);
         }
         else {
            if (gold) gold->push( *it );
         }
         ++it;
      } // while
   }
   
   void computeReduceUnaryLB(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost, const unsigned long &constituent) const {
      static CStack< CLabeledBracket >::const_iterator it;
      static bool bCorrect;
      if (gold) gold->clear();
      plost = plost_lb;
      rlost = rlost_lb;
      correct = correct_lb;
      if (!bTrain) return;
      // loop
      it = gold_lb.begin();
      bCorrect = false;
      while ( it != gold_lb.end() ) {
         if ( (*it).begin == node.lexical_start &&
              (*it).end == node.lexical_end &&
              (*it).constituent == constituent ) {
            bCorrect = true;
            ++correct;
         }
         else {
            if (gold) gold->push(*it);
         }
         ++it;
      } //while
      if (!bCorrect)
         ++(plost);
   }

   void computeReduceBinaryLB(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost, const unsigned long &constituent) const {
      const static CStateNode *l, *r;
      static CStack< CLabeledBracket >::const_iterator it;
      static bool bCorrect;
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      r = &node;
      l = &(stackPtr->node);
      // loop
      bCorrect = false;
      it=gold_lb.begin();
      while ( it != gold_lb.end() ) {
         if ( (*it).begin == l->lexical_start &&
              (*it).end == r->lexical_end &&
              (*it).constituent == constituent ) {
            bCorrect=true;
            ++correct;
         }
         else if ( (*it).begin == r->lexical_start ) {
            ++(rlost);
         }
         else {
            if (gold) gold->push(*it);
         }
         ++it;
      } // while
      if (!bCorrect)
         ++(plost);
   }
   void computeTerminateLoss(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      static CStack< CLabeledBracket >::const_iterator it;
      it = gold_lb.begin();
      while ( it != gold_lb.end() ) {
         assert( (*it).begin == node.lexical_start && (*it).end == node.lexical_end);
         ++(rlost);
         ++it;
      }//while
   }
   void computeIdleLoss(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
   }
#endif

public:

   void NextMove(const CCFGTree &snt, CAction &retval) const {
      int s = node.id;
      const CCFGTreeNode &nd = snt.nodes[s];
      const CCFGTreeNode &hd = snt.nodes[snt.parent(s)];
      assert(!hd.constituent.empty()); // so that reduce and reduce_root are not same
      bool single_child;
      bool head_left;
      bool temporary;
      // stack top single child ? reduce unary
      if (hd.single_child) {
         single_child = true;
         head_left = false; assert(hd.head_left==false);
         temporary = false; assert(hd.temp==false);
      }
      else {
         // stack top left child ? shift
         if (s == hd.left_child) {
            retval.encodeShift(snt.nodes[newNodeIndex()].constituent.code()); return;
         }
         // stack top right child ? reduce bin
         assert(s==hd.right_child);
         single_child = false;
         head_left = hd.head_left;
         temporary = hd.temp;
#ifdef NO_TEMP_CONSTITUENT
         ASSERT(!temporary, "This version does not accept temporary constituents, but the training data give them.");
#endif
      }
      retval.encodeReduce(hd.constituent.code(), single_child, head_left, temporary);
   }

   void StandardMove(const CCFGTree &tr, CAction &retval) const {
      if (IsTerminated()) {
         retval.encodeIdle();
         return;
      }
      // stack empty?shift
      if (stacksize()==0) {
         retval.encodeShift(tr.nodes[newNodeIndex()].constituent.code());
         return;
      }
      if (tr.parent(node.id) == -1) {
         assert(IsComplete(tr.words.size()));
         retval.encodeReduceRoot();
         return;
      }
      NextMove(tr, retval);
   }

   void Move(CStateItem *retval, const CAction &action) const {
      retval->action = action; // this makes it necessary for the actions to 
      retval->statePtr = this; // be called by Move
      if (action.isIdle()) {
         noact(retval);
#ifdef SCALE
         retval->size = this->size;
#endif
      }
      else {
         if (action.isShift())
            { shift(retval, action.getConstituent()); }
         else if (action.isReduceRoot())
            { terminate(retval); }
         else
            { reduce(retval, action.getConstituent(), action.singleChild(), action.headLeft(), action.isTemporary()); }
#ifdef SCALE
         retval->size = this->size + 1;
#endif
      }
   }
   
   bool IsComplete(const int &nWords) const {
#ifdef FRAGMENTED_TREE
      return current_word == nWords; // allow multiple-rt.
#else
      return current_word == nWords && stacksize() == 1;
#endif
   }

   bool IsTerminated() const {
      return action.type() == CActionType::POP_ROOT or action.type() == CActionType::IDLE; 
   }

   bool IsIdle() const {
      return action.type() == CActionType::IDLE; 
   }

   void GenerateTree(const CTwoStringVector &tagged, CSentenceParsed &out) const {
      // parsing done?
      assert(IsTerminated());
//      assert(tagged.size()==sent->size());
      out.clear();
#ifdef FRAGMENTED_TREE
      if (stacksize()>1) {
         static const CStateItem *item;
         item = statePtr;
         assert(item==stackPtr);
         static CStateItem *tmp;
         tmp = new CStateItem[stacksize()];
         static CStateItem *current;
         current = tmp;
         static CAction action;
         action.encodeReduce(CConstituent::NONE, false, false, false);
         while (item->stacksize()>1) {
            // form NONE nodes
            item->Move(current, action); 
            item = current;
            ++ current;
         }
         action.encodeReduceRoot();
         item->Move(current, action);
         item = current;
         item->GenerateTree(tagged, out);
         delete [] tmp;
         return;
      }
#else
      if (stacksize()>1) { WARNING("Parser failed.");return; }
#endif
      // generate nodes for out
      static int i,j;
      // first words
      for (i=0; i<tagged.size(); ++i) 
         out.newWord(tagged[i].first, tagged[i].second);
      // second constituents
      static const CStateNode* nodes[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
      static int count;
      count = 0;
      const static CStateItem *current;
      current = this;
      while (current) {
         if (!current->IsTerminated() && current->node.valid()) {
            nodes[count] = &current->node;
            ++count; 
         }
         current = current->statePtr;
      }

      for (i=count-1; i>=0; --i) {
         j = out.newNode();
         // copy node
         assert(j==nodes[i]->id);
         nodes[i]->toCCFGTreeNode(out.nodes[j]);
      }
      out.root = nodes[0]->id;
   }

  //(Miguel) New method for algo generate the STANFORD links
  //(Miguel) It is just an output process, the hard core thing is in the reduce method.
   void GenerateTreeAndLinks(const CTwoStringVector &tagged, CSentenceParsed &out, std::string &stanfordOut) const {
      // parsing done?
      assert(IsTerminated());
//      assert(tagged.size()==sent->size());
      out.clear();
#ifdef FRAGMENTED_TREE
      if (stacksize()>1) {
         static const CStateItem *item;
         item = statePtr;
         assert(item==stackPtr);
         static CStateItem *tmp;
         tmp = new CStateItem[stacksize()];
         static CStateItem *current;
         current = tmp;
         static CAction action;
         action.encodeReduce(CConstituent::NONE, false, false, false);
         while (item->stacksize()>1) {
            // form NONE nodes
            item->Move(current, action); 
            item = current;
            ++ current;
         }
         action.encodeReduceRoot();
         item->Move(current, action);
         item = current;
         item->GenerateTreeAndLinks(tagged, out);
         delete [] tmp;
         return;
      }
#else
      if (stacksize()>1) { WARNING("Parser failed.");return; }
#endif
      // generate nodes for out
      static int i,j;
      // first words
      for (i=0; i<tagged.size(); ++i) 
         out.newWord(tagged[i].first, tagged[i].second);
      // second constituents
      static const CStateNode* nodes[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
      static int count;
      count = 0;
      const static CStateItem *current;
      current = this;
      while (current) {
         if (!current->IsTerminated() && current->node.valid()) {
            nodes[count] = &current->node;
            ++count; 
         }
         current = current->statePtr; //
	//stackptr: the tail of the stack.
      }

      for (i=count-1; i>=0; --i) { //here we get the sequence of stateitems. Left order traver
         j = out.newNode();
         // copy node
         assert(j==nodes[i]->id);
         nodes[i]->toCCFGTreeNode(out.nodes[j]);
	 //here we should read the stanford links associated to each node, since it will have been created in the reduce process. (Miguel)
         //It is just an output process, the hard core thing is in the reduce method (Miguel)
      }
      out.root = nodes[0]->id;
   }


   //===============================================================================

   void trace(const CTwoStringVector *s=0) const {
      static const CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
      static int count;
      const static CStateItem *current;
      count = 0;
      current = this;
      while (current->statePtr) {
         states[count] = current;
         ++count ; //updating
         current = current->statePtr;
      }
      #ifdef SCALE
      TRACE("State item score == " << score);
      TRACE("State item size == " << size);
      #endif
#ifdef TRAIN_LOSS
      TRACE("cor = " << correct_lb << ", plo = " << plost_lb << ", rlo = " << rlost_lb << ", Loss = " << FLoss());
#endif
      --count;
      while (count>=0) {
         if (s) {
            TRACE(states[count]->action.str()<<" ["<<(states[count]->stacksize()>0?s->at(states[count]->node.lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(states[count]->action.str());
         }
         --count;
      }
      TRACE("");
   }

   //===============================================================================
#ifdef TRAIN_LOSS   
public:
   SCORE_TYPE actionLoss(const CAction &action, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      static unsigned long constituent;
      if (action.isShift()) {
         computeShiftLB(0, correct, plost, rlost);
      }
      else if (action.isReduceRoot()) {
         computeTerminateLoss(0, correct, plost, rlost);
      }
      else if (action.isIdle()) {
         computeIdleLoss(0, correct, plost, rlost);
      }
      else if (action.singleChild()) {
         computeReduceUnaryLB(0, correct, plost, rlost, action.getConstituent());
      }
      else {
#ifdef NO_TEMP_CONSTITUENT
         constituent = action.getConstituent();
#else
         constituent = CConstituent::encodeTmp(action.getConstituent(), action.isTemporary());
#endif
         computeReduceBinaryLB(0, correct, plost, rlost, constituent);
      }
   }
   SCORE_TYPE actionFLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return FLoss(correct, plost, rlost);
   }
   SCORE_TYPE FLoss(const unsigned &correct, const unsigned &plost, const unsigned &rlost) const {
      static SCORE_TYPE p, r, f;
      if (correct == 0) {
         if (plost == 0 && rlost == 0) {
            return static_cast<SCORE_TYPE>(0);
         }
         else {
            return static_cast<SCORE_TYPE>(1);
         }
      }
      p = static_cast<SCORE_TYPE>(correct) / (correct + plost);
      r = static_cast<SCORE_TYPE>(correct) / (correct + rlost);
      f = 2*static_cast<SCORE_TYPE>(p)*r / (p + r);
      return 1-f;
   }
   SCORE_TYPE FLoss() const {
      return FLoss(correct_lb, plost_lb, rlost_lb);
   }
   SCORE_TYPE HammingLoss() const {
      return plost_lb + rlost_lb;
   }
   SCORE_TYPE stepHammingLoss() const {
      if (statePtr == 0)
         return 0;
      return plost_lb-statePtr->plost_lb + rlost_lb-statePtr->rlost_lb;
   }
   SCORE_TYPE actionHammingLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return plost + rlost;
   }
   SCORE_TYPE actionStepHammingLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return plost-plost_lb + rlost-rlost_lb;
   }
#endif
};

/*===============================================================
 *
 * scored actions
 *
 *==============================================================*/

class CScoredStateAction {

public:
   CAction action;
   const CStateItem *item;
   SCORE_TYPE score;

public:
   CScoredStateAction() : item(0), action(), score(0) {}
   void load(const CAction &action, const CStateItem *item, const SCORE_TYPE &score) {
      SCORE_TYPE item_sc;
      this->action = action; 
      this->item = item;
      item_sc = item->score;
#ifdef SCALE
      item_sc *= item->size;
#endif
#ifdef TRAIN_LOSS
#define LOSS_ADD + item->actionStepHammingLoss(action)
//#define LOSS_ADD -std::sqrt(item->HammingLoss()) + std::sqrt(item->actionHammingLoss(action))
#else
#define LOSS_ADD 
#endif
      this->score = item_sc + score LOSS_ADD;
#ifdef SCALE
      this->score /= (item->size + 1);
#endif
   }

public:
   bool operator < (const CScoredStateAction &a1) const { return score < a1.score; }
   bool operator > (const CScoredStateAction &a1) const { return score > a1.score; }
   bool operator <= (const CScoredStateAction &a1) const { return score <= a1.score; }
   bool operator >= (const CScoredStateAction &a1) const { return score >= a1.score; }

};

#endif

