    //"VP < (S=target !$- (NN < order) < (VP < TO))",    // used to have !> (VP < (VB|AUX < be))
inline const bool & buildXComp1(const unsigned long &cons) {
    	if (cons==PENN_CON_VP){

    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* sTarg=childsVp->node;
    			if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))){
    				//A $- B: A is the immediate right sister of B
    				std::cerr<<"S"<<((*words)[sTarg->lexical_head].word)<<"\n";
    				bool sisterCondition=false;
    				if (childsVp->previous!=0){
    					const CStateNode* leftSisterS=childsVp->previous->node;
    					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
    							if ((*words)[leftSisterS->lexical_head].word==g_word_order){
    								sisterCondition=true;
    								std::cerr<<"NN order"<<((*words)[leftSisterS->lexical_head].word)<<"\n";
    							}

    					}
    				}
    				std::cerr<<"-->"<<sisterCondition<<"\n";
    				if (!sisterCondition){
    					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
    					while(childsS!=0){
    						const CStateNode* vpChild=childsS->node;
    						if (CConstituent::clearTmp(vpChild->constituent.code())==PENN_CON_VP){
    							std::cerr<<"VP"<<((*words)[vpChild->lexical_head].word)<<"\n";
    							CStateNodeList* childsVps=vpChild->m_umbinarizedSubNodes;
    							while(childsVps!=0){
    								if ((*words)[childsVps->node->lexical_head].tag.code()==PENN_TAG_TO) {
//    									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
    									std::cerr<<"TO"<<((*words)[childsVps->node->lexical_head].word)<<"\n";
    									if (buildStanfordLink(STANFORD_DEP_XCOMP, sTarg->lexical_head, node.lexical_head)) {
    										addLinked(&node,sTarg);
    									    return true;
    									}
    								}
    								childsVps=childsVps->next;
    							}
    						}
    						childsS=childsS->next;
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;

    }

    //"ADJP < (S=target <, (VP <, TO))",
inline const bool & buildXComp2(const unsigned long &cons) {
       	if (cons==PENN_CON_ADJP){
       		CStateNodeList* childsADJP=node.m_umbinarizedSubNodes;
       		while(childsADJP!=0){
       			const CStateNode* sTarg=childsADJP->node;
       			if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))){
       				//A <, B 	B is the first child of A
       				CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
       				if (childsS!=0){//only first child
       					const CStateNode* vpChildS=childsS->node;
       					if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP){
       						CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
       						if (childsVp!=0){//only first child
       							if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)){
//       								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
       								if (buildStanfordLink(STANFORD_DEP_XCOMP, sTarg->lexical_head, node.lexical_head)) {
       									addLinked(&node,sTarg);
       									return true;
       								}
       							}
       						}
       					}
       				}

       			}
       			childsADJP=childsADJP->next;
       		}
       	}
       	return false;

       }

//"VP < (S=target !$- (NN < order) < (NP $+ NP|ADJP))",
inline const bool &buildXComp3(const unsigned long &cons) {
	if (cons==PENN_CON_VP){
		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
		while(childsVp!=0){
			const CStateNode* sTarg=childsVp->node;
			if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))){
				//A $- B: A is the immediate right sister of B
				bool sisterCondition=false;
				if (childsVp->previous!=0){
					const CStateNode* leftSisterS=childsVp->previous->node;
					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
						//CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
						//while(childsNN!=0){
							//const CStateNode* orderChild=childsNN->node;
							if ((*words)[leftSisterS->lexical_head].word==g_word_order){
								sisterCondition=true;
							}
							//childsNN=childsNN->next;
						//}
					}
				}
				if (sisterCondition){
					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
					while(childsS!=0){
						const CStateNode* npChildS=childsS->node;
						if (CConstituent::clearTmp(npChildS->constituent.code())==PENN_CON_NP){
							//A $+ B 	A is the immediate left sister of B
							if (childsS->next!=0){
								if (CConstituent::clearTmp(childsS->next->node->constituent.code())==PENN_CON_NP
										||CConstituent::clearTmp(childsS->next->node->constituent.code())==PENN_CON_ADJP){
//									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
									if (buildStanfordLink(STANFORD_DEP_XCOMP, sTarg->lexical_head, node.lexical_head)) {
										addLinked(&node,sTarg);
									    return true;
									}
								}
							}
						}
						childsS=childsS->next;
					}
				}
			}
			childsVp=childsVp->next;
		}
	}
	return false;

}

//"VP < (/^(?:VB|AUX)/ $+ (VP=target < VB < NP))",
inline const bool &buildXComp4(const unsigned long &cons) {
   	if (cons==PENN_CON_VP){
   		bool firstCondition=false;
   		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
   		while(childsVp!=0){
   			if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB)){
   				firstCondition=true;
   			}
   			childsVp=childsVp->next;
   		}
   		if (firstCondition && childsVp!=0){
   			//A $+ B 	A is the immediate left sister of B
   			if (childsVp->next!=0){
   				const CStateNode* vpTarg=childsVp->next->node;
   				if ((CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP) && (!isLinked(&node,vpTarg))){
   					CStateNodeList* childsVpTarg=vpTarg->m_umbinarizedSubNodes;
   					while(childsVpTarg!=0){
   						const CStateNode* vbChildVp=childsVpTarg->node;
   						if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)){
   							CStateNodeList* childsVb=vbChildVp->m_umbinarizedSubNodes;
   							while(childsVb!=0){
   								if (CConstituent::clearTmp(childsVb->node->constituent.code())==PENN_CON_NP){
//   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
   									if (buildStanfordLink(STANFORD_DEP_XCOMP, vpTarg->lexical_head, node.lexical_head)) {
   										addLinked(&node,vpTarg);
   										return true;
   									}
   								}
   								childsVb=childsVb->next;
   							}
   						}
   						childsVpTarg=childsVpTarg->next;
   					}
   				}

   			}
   		}
   	}
   	return false;

   }

//"VP < (SBAR=target < (S !$- (NN < order) < (VP < TO))) !> (VP < (VB|AUX < be)) ",
inline const bool & buildXComp5(const unsigned long &cons) {
	  bool fstCond=true;
	  if (cons==PENN_CON_VP){
		  CStateNodeList* childsFstVp=node.m_umbinarizedSubNodes;
		  while(childsFstVp!=0){
			  if (((*words)[childsFstVp->node->lexical_head].tag.code()==PENN_TAG_VERB) && ((*words)[childsFstVp->node->lexical_head].word==g_word_be)) {
				  fstCond=false;
			  }

			  childsFstVp=childsFstVp->next;
		  }
	  }
	  if (fstCond){
		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
		  while(childs!=0){
			  const CStateNode* head=childs->node;
			  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_VP){
				  CStateNodeList* childsHead=head->m_umbinarizedSubNodes;
				  while(childsHead!=0){
					  const CStateNode* targ=childsHead->node;
					  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_SBAR && !isLinked(&node,targ)){
						  CStateNodeList* childsTarg=targ->m_umbinarizedSubNodes;
						  while(childsTarg!=0){
							  if (CConstituent::clearTmp(childsTarg->node->constituent.code())==PENN_CON_S){
								  bool firstCondition=true; //!$- (NN < order)
								  bool secCondition=false;  //< (VP < TO)
								  if (childsTarg->previous!=0){
									  if ((*words)[childsTarg->previous->node->lexical_head].tag.code()==PENN_TAG_NOUN && ((*words)[childsTarg->previous->node->lexical_head].word==g_word_order)){
										  firstCondition=false;
									  }
								  }
								  if (firstCondition){
									  CStateNodeList* childsS=childsTarg->node->m_umbinarizedSubNodes;
									  while(childsS!=0){
										  if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
											  CStateNodeList* childsVpS=childsS->node->m_umbinarizedSubNodes;
											  while(childsVpS!=0){
												  if ((*words)[childsVpS->node->lexical_head].tag.code()==PENN_TAG_TO){
													  secCondition=true;
												  }
												  childsVpS=childsVpS->next;
											  }
										  }
										  childsS=childsS->next;
									  }
								  }
								  if (firstCondition && secCondition){
										if (buildStanfordLink(STANFORD_DEP_XCOMP, targ->lexical_head, head->lexical_head)) {
											//addLinked(vpNode,sTarg);
											addLinked(&node,targ); //I think this is not correct, in this specific case.
											return true;
										}
								  }
							  }
							  childsTarg=childsTarg->next;
						  }
					  }
					  childsHead=childsHead->next;
				  }
			  }
			  childs=childs->next;
		  }
	  }
	  return false;
}




//"VP < (S=target !$- (NN < order) <: NP) > VP",
//the second vp is the head. Not the first one, because the first one is the one located to the right of >

inline const bool & buildXComp6(const unsigned long &cons) {
	if (cons==PENN_CON_VP){
		CStateNodeList* childsfstVp=node.m_umbinarizedSubNodes;
		while(childsfstVp!=0){
			const CStateNode* vpNode=childsfstVp->node;
			if (CConstituent::clearTmp(vpNode->constituent.code())==PENN_CON_VP){
				CStateNodeList* childsVp=vpNode->m_umbinarizedSubNodes;
				while(childsVp!=0){
					const CStateNode* sTarg=childsVp->node;
					if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(vpNode,sTarg))) {
						bool firstCondition=true;
						//A $- B 	A is the immediate right sister of B
						if (childsVp->previous!=0){
							const CStateNode* leftSisterS=childsVp->previous->node;
							if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
								//CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
								//while(childsNN!=0){
									//const CStateNode* orderChild=childsNN->node;
									if ((*words)[leftSisterS->lexical_head].word==g_word_order){
										firstCondition=false;
									}
									//childsNN=childsNN->next;
								 //}
							 }
						}
						if (firstCondition){
							//S=target <: NP
							//A <: B 	B is the only child of A
							CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
							if (childsS!=0){
								if ((CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP) && (childsS->next ==0)){
//									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
									if (buildStanfordLink(STANFORD_DEP_XCOMP, sTarg->lexical_head, vpNode->lexical_head)) {
										//addLinked(vpNode,sTarg);
										addLinked(&node,sTarg); //I think this is not correct, in this specific case.
										return true;
									}
								}
							}
						}

					}
					childsVp=childsVp->next;
				}
			}
			childsfstVp=childsfstVp->next;
		}
	}
	return false;

}


//"(VP < (S=target < (VP < VBG ) !< NP !$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)]) !$-- /^:$/))",
inline const bool &buildXComp7(const unsigned long &cons) {
     	if (cons==PENN_CON_VP){
     		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		while(childsVp!=0){
     			const CStateNode* sTarg=childsVp->node;
     			if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && (!isLinked(&node,sTarg))) {
     				bool secondCondition=true;// !<NP
     				bool thirdCondition=true; //!$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)])
     				bool fourthCondition=true; //!$-- /^:$/
     				CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
     				while(childsS!=0){
     					if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP) {
     						secondCondition=false;
     					}
     					childsS=childsS->next;
     				}
     				if (secondCondition){
     					//A $-- B 	A is a right sister of B
     					CStateNodeList* leftSistersS=childsVp->previous;
     					while(leftSistersS!=0){
     						if ((*words)[leftSistersS->node->lexical_head].word==g_word_two_dots){
     							fourthCondition=false;
     						}
     						leftSistersS=leftSistersS->previous;
     					}
     					if (fourthCondition){
     						if (childsVp->previous!=0){
     							const CStateNode* commaLeftSisterS=childsVp->previous->node;
     							if ((*words)[commaLeftSisterS->lexical_head].word==g_word_comma){
     								//and now,
     								///^,$/ $- @NP
     								//or
     								///^,$/ $- (@PP $-- @NP )
     								//or
     								///^,$/ $- @NP $- (@ADVP $-- @NP)
     								bool firstInCondition=false;
     								bool secondInCondition=false;
     								bool thirdInCondition=false;
     								//A $- B. A is the immediate right sister of B

     								//In this part of the code I matched the @NP as NP, what is the exact difference, what is the head category?

     								if (childsVp->previous->previous!=0){
     									const CStateNode* leftSisterOfComma=childsVp->previous->previous->node;
     									if (CConstituent::clearTmp(leftSisterOfComma->constituent.code())==PENN_CON_NP){
     										firstInCondition=true; // I don't see the difference between matching just one @NP to matching @NP $- (@ADVP $-- @NP), but anyway, let's make the code.
     										if (childsVp->previous->previous->previous!=0){
     											const CStateNode* leftSisterOfNp=childsVp->previous->previous->previous->node;
     											if (CConstituent::clearTmp(leftSisterOfNp->constituent.code())==PENN_CON_ADVP){
     												CStateNodeList* leftSistersADVP=childsVp->previous->previous->previous->previous;
     												while(leftSistersADVP!=0){
     													if (CConstituent::clearTmp(leftSistersADVP->node->constituent.code())==PENN_CON_NP){
     														thirdInCondition=true;
     													}
     													leftSistersADVP=leftSistersADVP->next;
     												}
     											}
     										}
     									}
     									if (CConstituent::clearTmp(leftSisterOfComma->constituent.code())==PENN_CON_PP){
     										CStateNodeList* leftSistersPP=childsVp->previous->previous->previous;
     										while(leftSistersPP!=0){
     											if (CConstituent::clearTmp(leftSistersPP->node->constituent.code())==PENN_CON_NP){
     												secondInCondition=true;
     											}
     											leftSistersPP=leftSistersPP->next;
     										}

     									}

     								}



     								if (firstInCondition||secondInCondition||thirdInCondition){
     									thirdCondition=false;
     								}
     							}
     						}
     						if (thirdCondition){
     							childsS=sTarg->m_umbinarizedSubNodes;
     							while(childsS!=0){
     								const CStateNode* vpChildS=childsS->node;
     								if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP){
     									CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
     									while(childsVp!=0){
     										if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)) {
//     											CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
     											if (buildStanfordLink(STANFORD_DEP_XCOMP, sTarg->lexical_head, node.lexical_head)) {
     												//addLinked(vpNode,sTarg);
     												addLinked(&node,sTarg);
     												return true;
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
     			childsVp=childsVp->next;
     		}
     	}
     	return false;
     }

