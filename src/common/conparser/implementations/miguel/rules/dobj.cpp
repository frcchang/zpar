/* 
    	"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"

          // match "give it next week"
          
          "VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
          
         // basic direct object cases
          "VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )",
          // cdm Jul 2010: No longer require WHNP as first child of SBARQ below: often not because of adverbials, quotes, etc., and removing restriction does no harm
          // this next pattern used to assume no empty NPs. Corrected.  If you adjust this pattern, also adjust the corresponding one for attr!
         
         "SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
         
          // matches direct object in relative clauses "I saw the book that you bought"
          "SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",

          // matches direct object in relative clauses "I saw the book that you said you bought"
          "SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",

          // matches direct object for long dependencies in relative clause without explicit relative pronouns
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
          
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"
*/



    /*
       *"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")"
       *
       */
       bool buildDobj1() {
    	   if (node.constituent==PENN_CON_VP) {
    		   CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		   bool secondCondition=true;
    		   while(childsVp!=0){
    			   const CStateNode* vbChildVp=childsVp->node;
    			   if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
    				   CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
    			       while(childsVB!=0){
    			    	   if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
    			    		   secondCondition=false;
    			    	   }
    			       	   childsVB=childsVB->next;
    			       	}
    			   }
    			   childsVp=childsVp->next;
    		   }

    		   if (secondCondition) { //VP does not a have child that looks like (/^(?:VB|AUX)/ < " + copularWordRegex + ")
    			   childsVp=node.m_umbinarizedSubNodes;
    			   while(childsVp!=0){
    				   const CStateNode* npChildVp=childsVp->node;

    				   if (npChildVp->constituent==PENN_CON_NP) {
    					   	  //now the immediate left sister must look like (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))
    					   	   CStateNodeList* previous=childsVp->previous;
    					   	   if (previous!=0) {
    					   		   const CStateNode* leftSisterNpTarg=previous->node;

    					   		   if ((leftSisterNpTarg->constituent==PENN_CON_NP || leftSisterNpTarg->constituent==PENN_CON_WHNP) && !(isLinked(&node, leftSisterNpTarg))) {
    					   			   bool noChildNNTime=true;
    					   			   CStateNodeList* childsOfTarg=leftSisterNpTarg->m_umbinarizedSubNodes;
    					   			   while(childsOfTarg!=0){
    					   				   const CStateNode* nnChildNp=childsOfTarg->node;
    					   				   if ((nnChildNp->type==CStateNode::LEAF)
    					   					   && (nnChildNp->lexical_head==leftSisterNpTarg->lexical_head) //<#
    					   					   && (compareWordToTimeLotWordRegex((*words)[nnChildNp->lexical_head].word))
    					   					   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
    					   					   noChildNNTime=false;
    					   					}
    					   				   childsOfTarg=childsOfTarg->next;
    					   			   }


    					   			   if (noChildNNTime) {
    					   				   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
    					   				   if (buildStanfordLink(label, leftSisterNpTarg->lexical_head, node.lexical_head)) {
    					   					   addLinked(&node,leftSisterNpTarg);
    					   					   return true;
    					   				   }
    					   			   }
    					   	   	   }
    					   	   }

    				   }
    				   childsVp=childsVp->next;
    			   }
    		   }
    	   }
    	   return false;

       }

       //"VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")"
          bool buildDobj2() {
          	if (node.constituent==PENN_CON_VP) {
          		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
          		bool secondCondition=true;
          		while(childsVp!=0){
          			const CStateNode* vbChildVp=childsVp->node;
          		    if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
          		    	CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
          		    	while(childsVB!=0){
          		    		if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
          		    			secondCondition=false;
          		    		}
          		    		childsVB=childsVB->next;
          		    	}
          		    }
          		    childsVp=childsVp->next;
          		}
          		if (secondCondition){
          			childsVp=node.m_umbinarizedSubNodes;
          			while(childsVp!=0){
          				const CStateNode* fstnpChildVp=childsVp->node;
          				if (fstnpChildVp->constituent==PENN_CON_NP) {
          					CStateNodeList* childsNP=fstnpChildVp->m_umbinarizedSubNodes;
          					while(childsNP!=0){
          						const CStateNode* npChildVp=childsNP->node;
          						if (npChildVp->constituent==PENN_CON_NP) {
          						//now the immediate left sister must look like (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))
          							CStateNodeList* previous=childsVp->previous;
          						    if (previous!=0) {
          						    	const CStateNode* leftSisterNpTarg=previous->node;
          						    	if ((leftSisterNpTarg->constituent==PENN_CON_NP || leftSisterNpTarg->constituent==PENN_CON_WHNP) && !(isLinked(&node, leftSisterNpTarg))) {
          						    		bool noChildNNTime=true;
          						    		CStateNodeList* childsOfTarg=leftSisterNpTarg->m_umbinarizedSubNodes;
          						    		while(childsOfTarg!=0){
          						    			const CStateNode* nnChildNp=childsOfTarg->node;
          						    			if ((nnChildNp->type==CStateNode::LEAF)
          						    					&& (nnChildNp->lexical_head==leftSisterNpTarg->lexical_head) //<#
          						    					&& (compareWordToTimeLotWordRegex((*words)[nnChildNp->lexical_head].word))
          						    					&& ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
          						    						noChildNNTime=false;
          						    			}
          						    			childsOfTarg=childsOfTarg->next;
          						    		}
          						            if (noChildNNTime) {
          						            	CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
          						    			if (buildStanfordLink(label, leftSisterNpTarg->lexical_head, node.lexical_head)) {
          						    				addLinked(&node,leftSisterNpTarg);
          						    				return true;
          						    			}
          						    		}
          						    	}
          						    }
          						    				   }
          						childsNP=childsNP->next;
          					}
          				}
          				childsVp=childsVp->next;
          			}
          		}
          	}
          	return false;

          }

 //"VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )"
 bool buildDobj3() {

 }


 //"SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
 bool buildDobj4() {

 }

//"SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",
 bool buildDobj5() {

 }

 //"SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",
 bool buildDobj6() {
 	//we do not care about what the head of S is, S is not the the head, but is the "lexical" head of VP, which is the head of SBAR which is the the "stanford" link head of WHNP
 	CStateNodeList* childs=node.m_umbinarizedSubNodes;
 	while(childs!=0){
 		const CStateNode* sChild=childs->node;
 		if (sChild->constituent==PENN_CON_S){
 			if (childs->previous!=0){
 				const CStateNode* whnpTarg=childs->previous->node;
 				if (whnpTarg->constituent==PENN_CON_WHNP){ //I'll check whether it is linked when I have the head (SBAR)
 					CStateNodeList* childsS=sChild->m_umbinarizedSubNodes;
 					bool npFound=false;
 					while(childsS!=0){
 						if (childsS->node->constituent==PENN_CON_NP){
 							npFound=true;
 						}
 						childsS=childsS->next;
 					}
 					if (npFound){
 						childsS=sChild->m_umbinarizedSubNodes;
 						while(childsS!=0){
 							const CStateNode* vpChilds=childsS->node;
 							if (vpChilds->constituent==PENN_CON_VP){
 								CStateNodeList* childsVp=vpChilds->m_umbinarizedSubNodes;
 								while(childsVp!=0){
 									const CStateNode* sbarHead=childsVp->node;
 									if ((sbarHead->constituent==PENN_CON_SBAR) && !(isLinked(sbarHead, whnpTarg))){
 										CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
 										bool firstCondition=true;
 										while(childsSbar!=0){
 											if (childsSbar->node->constituent==PENN_CON_WHNP ||childsSbar->node->constituent==PENN_CON_WHADVP){
 												firstCondition=false;
 											}
 											childsSbar=childsSbar->next;
 										}
 										if (firstCondition){
 											childsSbar=sbarHead->m_umbinarizedSubNodes;
 											while(childsSbar!=0){
 												const CStateNode* sChildSbar=childsSbar->node;
 												if (sChildSbar->constituent==PENN_CON_S){
 													CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
 													while(childsS!=0){
 														const CStateNode* npChildS=childsS->node;
 														if (npChildS->constituent==PENN_CON_NP){ //again, @NP, basic "class" of NP. how come?
 															//A $++ B 	A is a left sister of B
 															CStateNodeList* rightSistersNp=childsS;
 															while(rightSistersNp!=0){
 																if (rightSistersNp->node->constituent==PENN_CON_VP){
 																	CStateNodeList* rightSistersVp=rightSistersNp;
 																	bool noRightSister=true;
 																	while(rightSistersVp!=0){
 																		if (rightSistersVp->node->constituent==PENN_CON_NP){
 																			noRightSister=false;
 																		}
 																		rightSistersVp=rightSistersVp->next;
 																	}
 																	if (noRightSister){
 																		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
 																		if (buildStanfordLink(label, whnpTarg->lexical_head, sbarHead->lexical_head)) {
 																			addLinked(&node,whnpTarg); //this is not correct! but almost
 																			return true;
 					          						    			}

 																	}
 																}
 																rightSistersNp=rightSistersNp->next;
 															}
 														}
 														childsS=childsS->next;
 													}
 												}
 												childsSbar=childsSbar->next;
 											}
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
 		}
			childs=childs->next;
		}
 	return false;
	}
//"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
 bool buildDobj7() {

 }
//"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"
 bool buildDobj8() {

 }
