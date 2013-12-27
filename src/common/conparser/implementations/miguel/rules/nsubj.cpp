  //===================================================================================================
    
   /*
    * Miguel: RULES FOR NSUBJ. There are 13

    * 
    * //S < (NP=target $+ NP|ADJP) > VP
    * 
    * Double-checked and seems fine. 11th July 13.
    */
inline const bool &buildNsubj1(const unsigned long &cons) {
	   if (cons==PENN_CON_VP) {
		   CStateNodeList* childsVP=node.m_umbinarizedSubNodes;
		   while(childsVP!=0) {
			   const CStateNode* sHead=childsVP->node;
			   if (CConstituent::clearTmp(sHead->constituent.code())==PENN_CON_S) {
				   CStateNodeList* childsS=sHead->m_umbinarizedSubNodes;
				   while(childsS!=0){
					   const CStateNode* npTarg=childsS->node;
					   //if (npTarg->constituent==PENN_CON_NP && !(isLinked(sHead,npTarg))) {
					   if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))) {
						   CStateNodeList* sistersRightNp=childsS;
						   if (sistersRightNp!=0) sistersRightNp=sistersRightNp->next;
						   if(sistersRightNp!=0) { //it is the immediately (just one)
							   const CStateNode* sister=sistersRightNp->node;
							   if (CConstituent::clearTmp(sister->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(sister->constituent.code())==PENN_CON_ADJP) { //FIRE RULE
//								   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
								   if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, sHead->lexical_head)){
									   //std::cout<<"nSubj1"<<" (head: "<<sHead->lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
									   //addLinked(sHead,npTarg);
									   addLinked(&node,npTarg);
									   return true;
								   }
							   }
						   }
					   }
					   childsS=childsS->next;
				   }
			   }
			   childsVP=childsVP->next;
		   }
	   }
	   return false;
	   
   }
   
   
   /*
   *SQ|PRN < (NP=target !< EX $++ VP)
   *
   * Double checked and seems fine.
   */
inline const bool &buildNsubj2(const unsigned long &cons) {
	   if (cons==PENN_CON_SQ || cons==PENN_CON_PRN) {
		   CStateNodeList* childs=node.m_umbinarizedSubNodes;
		   while(childs!=0) {
			   const CStateNode* npTarg=childs->node;
			   if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))) {
				   CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
				   bool noExChild=true;
				   while(childsNp!=0){
					   const CStateNode* noex=childsNp->node;
					   if ((*words)[noex->lexical_head].tag.code()==PENN_TAG_EX) {
						   noExChild=false;
					   }
					   childsNp=childsNp->next;
				   }
				   if (noExChild) {
					   CStateNodeList* sistersRightNp=childs;
					   if (sistersRightNp!=0) sistersRightNp=sistersRightNp->next;
					   while(sistersRightNp!=0) { //iterate through the righ sisters of NP
						   const CStateNode* sister=sistersRightNp->node;
						   if (CConstituent::clearTmp(sister->constituent.code())==PENN_CON_VP) { //FIRE RULE
//							   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
								if (buildStanfordLinkForDebug(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head,2)) {
								   
								   //std::cout<<"nSubj2"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
								   addLinked(&node,npTarg);
								   return true;
							   }
						   }
						   sistersRightNp=sistersRightNp->next;
					   }
					  
				   }
			   }
			   childs=childs->next;
		   }
	   }
	   return false;  	  
    }
   
   /*
   *"S < ((NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + "))) $++ VP)"
   * 
   * Checked and seems fine.
   */
inline const bool &buildNsubj3(const unsigned long &cons) {
	   if (cons==PENN_CON_S) {
		   //std::cout<<"S: "<<(*words)[node.lexical_head].word<<"\n";
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while(childsS!=0){
			   const CStateNode* npwhnpTarg=childsS->node;
			   if ((CConstituent::clearTmp(npwhnpTarg->constituent.code())==PENN_CON_NP ||CConstituent::clearTmp(npwhnpTarg->constituent.code())==PENN_CON_WHNP)&& !(isLinked(&node, npwhnpTarg))){
				   //std::cout<<"NP|WHNP: "<<(*words)[npwhnpTarg->lexical_head].word<<"\n";
				   bool vpSister=false;
				   CStateNodeList* rightSistersNp=childsS->next;
				   while(rightSistersNp!=0){
					   if (CConstituent::clearTmp(rightSistersNp->node->constituent.code())==PENN_CON_VP){
						   vpSister=true;
						   //std::cout<<"VP(sis): "<<(*words)[rightSistersNp->node->lexical_head].word<<"\n";
				   		}
				   		rightSistersNp=rightSistersNp->next;
				   	}
				   if (vpSister) { //NP|WHNP must have a VP sister.
					   CStateNodeList* childsNpwhnp=npwhnpTarg->m_umbinarizedSubNodes;
					   bool noEx=true;
					   bool noNNTime=true;
					   //NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + ")))
					   while(childsNpwhnp!=0){
						   const CStateNode* nnChildNp=childsNpwhnp->node;
						   //std::cout<<"childsNP: "<<(*words)[nnChildNp->lexical_head].word<<"\n";
						   if ((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_EX) {
							   noEx=false;
						   }
						   if ((nnChildNp->type==CStateNode::LEAF)
								   && (nnChildNp->lexical_head==npwhnpTarg->lexical_head) //<#
								   && (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
								   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
							   noNNTime=false;
						   }
						   childsNpwhnp=childsNpwhnp->next;
					   }
					   if (noEx && noNNTime){ //FIRE RULE
//						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
						   if (buildStanfordLink(STANFORD_DEP_NSUBJ, npwhnpTarg->lexical_head, node.lexical_head)) {
							   //std::cout<<"------\n";
							   addLinked(&node,npwhnpTarg);
							   //std::cout<<"nSubj3"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npwhnpTarg->lexical_head<<")\n";
						   	   return true;
						   	}     
					   }
				   }
			   }
			   childsS=childsS->next;
		   }
	   }
	   return false;
   }
   
  /*
   *"S < ( NP=target <# (/^NN/ < " + timeWordRegex + ") !$++ NP $++ VP)",
   * 
   * Checked and seems fine.
   */
inline const bool & buildNsubj4(const unsigned long &cons) {
	   if (cons==PENN_CON_S){
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while (childsS!=0){
			   const CStateNode* npTarg=childsS->node;
			   if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node, npTarg))) {
				   //$++ and !$++
				   bool noNpSister=true;
				   bool vpSister=false;
				   CStateNodeList* rightSistersNp=childsS;
				   while(rightSistersNp!=0){
					   if (CConstituent::clearTmp(rightSistersNp->node->constituent.code())==PENN_CON_NP){
						   noNpSister=false;
					   }
					   else if (CConstituent::clearTmp(rightSistersNp->node->constituent.code())==PENN_CON_VP){
						   vpSister=true;
					   }
					   rightSistersNp=rightSistersNp->next;
				   }
				   
				   if (noNpSister && vpSister) { //the first NP must have a VP as right sister and it must not have a NP as right sister.
					   CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
					   while(childsNp!=0){
						   const CStateNode* nnChildNp=childsNp->node;
						   
						   if ((nnChildNp->type==CStateNode::LEAF)
							   && (nnChildNp->lexical_head==npTarg->lexical_head) //<#
							   && (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
							   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) { //FIre rule
//							   		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
							   		if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head)) {
							   			addLinked(&node,npTarg);
							   			//std::cout<<"nSubj4"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
							   			return true;
							  }   
						   }
						   childsNp=childsNp->next;
					   }
				   }
			   }
			   childsS=childsS->next;
		   }
	   }
	   return false;
   }
    
   /*
    *"S < (NP < EX) <+(VP) (VP < NP=target)"
    *
    * Checked and seems fine. SEGURO? qué pasa si no hay unbroken chain
    */
inline const bool &buildNsubj5(const unsigned long &cons) {
	   if (cons==PENN_CON_S) {
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while (childsS!=0) {
			   const CStateNode* npChilds=childsS->node;
			   if (CConstituent::clearTmp(npChilds->constituent.code())==PENN_CON_NP) {
				   CStateNodeList* childsNp=npChilds->m_umbinarizedSubNodes;
				   //std::cout<<"2\n";
				   bool thereIsEx=false;
      			   while(childsNp!=0){
      				   const CStateNode* exChildnp=childsNp->node;
      				   //std::cout<<"before (lex_head):"<<exChildnp->lexical_head<<"\n";
      				   //std::cout<<"problem?:"<<(*words)[exChildnp->lexical_head]<<"\n";
                                   assert(words);
      				   if ((*words)[exChildnp->lexical_head].tag.code()==PENN_TAG_EX) {
      					   	  //std::cout<<"after (lex_head):"<<exChildnp->lexical_head<<"\n";
      					   	  thereIsEx=true;
      				   }
      				   childsNp=childsNp->next;
      			   }
      			   if (thereIsEx) {
      				   CStateNodeList* childsS2=node.m_umbinarizedSubNodes;
      				  //std::cout<<"3\n";
      				  while(childsS2!=0){
      					  const CStateNode* vpSonofNode=childsS2->node;
      					  if (CConstituent::clearTmp(vpSonofNode->constituent.code())==PENN_CON_VP) {
      						  //<+ (VP) (VP ...)
      						  CStateNodeList* vpsChain=new CStateNodeList();
      						  //std::cout<<"findingchain\n";
      						  findChain(PENN_CON_VP,PENN_CON_VP, vpSonofNode, vpsChain);
      						  if (vpsChain->node==0) {
      							  vpsChain->clear();
      							  vpsChain=0;
      						  }
      						  //std::cout<<"4\n";
      						  while(vpsChain!=0){
      							  CStateNodeList* childsOfAVp=vpsChain->node->m_umbinarizedSubNodes;
      							  //std::cout<<"5\n";
      							  while(childsOfAVp!=0){
      								  const CStateNode* npTarg=childsOfAVp->node;
      								  //if (npTarg->constituent==PENN_CON_NP && !(isLinked(vpsChain->node,npTarg))) {
      								  if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))) {
//      									  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
      									if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head)) {
      										  //addLinked(vpsChain->node,npTarg);
      										  addLinked(&node,npTarg);	
      										  //std::cout<<"nSubj5"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
      										  return true; 
      									  }
      								  }
      								  childsOfAVp=childsOfAVp->next;
      							  }
      							  vpsChain=vpsChain->next;
      							}
      					 }
      					 childsS2=childsS2->next;
      				  }
      			   } //if thereisex
      			} //if NP
      		 childsS=childsS->next;
      	   }//while childsS
      	}
      	return false;
    }
   
    /*
     *SQ < ((NP=target !< EX) $- /^(?:VB|AUX)/ !$++ VP)
     *
     *  Checked and seems fine.
     */
inline const bool &buildNsubj6(const unsigned long &cons) {
   		if (cons==PENN_CON_SQ) {
   			CStateNodeList* childsSq=node.m_umbinarizedSubNodes;
   			while (childsSq!=0){
   				const CStateNode* npTarg=childsSq->node;
   				if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))){
   					bool noEx=true;
   					CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
   					while(childsNp!=0) {
   						if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_EX) {
   							noEx=false;
   						}
   						childsNp=childsNp->next;
   					}
   					if (noEx){
   						bool noVpRightSister=true;
   						CStateNodeList* sistersNp=childsSq;
   						while(sistersNp!=0){
   							if (CConstituent::clearTmp(sistersNp->node->constituent.code())==PENN_CON_VP) {
   								noVpRightSister=false;
   							}
   							sistersNp=sistersNp->next;
   						}
   						if (noVpRightSister) {
   							sistersNp=childsSq;
   							if (sistersNp->previous!=0) {
   								const CStateNode* npLeftSister=sistersNp->previous->node;
   								if ((*words)[npLeftSister->lexical_head].tag.code()==PENN_TAG_VERB) { //for now, we leave AUX, as it is.
   									//std::cout<<(*words)[npLeftSister->lexical_head].word;
//   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   								    if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head)) {
   										addLinked(&node,npTarg);
   										//std::cout<<"nSubj6"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
   										return true;
   									}
   								}
   							}
   						}
   					}
   				}
   				childsSq=childsSq->next;
   			}
   		}
   		return false;
   	}
   	
   	
   	/*
   	*"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) ![$++ VP])",
    *"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) !$++ VP)", (they are equivalent)
    * 
    * Checked ans seems fine.
    */
inline const bool &buildNsubj7(const unsigned long &cons) {
 		if (cons==PENN_CON_SQ){
 			CStateNodeList* childsSQ=node.m_umbinarizedSubNodes;
 			while(childsSQ!=0){
 				const CStateNode* npTarg=childsSQ->node;
 				if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))){
 					bool noEx=true;
 					CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
 					while(childsNp!=0){
 						if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_EX) {
 							noEx=false;
 						}
 						childsNp=childsNp->next;
 					}
 					if (noEx){ //then keep going
 						//NP $- ()
 						if (childsSQ->previous!=0){ //NP $- RB 
 							if ((*words)[childsSQ->previous->node->lexical_head].tag.code()==PENN_TAG_ADVERB) { //RB==ADVERB?
 								if (childsSQ->previous->previous!=0){//RB $- /^.../
 									if ((*words)[childsSQ->previous->previous->node->lexical_head].tag.code()==PENN_TAG_VERB) { //VB? YUE
 										CStateNodeList* rightSisters=childsSQ;
 										bool noVp=true;
 										while(rightSisters!=0){
 											if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_VP){
 												noVp=false;
 											}
 											rightSisters=rightSisters->next;
 										}
 										if (noVp) { //fire rule
// 											CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
 											if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head)){
 												addLinked(&node,npTarg);
 												//std::cout<<"nSubj7"<<" (head: "<<node.lexical_head<<")"<<"(dependent:"<<npTarg->lexical_head<<")\n";
 												return true;
 											}
 										}
 									}
 								}
 							}
 						}
					}
				}
				childsSQ=childsSQ->next;
			}
		}
		return false;
	}
   
   
       /* 	
   	   *"SBARQ < WHNP=target < (SQ < (VP ![$-- NP]))",
   	   *"SBARQ < WHNP=target < (SQ < (VP !$-- NP))", I did it, presuming that it is like this. According to John Bauer, it is like this.
   	   * 
   	   * Checked and seems fine.
   	   */
inline const bool &buildNsubj8(const unsigned long &cons) {
    	  if (cons==PENN_CON_SBARQ){
    		  CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
    		  bool secondCondition=false;
    		  while(childsSbarq!=0){
    			  const CStateNode* sq=childsSbarq->node;
    			  if (CConstituent::clearTmp(sq->constituent.code())==PENN_CON_SQ) {
    				  CStateNodeList* childsSq=sq->m_umbinarizedSubNodes;
    				  while(childsSq!=0){
    					  if (CConstituent::clearTmp(childsSq->node->constituent.code())==PENN_CON_VP) {
    						  bool noSisterNp=true;
    						  CStateNodeList* leftSistersVp=childsSq;
    						  leftSistersVp=leftSistersVp->previous;
    						  while(leftSistersVp!=0){
    							  if (CConstituent::clearTmp(leftSistersVp->node->constituent.code())==PENN_CON_NP){
    								  noSisterNp=false;
    							  }
    							  leftSistersVp=leftSistersVp->previous;
    						  }
    						  if (noSisterNp) secondCondition=true;
    					  }
    					  childsSq=childsSq->next;
    				  }
    			  }
    			  childsSbarq=childsSbarq->next;
    		  }
    		  if (secondCondition) {
    			  childsSbarq=node.m_umbinarizedSubNodes;
    			  while(childsSbarq!=0){
    				  const CStateNode* whnpTarg=childsSbarq->node;
    				  if (CConstituent::clearTmp(whnpTarg->constituent.code())==PENN_CON_WHNP && !(isLinked(&node,whnpTarg))) {
//    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
    					  if (buildStanfordLink(STANFORD_DEP_NSUBJ, whnpTarg->lexical_head, node.lexical_head)){
    						   addLinked(&node,whnpTarg);
    						   //std::cout<<"nSubj8"<<" (head: "<<node.lexical_head<<")"<<"(dependent:"<<whnpTarg->lexical_head<<")\n";
    					   	   return true;
    					   }
    				  }
    				  childsSbarq=childsSbarq->next;
    			  }
    		  }
    	  }
    	  return false;
      }
      
       /*
        *"SBARQ < (SQ=target < /^(?:VB|AUX)/ !< VP)", 
        * 
        * Checked and seems fine.
   	    */
inline const bool &buildNsubj9(const unsigned long &cons) {
   		   if (cons==PENN_CON_SBARQ) {
   			   CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
   			   while(childsSbarq!=0){
   				   const CStateNode* sqTarg=childsSbarq->node;
   				   if (CConstituent::clearTmp(sqTarg->constituent.code())==PENN_CON_SQ && !(isLinked(&node,sqTarg))) {
   					   bool firstCondition=false;
   					   bool noVp=true;
   					   CStateNodeList* sonsOfSq=sqTarg->m_umbinarizedSubNodes;
   					   while(sonsOfSq!=0){
   						   if (CConstituent::clearTmp(sonsOfSq->node->constituent.code())==PENN_CON_VP) {
   							   noVp=false;
   						   }
   						   if ((*words)[sonsOfSq->node->lexical_head].tag.code()==PENN_TAG_VERB) {
   						   // || sonsOfSq->node->constituent==PENN_CON_AUX) { WE DON¡T KNOW WHAT IS AUX yet
   							   firstCondition=true;
   							   
   						   	}
   						   sonsOfSq=sonsOfSq->next;
   					   }
   					   if (noVp && firstCondition) {
//   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(STANFORD_DEP_NSUBJ, sqTarg->lexical_head, node.lexical_head)){
   							   addLinked(&node,sqTarg);
   							   //std::cout<<"nSubj9"<<" (head: "<<node.lexical_head<<")"<<"(sqTarg->lexical_head<<")\n";
   							   return true;
   						   }
   						   
   					   }
   				   }
   				   childsSbarq=childsSbarq->next;
   			   }
   		   }
   		   return false;
   	   }
   	   
   	   
   	   
   	   /*
   	   *"SINV < (NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )",
   	   *
   	   *(NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )
   	   *  //2.1 NP|WHNP $- VP... OR
   	   *   //2.2 NP|WHNP $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) OR
   	   *   //2.3 NP|WHNP !$- __ !$ @NP
   	   * 
   	   * 
   	   */
inline const bool &buildNsubj10(const unsigned long &cons) {
   		   if (cons==PENN_CON_SINV) {
   			   CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
   			   while(childsSinv!=0){
   				   const CStateNode* npwhnpTarg=childsSinv->node;
   				   
   				   if ((CConstituent::clearTmp(npwhnpTarg->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(npwhnpTarg->constituent.code())==PENN_CON_WHNP) && !isLinked(&node,npwhnpTarg)) {
   					   bool firstCondition=false;
   					   bool secondCondition=false;
   					   bool thirdCondition=false;
   					   
   					   if (childsSinv->previous!=0){
   						   const CStateNode* leftSisterNpWhnp=childsSinv->previous->node;
   						   if ((CConstituent::clearTmp(leftSisterNpWhnp->constituent.code())==PENN_CON_VP)
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_PAST) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_PRES) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_MD)) {
   								
   								   firstCondition=true;
   							   }
   							
   							if ((CConstituent::clearTmp(leftSisterNpWhnp->constituent.code())==PENN_CON_ADVP)
   								|| ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB)
   							    || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE) 
   							    || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE)) { 
   							    	
   							    	if (childsSinv->previous->previous!=0){
   							    		const CStateNode* leftSisterRbAdvp=childsSinv->previous->node;
   							    		if ((CConstituent::clearTmp(leftSisterRbAdvp->constituent.code())==PENN_CON_VP)
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_PAST) 
   							    		    ||((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_PRES) 
   							    		   	|| ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB) 
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_MD)) {
   							    		   		 secondCondition=true;
   							    		   		 
   							    		   	 }
   							    	}
   							  }
   					   }
   					   //third condition
   					   //NP|WHNP !$- __ !$ @NP
   					   bool firstPart=true;
   					   bool secondPart=true;
   					   //first part
   					   if (childsSinv->previous!=0){
   							   firstPart=false;
   					   }
   					   //second part
   					   CStateNodeList* sisters=childsSinv;
   					   sisters=sisters->next;
   					   while(sisters!=0){
   						   const CStateNode* atnp=sisters->node;
   						   if (CConstituent::clearTmp(atnp->constituent.code())==PENN_CON_NP){
   							   secondPart=false;
   						   }
   						   sisters=sisters->next;
   					   }
   					   sisters=childsSinv;
   					   sisters=sisters->previous;
   					   while(sisters!=0){
   						   const CStateNode* atnp=sisters->node;
   						   if (CConstituent::clearTmp(atnp->constituent.code())==PENN_CON_NP){
   							   secondPart=false;
   						   }
   						   sisters=sisters->previous;
   					   }
   					   
   					   if (firstPart && secondPart) {
   						   thirdCondition=true;
   					   }
   					   
   					   if (firstCondition || secondCondition || thirdCondition) {
//   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(STANFORD_DEP_NSUBJ, npwhnpTarg->lexical_head, node.lexical_head)){
   							   addLinked(&node,npwhnpTarg);
   							   //std::cout<<"nSubj10"<<" (head: "<<node.lexical_head<<")"<<"(npwhnpTarg->lexical_head<<")\n";
   						   	   return true;
   						   }   
   					   }
   					   
   				   }
   				   childsSinv=childsSinv->next;
   			   }
   		   }
   		   return false;
   	   }
   
      
   	   //"SBAR < WHNP=target [ < (S < (VP !$-- NP) !< SBAR) | < (VP !$-- NP) !< S ]"
   	   //Let's try to put it into words
       //Relations can be grouped using brackets [ and ]. So the expression, NP [< NN | < NNS] & > S means that
       // (1) NP dominates either an NN or an NNS
       // (2) S dominates NP
   
       //So in our case of study.
       // 1. SBAR < WHNP=target
       // 2.1 SBAR < ((S < VP !$-- NP) !< SBAR)  OR
       // 2.2 SBAR < (VP !$-- NP) & !< S
      
       
inline const bool &buildNsubj11(const unsigned long &cons) {
   		   if (cons==PENN_CON_SBAR) {
   			   CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
   			   //std::cout<<"first while \n";
   			   const CStateNode* whnpTarg=0;
   			   bool thereIsWhnp=false;
   			   while(childsSbar!=0){
   				   const CStateNode* whnpTarg=childsSbar->node;
   				   if (CConstituent::clearTmp(whnpTarg->constituent.code())==PENN_CON_WHNP && !(isLinked(&node, whnpTarg))) {
   					  thereIsWhnp=true;
   				   }
   				   if (thereIsWhnp) {
   					   CStateNodeList* childsSbar2=node.m_umbinarizedSubNodes;
   					   bool secondCondition=true;
   					   bool thereIsVp=false;
   					   //std::cout<<"second while \n";
   					   while(childsSbar2!=0) {
   						   const CStateNode* childSbar=childsSbar2->node;
   						   if (CConstituent::clearTmp(childSbar->constituent.code())==PENN_CON_S){
   							   secondCondition=false;
   						   } //if childSbar
   						   else if (CConstituent::clearTmp(childSbar->constituent.code())==PENN_CON_VP){
   							   thereIsVp=true;
   							   if (childsSbar2->previous!=0) {
   								   const CStateNode* immLeftSister=childsSbar2->previous->node;
   								   if (CConstituent::clearTmp(immLeftSister->constituent.code())==PENN_CON_NP) {
   									   secondCondition=false;
   								   }
   							   } //if childsSbar2
   						   }//else if
   						   childsSbar2=childsSbar2->next;
   					   } //while childsSbar2
   					   if (thereIsVp && secondCondition) secondCondition=true;
   					   
   					   // 2.1 SBAR < ((S < VP !$-- NP) !< SBAR)  OR
   					   CStateNodeList* childsSbar3=node.m_umbinarizedSubNodes;
   					   bool thereIsVpAndS=false;
   					   bool firstCondition=true;
   					   //std::cout<<"third while \n";
   					   while(childsSbar3!=0) {
   						   const CStateNode* childSbar=childsSbar3->node;
   						   if (CConstituent::clearTmp(childSbar->constituent.code())==PENN_CON_S){
   							   if (childsSbar3->previous!=0) {
   								   const CStateNode* immLeftSister=childsSbar3->previous->node;
   								   if (CConstituent::clearTmp(immLeftSister->constituent.code())==PENN_CON_NP) {
   									   firstCondition=false;
   								   }//if inmLeftSister
   							   }//if (childsSbar3)
   							   CStateNodeList* childsS=childSbar->m_umbinarizedSubNodes;
   							   while(childsS!=0) {
   								   const CStateNode* childS=childsS->node; 
   								   if (CConstituent::clearTmp(childS->constituent.code())==PENN_CON_VP) {
   									   thereIsVpAndS=true;
   								   }
   								   if (CConstituent::clearTmp(childS->constituent.code())==PENN_CON_SBAR) {
   									   firstCondition=false;
   								   }
   								   childsS=childsS->next;
   							   }//while childsS
   						   }//if (penn_con_s)
   						   childsSbar3=childsSbar3->next;
   					   }//while childSbar3   
   					   if (firstCondition && thereIsVpAndS){
   						   firstCondition=true;
   					   }   
   					   if (secondCondition || firstCondition) {
//   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(STANFORD_DEP_NSUBJ, whnpTarg->lexical_head, node.lexical_head)){
   							   addLinked(&node, whnpTarg);
   							   //std::cout<<"nSubj11"<<" (head: "<<node.lexical_head<<")"<<"(whnpTarg->lexical_head<<")\n";
   							   return true;
   						   }
   					   }
   				   } //if whnp
   		   		   childsSbar=childsSbar->next;
   			   	} //while(childsSbar!=0)
   		   
   		   }//if node=sbar
   		   return false;	
   	   	}//method
       
   	
   
   		//"SBAR !< WHNP < (S !< (NP $++ VP)) > (VP > (S $- WHNP=target))",
   		//Simplified version
   		// SBAR !< WHNP & < (S !< (NP $++ VP)) & > (VP > (S $- WHNP=target))
        //which means that SBAR is not the immediate head of WHNP but it is the immediate head of (S !< (NP $++ VP)), and it also means that 
        //SBAR is the immediate child of the following structure (VP > (S $- WHNP=target)) which is headed by S.
        
   	   
   	   //ARE WE SURE ABOUT THIS ONE? I'm NOT...but I think is fine.
   	   
   	   //YUE:
   	   //little trick in this one. I have to start with the immediate head of the rightmost S (in the rule)... but I don't care about its constituent
   	   //S must have a head, because otherwise it cannot be the immediate right sister of WHNP
   	   
inline const bool &buildNsubj12(const unsigned long &cons) {
        	CStateNodeList* childsNode=node.m_umbinarizedSubNodes;
        	while(childsNode!=0) {
        		const CStateNode* secondS=childsNode->node;
        		if (CConstituent::clearTmp(secondS->constituent.code())==PENN_CON_S) {
        			CStateNodeList* leftSisterS=childsNode;
        			if (leftSisterS->previous!=0){
        				const CStateNode* whnpTarg=leftSisterS->node;
        				if (CConstituent::clearTmp(whnpTarg->constituent.code())==PENN_CON_WHNP && !(isLinked(&node,whnpTarg))) {
        					CStateNodeList* childsS=secondS->m_umbinarizedSubNodes;
        					while(childsS!=0) {
        						const CStateNode* vp=childsS->node;
        						if (CConstituent::clearTmp(vp->constituent.code())==PENN_CON_VP) {
        							CStateNodeList* childsVp=vp->m_umbinarizedSubNodes;
        							while(childsVp!=0){
        								const CStateNode* sbarHead=childsVp->node;
        								if (CConstituent::clearTmp(sbarHead->constituent.code())==PENN_CON_SBAR){
        									CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
        									bool noWhnp=true;
        									while(childsSbar!=0){
        										if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHNP){
        											noWhnp=false;
        										}
        										//(S !< (NP $++ VP)
        										bool secondCondition=true;
        										const CStateNode* sSonOfSbar=childsSbar->node;
        										if (CConstituent::clearTmp(sSonOfSbar->constituent.code())==PENN_CON_S){
        											CStateNodeList* childs2S=sSonOfSbar->m_umbinarizedSubNodes;
        											while(childs2S!=0){
        												const CStateNode* npSonofS=childs2S->node;
        												if (CConstituent::clearTmp(npSonofS->constituent.code())==PENN_CON_NP) {
        													CStateNodeList* leftSistersNP=childs2S;
        													while(leftSistersNP!=0){
        														if (CConstituent::clearTmp(leftSistersNP->node->constituent.code())==PENN_CON_VP) {
        															secondCondition=false;
        														}
        														leftSistersNP=leftSistersNP->next;
        													}
        												}
        												childs2S=childs2S->next;
        											}
        											if (noWhnp && secondCondition){
//        												CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
        												if (buildStanfordLink(STANFORD_DEP_NSUBJ, whnpTarg->lexical_head, sbarHead->lexical_head)){
        													addLinked(&node,whnpTarg);
        													//std::cout<<"nSubj11"<<" (head: "<<sbarHead->lexical_head<<")"<<"(whnpTarg->lexical_head<<")\n";
        													return true;	
        												}
        											}
        										}
        										childsSbar=childsSbar->next;
        									}
        								}
        								childsVp=childsVp->next;
        							}
        						}
        						childsS=childsS->next;
        					}
        				}
        			}
        		}
        		childsNode=childsNode->next;
        	}
        	return false;
        }	
   
        //"SQ < ((NP < EX) $++ NP=target)",
      
inline const bool & buildNsubj13(const unsigned long &cons) {
   	   	if (cons==PENN_CON_SQ) {
   		   //const CStateNode* s=immediatelyDominates(node, PENN_CON_S);
   		   CStateNodeList* childs=node.m_umbinarizedSubNodes;
   		   while(childs!=0) {
   			   const CStateNode* np=childs->node;
   			   if (CConstituent::clearTmp(np->constituent.code())==PENN_CON_NP) {
   				   CStateNodeList* sistersNp=childs;
   				   bool sisterIsNp=false;
   				   const CStateNode* npTarg;
   				   while(sistersNp!=0) {
   					   npTarg=sistersNp->node;
   					   if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))) {
   						   sisterIsNp=true;
   					   }
   					   sistersNp=sistersNp->next;
   				   }
   				   CStateNodeList* childsNp=np->m_umbinarizedSubNodes;
   				   while(childsNp!=0){
   					   const CStateNode* ex=childsNp->node;
   					   //if ((*words)[noex->lexical_head].word==CWord("yue")) {}
   					   
   					   if ((*words)[ex->lexical_head].tag.code()==PENN_TAG_EX) {
   						   if (sisterIsNp) {
//   							   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   							   if (buildStanfordLink(STANFORD_DEP_NSUBJ, npTarg->lexical_head, node.lexical_head)){
   								   addLinked(&node,npTarg);
   								   //std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
   								   return true;
   							   }
   						   }
   					   }
   					   childsNp=childsNp->next;
   				   }
   			   }
   			   childs=childs->next;
   		   }
   	   }
   	   return false;
       }
