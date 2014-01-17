
 //"VP < (S=target < (VP !<, TO|VBG|VBN) !$-- NP)",
    bool buildCComp1(const unsigned long &cons) {
    	if (cons==PENN_CON_VP) {
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp=0){
    			const CStateNode* sTarg=childsVp->node;
    			if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))) {
    				bool inCondition=true; //!<, TO|VBG|VBN
    				//A <, B B is the first child of A
    				CStateNodeList* childsS=node.m_umbinarizedSubNodes;
    				while(childsS!=0) {
    					const CStateNode* vpChildS=childsS->node;
    					if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP) {
    						CStateNodeList* childsVpIn=vpChildS->m_umbinarizedSubNodes;
    						if (childsVpIn!=0) { //only the first one
    							const CStateNode* firstChild=childsVpIn->node;
    							if (((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_TO)
    								 || ((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
    								 || ((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) {

    								inCondition=false;

    							}
    						}
    					}
    					childsS=childsS->next;
    				}
    				if (inCondition) {
    					//A $-- B (A is a right sister of B)
    					bool secondCondition=true;
    					CStateNodeList* leftSistersS=childsVp->previous;
    					while(leftSistersS!=0){
    						if (CConstituent::clearTmp(leftSistersS->node->constituent.code())==PENN_CON_NP){
    							secondCondition=false;
    						}
    						leftSistersS=leftSistersS->previous;
    					}
    					if (secondCondition) {
//    						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
    						if (buildStanfordLink(STANFORD_DEP_CCOMP, sTarg->lexical_head, node.lexical_head)) {
    							addLinked(&node,sTarg);
    						    return true;
    						}
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;
    }


    //"VP < (SBAR=target < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/))",
      bool buildCComp2(const unsigned long &cons) {

      	if (cons==PENN_CON_VP){
      		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
      		while(childsVp!=0){
      			const CStateNode* sbarTarg=childsVp->node;
      			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
      				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
      				bool secondCondition=false;
      				if (childsSbar!=0){ //Only first child <,
      					const CStateNode* firstChildSbar=childsSbar->node;
      					if (((*words)[firstChildSbar->lexical_head].tag.code()==PENN_TAG_IN)
      						|| ((*words)[firstChildSbar->lexical_head].tag.code()==PENN_TAG_DT)){
      						CStateNodeList* childsInDt=firstChildSbar->m_umbinarizedSubNodes;
      						if (((*words)[firstChildSbar->lexical_head].word==g_word_whether)&&((*words)[firstChildSbar->lexical_head].word==g_word_that)) {
      							secondCondition=true;
      						}
      					}
      				}
      				if (secondCondition){
      					while(childsSbar!=0){
      						const CStateNode* sChildSbar=childsSbar->node;
      						if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
      							//A dominates B via an unbroken chain of (zero or more) nodes matching description C
      							CStateNodeList* sChain=new CStateNodeList();
      							findChain(PENN_CON_S,PENN_CON_VP, sChildSbar, sChain);
      							if (sChain->node==0) {
      								sChain->clear();
      							   	sChain=0;
      							}
      							while(sChain!=0){
      								if (CConstituent::clearTmp(sChain->node->constituent.code())==PENN_CON_VP){
//      									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
      									if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
      										addLinked(&node,sbarTarg);
      										return true;
      									}
      								}
      								sChain=sChain->next;
      							}
      						}
      						childsSbar=childsSbar->next;
      					}
      				}
      			}
      			childsVp=childsVp->next;
      		}
      	}
      	return false;
      }



      //"VP < (SBAR=target < (SBAR < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/)) < CC|CONJP)",
         bool buildCComp3(const unsigned long &cons) {

            	if (cons==PENN_CON_VP){
            		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
            		while(childsVp!=0){
            			const CStateNode* sbarTarg=childsVp->node;
            			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
            				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
            				bool secondCondition=false;
            				if (childsSbar!=0){ //Only first child <,
            					const CStateNode* firstChildSbar=childsSbar->node;
            					if (((*words)[firstChildSbar->lexical_head].tag.code()==PENN_TAG_IN)
            						|| ((*words)[firstChildSbar->lexical_head].tag.code()==PENN_TAG_DT)){
            						CStateNodeList* childsInDt=firstChildSbar->m_umbinarizedSubNodes;
            						//while(childsInDt!=0){
            							if (((*words)[firstChildSbar->lexical_head].word==g_word_whether)&&((*words)[firstChildSbar->lexical_head].word==g_word_that)) {
            								secondCondition=true;
            							}
            							//childsInDt=childsInDt->next;
            						//}
            					}
            				}
            				bool thirdCondition=false;
            				while(childsSbar!=0){
            					if (((*words)[childsSbar->node->lexical_head].tag.code()==PENN_TAG_CC)
            					    || (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_CONJP)){
            						thirdCondition=true;
            					}
            					childsSbar=childsSbar->next;
            				}
            				if (secondCondition && thirdCondition){
            					childsSbar=sbarTarg->m_umbinarizedSubNodes;
            					while(childsSbar!=0){
            						const CStateNode* sChildSbar=childsSbar->node;
            						if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
            							//A dominates B via an unbroken chain of (zero or more) nodes matching description C
            							CStateNodeList* sChain=new CStateNodeList();
            							findChain(PENN_CON_S,PENN_CON_VP, sChildSbar, sChain);
            							if (sChain->node==0) {
            								sChain->clear();
            							   	sChain=0;
            							}
            							while(sChain!=0){
            								if (CConstituent::clearTmp(sChain->node->constituent.code())==PENN_CON_VP){
//            									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
            									if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
            										addLinked(&node,sbarTarg);
            										return true;
            									}
            								}
            								sChain=sChain->next;
            							}
            						}
            						childsSbar=childsSbar->next;
            					}
            				}
            			}
            			childsVp=childsVp->next;
            		}
            	}
            	return false;
            }


//"VP < (SBAR=target < (S < VP) !$-- NP !<, (IN|WHADVP))",
 bool buildCComp4(const unsigned long &cons) {
 	if (cons==PENN_CON_VP) {
 		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
 		while(childsVp!=0){
 			const CStateNode* sbarTarg=childsVp->node;
 			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
 				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
 				bool secondCondition=true;
 				bool thirdCondition=true;

 				//A $-- B A is a right sister of B
 				CStateNodeList* leftSistersSbar=childsVp->previous;
 				while(leftSistersSbar!=0){
 					if (CConstituent::clearTmp(leftSistersSbar->node->constituent.code())==PENN_CON_NP){
 						secondCondition=false;
 					}
 					leftSistersSbar=leftSistersSbar->previous;
 				}
 				//A !<, B 	B is the first child of A
 				if (childsSbar!=0) { //only the first child
 					if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHADJP || (*words)[childsSbar->node->lexical_head].tag.code()==PENN_TAG_IN) {
 						thirdCondition=false;
 					}
 				}
 				if (secondCondition && thirdCondition) {
 					while(childsSbar!=0) {
 						const CStateNode* sChildSbar=childsSbar->node;
 						if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
 							CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
 							while(childsS!=0){
 								const CStateNode* vpChildS=childsS->node;
 								if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP){
// 									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
 									if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
 										addLinked(&node,sbarTarg);
 									    return true;
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
 	return false;

 }

 //"VP < (SBAR=target < (S < VP) !$-- NP <, (WHADVP < (WRB < /^(?i:how)$/)))",
 bool buildCComp5(const unsigned long &cons) {
 	if (cons==PENN_CON_VP){
 		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
 		while(childsVp!=0){
 			const CStateNode* sbarTarg=childsVp->node;
 			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))){
 				bool noNpLeftSister=true;
 				CStateNodeList* leftSistersSbar=childsVp->previous;
 				while(leftSistersSbar!=0){
 					if (CConstituent::clearTmp(leftSistersSbar->node->constituent.code())==PENN_CON_NP){
 						noNpLeftSister=false;
 					}
 					leftSistersSbar=leftSistersSbar->previous;
 				}

 				if (noNpLeftSister){
 					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
 					while(childsSbar!=0){
 						const CStateNode* sChildSbar=childsSbar->node;
 						if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
 							CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
 							while(childsS!=0){
 								if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
 									const CStateNode* firstChildSbar=sbarTarg->m_umbinarizedSubNodes->node;
 									if (CConstituent::clearTmp(firstChildSbar->constituent.code())==PENN_CON_WHADVP){
 										CStateNodeList* childsWhadvp=firstChildSbar->m_umbinarizedSubNodes;
 										while(childsWhadvp!=0){
 											const CStateNode* wrbChildWh=childsWhadvp->node;
 											if ((*words)[wrbChildWh->lexical_head].tag.code()==PENN_TAG_WRB){
 												//CStateNodeList* childsWrb=wrbChildWh->m_umbinarizedSubNodes;
 												//while(childsWrb!=0){
 													if (((*words)[wrbChildWh->lexical_head].word==g_word_how)) {
// 														CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
 														if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
 															addLinked(&node,sbarTarg);
 															return true;
 														}
 													}
 													//childsWrb=childsWrb->next;
 												//}
 											}
 											childsWhadvp=childsWhadvp->next;
 										}
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
 	return false;

 }

 //"VP < (/^VB/ < " + haveRegex + ") < (S=target < @NP < VP)",
   bool buildCComp6(const unsigned long &cons) {

   	if (cons==PENN_CON_VP){
   		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
   		bool firstCondition=false;
   		while(childsVp!=0){
   			const CStateNode* vbChildVp=childsVp->node;
   			if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)
   					|| ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PAST)
   					|| ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
   					|| ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)
   					|| ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PRES)
   					|| ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)) {

   				CStateNodeList* childsVb=vbChildVp->m_umbinarizedSubNodes;
   				//while(childsVb!=0){
   					//const CStateNode* haveChildVb=childsVb->node;
   					 if (compareWordToHaveWordRegex((*words)[vbChildVp->lexical_head].word)) {
   						 firstCondition=true;
   					 }
   					//childsVb=childsVb->next;
   				//}
   			}
   			childsVp=childsVp->next;
   		}
   		childsVp=node.m_umbinarizedSubNodes;
   		if (firstCondition){
   			while(childsVp!=0){
   				const CStateNode* sTarg=childsVp->node;
   				if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))) {
   					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
   					while(childsS!=0){
   						const CStateNode* npChildS=childsS->node;
   						if (CConstituent::clearTmp(npChildS->constituent.code())==PENN_CON_NP){
   							CStateNodeList* childsNp=npChildS->m_umbinarizedSubNodes;
   							while(childsNp!=0){
   								if (CConstituent::clearTmp(childsNp->node->constituent.code())==PENN_CON_VP){
//   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
   									if (buildStanfordLink(STANFORD_DEP_CCOMP, sTarg->lexical_head, node.lexical_head)) {
   										addLinked(&node,sTarg);
   										return true;
   									}
   								}
   								childsNp=childsNp->next;
   							}
   						}
   						childsS=childsS->next;
   					}
   				}
   				childsVp=childsVp->next;
   			}
   		}
   	}
   	return false;
   }


   //"S|SINV < (S|SBARQ=target $+ /^(,|\\.|'')$/ !$- /^(?:CC|:)$/ !< (VP < TO|VBG|VBN))",
   bool buildCComp7(const unsigned long &cons) {
   	if (cons==PENN_CON_S||cons==PENN_CON_SINV){
   		CStateNodeList* childsSSinv=node.m_umbinarizedSubNodes;
   		while(childsSSinv!=0){
   			const CStateNode* ssbarqTarg=childsSSinv->node;
   			if ((CConstituent::clearTmp(ssbarqTarg->constituent.code())==PENN_CON_S||CConstituent::clearTmp(ssbarqTarg->constituent.code())==PENN_CON_SBARQ) && (!isLinked(&node,ssbarqTarg))){
   				bool secondCond=true;
   				bool thirdCond=true;

   				//A $- B 	A is the immediate right sister of B
   				if (childsSSinv->previous!=0){
   					if ((*words)[childsSSinv->previous->node->lexical_head].tag.code()==PENN_TAG_CC) { //:CC, subcategory of CC...¿? how come? I match it with CC ... should be enough.
   						secondCond=false;
   					}
   				}

   				CStateNodeList* childsSSbarq=ssbarqTarg->m_umbinarizedSubNodes;
   				while(childsSSbarq!=0){
   					const CStateNode* vpChildssbarq=childsSSbarq->node;
   					if (CConstituent::clearTmp(vpChildssbarq->constituent.code())==PENN_CON_VP){
   						CStateNodeList* childsVp=vpChildssbarq->m_umbinarizedSubNodes;
   						while(childsVp!=0){
   							if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)
   									|| ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
   									|| ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) { //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
   								thirdCond=false;

   							}
   							childsVp=childsVp->next;
   						}
   					}
   					childsSSbarq=childsSSbarq->next;
   				}

   				//A $+ B 	A is the immediate left sister of B
   				if (secondCond && thirdCond && childsSSinv->next!=0){
   					if (((*words)[childsSSinv->next->node->lexical_head].word==g_word_comma)
   							||((*words)[childsSSinv->next->node->lexical_head].word==g_word_dot)
   							||((*words)[childsSSinv->next->node->lexical_head].tag.code()==PENN_TAG_R_QUOTE)) {
//   						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
   						if (buildStanfordLink(STANFORD_DEP_CCOMP, ssbarqTarg->lexical_head, node.lexical_head)) {
   							addLinked(&node,ssbarqTarg);
   							return true;
   						}
   					}

   				}

   			}
   			childsSSinv=childsSSinv->next;
   		}
   	}
   	return false;
   }


//"ADVP < (SBAR=target < (IN < /^(?i:as|that)/) < (S < (VP !< TO)))"
bool buildCComp8(const unsigned long &cons) {
	if (cons==PENN_CON_ADVP){
		CStateNodeList* childsAdvp=node.m_umbinarizedSubNodes;
		bool secondCondition=false;
		while(childsAdvp!=0){
			const CStateNode* sChildAdvp=childsAdvp->node;
			bool inCondition=true;
			CStateNodeList* childsS=sChildAdvp->m_umbinarizedSubNodes;
			while(childsS!=0){
				const CStateNode* vpChildS=childsS->node;
				if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP) {
					CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
					while(childsVp!=0){
						const CStateNode* toChildVp=childsVp->node;
						if (((*words)[toChildVp->lexical_head].tag.code()==PENN_TAG_TO)) {
							inCondition=false;

						}
						childsVp=childsVp->next;
					}
					secondCondition=inCondition;
				}
				childsS=childsS->next;
			}
			childsAdvp=childsAdvp->next;
		}
		if (secondCondition) {
			childsAdvp=node.m_umbinarizedSubNodes;
			while(childsAdvp!=0){
				const CStateNode* sbarTarg=childsAdvp->node;
				if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
					while(childsSbar!=0){
						const CStateNode* inChildSbar=childsSbar->node;
						if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_IN) {
								if (((*words)[inChildSbar->lexical_head].word==g_word_as)&&((*words)[inChildSbar->lexical_head].word==g_word_that)) {
//									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
									if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
										addLinked(&node,sbarTarg);
									    return true;
									}
								}
						}
						childsSbar=childsSbar->next;
					}
				}


						//(*words)[childsSbar->node->lexical_head].tag.code()==PENN_TAG_IN
				childsAdvp=childsAdvp->next;
			}

		}

	}
	return false;
}

//"ADJP < (SBAR=target !< (IN < as) < S)"
    bool buildCComp9(const unsigned long &cons) {
    	if (cons==PENN_CON_ADJP){
    		CStateNodeList* childsAdjp=node.m_umbinarizedSubNodes;
    		while(childsAdjp!=0){
    			const CStateNode* sbarTarg=childsAdjp->node;
    			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
    				bool firstCondition=true;
    				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				while(childsSbar!=0){
    					const CStateNode* inChildSbar=childsSbar->node;
    					if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_IN) {
    						if ((*words)[inChildSbar->lexical_head].word==g_word_as) {
    							firstCondition=false;
    						}
    					}
    					childsSbar=childsSbar->next;
    				}
    				if (firstCondition){
    					childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					while(childsSbar!=0){
    						const CStateNode* schildSbar=childsSbar->node;
    						if (CConstituent::clearTmp(schildSbar->constituent.code())==PENN_CON_S){
//    							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
    							 if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,sbarTarg);
    							 	 return true;
    							}
    						}
    						childsSbar=childsSbar->next;
    					}
    				}
    			}
    			childsAdjp=childsAdjp->next;
    		}
    	}
    	return false;
    }

    //"S <, (SBAR=target <, (IN < /^(?i:that|whether)$/) !$+ VP)",
    bool buildCComp10(const unsigned long &cons) {
    	if (cons==PENN_CON_S){
    		CStateNodeList* childsS=node.m_umbinarizedSubNodes;
    		if (childsS!=0){ //<, first child only
    			const CStateNode* sbarTarg=childsS->node;
    			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
    				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				if (childsSbar!=0){ //<, first child only
    					const CStateNode* inChildSbar=childsSbar->node;
    					if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_IN) {
    						// A $+ B. A is the immediate left sister of B
    						bool secondCondition=true;
    						if (childsSbar->next!=0){
    							const CStateNode* leftSisterIn = childsSbar->next->node;
    							if (CConstituent::clearTmp(leftSisterIn->constituent.code())==PENN_CON_VP){
    								secondCondition=false;
    							}
    						}
    						if (secondCondition){
    							CStateNodeList* childsIn=inChildSbar->m_umbinarizedSubNodes;
    							//while(childsIn!=0){
    								if (((*words)[inChildSbar->lexical_head].word==g_word_whether)&&((*words)[inChildSbar->lexical_head].word==g_word_that)) {
//    									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
    									if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
    										addLinked(&node,sbarTarg);
    										return true;
    									}
    								}
    								//childsIn=childsIn->next;
    							//}
    						}
    					}
    				}
    			}
    		}

    	}
    	return false;

    }
    //"@NP < JJ|NN|NNS < (SBAR=target [ !<(S < (VP < TO )) | !$-- NP|NN|NNP|NNS ] )"

           	   //SBAR=target !< (S<(VP<TO))      (SBAR does not have a child such as (S<...
           	   //SBAR=target!$-- NP|NN|NNP|NNS   (SBAR is not a right sister of NP ...)
        //but this is an OR
        //so, not at the same time...?

        bool buildCComp11(const unsigned long &cons) {
        	if (cons==PENN_CON_NP){
        		CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
        		while(childsNp!=0){
        			const CStateNode* jjnnnsChildNp=childsNp->node;
        			if (((*words)[jjnnnsChildNp->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)
        					||((*words)[jjnnnsChildNp->lexical_head].tag.code()==PENN_TAG_NOUN)
        					||((*words)[jjnnnsChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)) {

        				CStateNodeList* childsJJ=jjnnnsChildNp->m_umbinarizedSubNodes;
        				while(childsJJ!=0){
        					const CStateNode* sbarTarg=childsJJ->node;
        					if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
        						bool childCondition=true;
        						bool sisterCondition=true; //I assume at first sight that they are true.

        						CStateNodeList* leftSistersSbar=childsJJ->previous;
        						while(leftSistersSbar!=0){
        							const CStateNode* sister=leftSistersSbar->node;
        							if ((CConstituent::clearTmp(sister->constituent.code())==PENN_CON_NP)
        							     ||((*words)[sister->lexical_head].tag.code()==PENN_TAG_NOUN)
        							     ||((*words)[sister->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER)
        							     ||((*words)[sister->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)) {
        								childCondition=false;
        							}
        							leftSistersSbar=leftSistersSbar->previous;
        						}

        						//SBAR=target !< (S<(VP<TO))
        						CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
        						while(childsSbar!=0){
        							const CStateNode* schildSbar=childsSbar->node;
        							if (CConstituent::clearTmp(schildSbar->constituent.code())==PENN_CON_S){
        								CStateNodeList* childsS=schildSbar->m_umbinarizedSubNodes;
        								while (childsS!=0){
        									const CStateNode* vpChild=childsS->node;
        									if (CConstituent::clearTmp(vpChild->constituent.code())==PENN_CON_VP){
        										CStateNodeList* childsVp=vpChild->m_umbinarizedSubNodes;
        										while(childsVp!=0){
        											if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)){
        												childCondition=false;
        											}
        											childsVp=childsVp->next;
        										}
        									}
        									childsS=childsS->next;
        								}
        							}
        							childsSbar=childsSbar->next;
        						}

        						if (childCondition || sisterCondition) { //[] represents an or.
//        							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
        							if (buildStanfordLink(STANFORD_DEP_CCOMP, sbarTarg->lexical_head, node.lexical_head)) {
        								addLinked(&node,sbarTarg);
        							    return true;
        							}
        						}
        					}
        					childsJJ=childsJJ->next;
        				}
        			}
        			childsNp=childsNp->next;
        		}
        	}
        	return false;

        }

