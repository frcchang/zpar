

     //"VP < (@SBAR=target [ < (IN !< /^(?i:that|whether)$/) | <: (SINV <1 /^(?:VB|MD|AUX)/) | < (IN < that) < (RB|IN < so) ] )",

inline const bool &advcl1(const unsigned long &cons){
    	 if (cons==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* sbarTarg=childsVp->node;
    			if ((CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR) && !(isLinked(&node,sbarTarg))) {
    				bool firstCondition=false;
    				bool secondCondition=false;
    				bool thirdCondition=false;

    				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				while(childsSbar!=0){
    					const CStateNode* inChildSbar=childsSbar->node;
    					if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_NOUN){
    						bool inCondition=true;
    						if (((*words)[inChildSbar->lexical_head].word==g_word_that) ||((*words)[inChildSbar->lexical_head].word==g_word_whether)){
    							inCondition=false;
    						}
    						firstCondition=inCondition;
    					}
    					childsSbar=childsSbar->next;
    				}
    				if (!firstCondition){
    					childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					//<: (SINV <1 /^(?:VB|MD|AUX)/)
    					if(childsSbar!=0){
    						const CStateNode* sinvChild=childsSbar->node;
    						if (CConstituent::clearTmp(sinvChild->constituent.code())==PENN_CON_SBAR && childsSbar->next==0){
    							//I will assume that this <1 is like <,
    							CStateNodeList* childsSinv=sinvChild->m_umbinarizedSubNodes;
    							if(childsSinv!=0){

    							//PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
    								if (((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
    										((*words)[childsSinv->node->lexical_head].tag.code()==PENN_TAG_MD)){
    									secondCondition=true;
    								}
    								//childsSinv=childsSinv->next;
    							}
    						}
    					}
    				}
    				if (!firstCondition && !secondCondition){
    					//< (IN < that) < (RB|IN < so) ]
    					childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					bool firstInCond=false;
    					while(childsSbar!=0){
    						if ((*words)[childsSbar->node->lexical_head].tag.code()==PENN_TAG_IN){
    							if ((*words)[childsSbar->node->lexical_head].word==g_word_that){
    								firstInCond=true;
    							}
    						}
    						childsSbar=childsSbar->next;
    					}
    					if (firstInCond){
    						childsSbar=sbarTarg->m_umbinarizedSubNodes;
    						while(childsSbar!=0){
    							const CStateNode* rbInChildSbar=childsSbar->node;
    							if ((*words)[rbInChildSbar->lexical_head].tag.code()==PENN_TAG_IN ||
    									(*words)[rbInChildSbar->lexical_head].tag.code()==PENN_TAG_ADVERB){
    								if ((*words)[rbInChildSbar->lexical_head].word==g_word_so){
    									thirdCondition=true;
    								}
    							}
    							childsSbar=childsSbar->next;
    						}
    					}
    				}



    				if (firstCondition || secondCondition || thirdCondition){
//    					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
    					if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
    					 addLinked(&node,sbarTarg);
    					 return true;
    					}
    				}

    			}
    			childsVp=childsVp->next;
    		}
    	 }
    	 return false;
     }


     //"S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/ !$+ VP)",
inline const bool &advcl2(const unsigned long &cons){
       	 if (cons==PENN_CON_S || cons==PENN_CON_SQ || cons==PENN_CON_SINV){
       		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		 while(childs!=0){
       			 const CStateNode* sbarTarg=childs->node;
       			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
       				 bool firstCond=false; //<, (IN !< /^(?i:that|whether)$/ !$+ (NN < order))
       				 bool secCond=true; //!$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/
       				 bool thirdCond=true; //!$+ VP

       				 if (childs->next!=0){
       					 if (CConstituent::clearTmp(childs->next->node->constituent.code())==PENN_CON_VP){
       						 thirdCond=false;
       					 }
       				 }
       				 if (thirdCond){
       					 CStateNodeList* leftSisters=childs->previous;
       					 while(leftSisters!=0){
       						 const CStateNode* lSis=leftSisters->node;
       						 if (CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_CONJP ||CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_INTJ
       								 ||CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_PP|| (*words)[lSis->lexical_head].tag.code()==PENN_TAG_CC||
       								 (*words)[lSis->lexical_head].word==g_word_comma ||(*words)[lSis->lexical_head].word==g_word_quotes){
       							 secCond=false;
       						 }
       						 leftSisters=leftSisters->previous;
       					 }
       					 if (secCond){
       						 CStateNodeList* childsTarg=sbarTarg->m_umbinarizedSubNodes;
       						if(childsTarg!=0){
       							const CStateNode* inChild=childsTarg->node;
       							if ((*words)[inChild->lexical_head].tag.code()==PENN_TAG_IN){
       								bool secInCond=true;
       								bool thirdInCond=true;
       								if ((*words)[inChild->lexical_head].word==g_word_whether||(*words)[inChild->lexical_head].word==g_word_that){
       									secInCond=false;
       								}
       								CStateNodeList* childsIn=inChild->m_umbinarizedSubNodes;
       								
       								if (secInCond){
       									if (childs->next!=0){
       										if ((*words)[childs->next->node->lexical_head].tag.code()==PENN_TAG_NOUN){
       											if ((*words)[childs->next->node->lexical_head].word==g_word_order){
       												thirdInCond=false;
       											}

  
       									    }
       									}
       								}
       								if (secInCond && thirdInCond){
       									firstCond=true;
       								}
       							}
       						}
       					 }
       				 }

       				 if (firstCond && secCond && thirdCond){
//       					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
       					 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
       						 addLinked(&node,sbarTarg);
       					     return true;
       					 }
       				 }
       			 }
       			 childs=childs->next;
       		 }
       	 }
       	 return false;
        }

       // if ((nnChildNp->type==CStateNode::LEAF)
   	  // && (nnChildNp->lexical_head==npwhnpTarg->lexical_head) //<#



        //"S|SQ|SINV < (SBAR|SBAR-TMP=target <2 (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?$).*$/)",

inline const bool &advcl3(const unsigned long &cons){
            	 if (cons==PENN_CON_S || cons==PENN_CON_SQ || cons==PENN_CON_SINV){
            		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
            		 while(childs!=0){
            			 const CStateNode* sbarTarg=childs->node;
            			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
            				 bool firstCond=false; //<, (IN !< /^(?i:that|whether)$/ !$+ (NN < order))
            				 bool secCond=true; //!$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/
            				 //bool thirdCond=true; //!$+ VP (this is from advcl2)

            				 CStateNodeList* leftSisters=childs->previous;
            				 while(leftSisters!=0){
            					 const CStateNode* lSis=leftSisters->node;
            					 if (CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_CONJP
            							 ||CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_INTJ
            							 ||CConstituent::clearTmp(lSis->constituent.code())==PENN_CON_PP|| (*words)[lSis->lexical_head].tag.code()==PENN_TAG_CC||
            							 (*words)[lSis->lexical_head].word==g_word_comma ||(*words)[lSis->lexical_head].word==g_word_quotes){
            						 secCond=false;
            					 }
            					 leftSisters=leftSisters->previous;
            			      }
            				  if (secCond){
            				  CStateNodeList* childsTarg=sbarTarg->m_umbinarizedSubNodes;
            				  	  if(childsTarg!=0){
            				  		  if (childsTarg->next!=0){ //<2
            							const CStateNode* inChild=childsTarg->next->node;
            							if ((*words)[inChild->lexical_head].tag.code()==PENN_TAG_IN){
            								bool secInCond=true;
            								bool thirdInCond=true;
            								if ((*words)[inChild->lexical_head].word==g_word_whether||(*words)[inChild->lexical_head].word==g_word_that){
            									secInCond=false;
            								}
            								
            								if (secInCond){
            									if (childs->next!=0){
            										if ((*words)[childs->next->node->lexical_head].tag.code()==PENN_TAG_NOUN){
            											if ((*words)[childs->next->node->lexical_head].word==g_word_order){
            												thirdInCond=false;
            											}
            									    }
            									}
            								}
            								if (secInCond && thirdInCond){
            									firstCond=true;
            								}
            							}
            						}
            					}
            				  }
            				 //}

            				 if (firstCond && secCond){
//            					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
            					 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
            						 addLinked(&node,sbarTarg);
            					     return true;
            					 }
            				 }
            			 }
            			 childs=childs->next;
            		 }
            	 }
            	 return false;
             }

//"S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$+ @VP $+ /^,$/ $++ @NP)"
inline const bool &advcl4(const unsigned long &cons){

   	if (cons==PENN_CON_S || cons==PENN_CON_SQ || cons==PENN_CON_SINV){
   		CStateNodeList* childsSSqS=node.m_umbinarizedSubNodes;
   		while(childsSSqS!=0){
   			const CStateNode* sbarTarg=childsSSqS->node;
   			if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))){
   				bool secondCondition=true;
   				bool thirdCondition=false;
   				bool fourthCondition=false;

   				if (childsSSqS->next!=0){
   					if (CConstituent::clearTmp(childsSSqS->next->node->constituent.code())==PENN_CON_VP){
   						secondCondition=false;
   					}
   					else if ((*words)[childsSSqS->next->node->lexical_head].word==g_word_comma){
   						thirdCondition=true;
   					}
   				}
   				if (secondCondition && thirdCondition){
   					CStateNodeList* rightSisters=childsSSqS;
   					while(rightSisters!=0){
   						if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){
   							fourthCondition=true;
   						}
   						rightSisters=rightSisters->next;
   					}
   				}

   				if (secondCondition && thirdCondition && fourthCondition){
   					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
   					if (childsSbar->next!=0){
   						const CStateNode* inChild=childsSbar->node;
   						if ((*words)[inChild->lexical_head].tag.code()==PENN_TAG_IN){
   							//(IN !< /^(?i:that|whether)$/ !$+ (NN < order))
   							bool secInCondition=true;
   							bool thirdInCondition=true;
   							if (((*words)[inChild->lexical_head].word==g_word_that) ||((*words)[inChild->lexical_head].word==g_word_whether)){
   								secInCondition=false;
   							}
   							if (childsSbar->next!=0){
   								const CStateNode* nnRightSis=childsSbar->next->node;
   								if ((*words)[nnRightSis->lexical_head].tag.code()==PENN_TAG_NOUN){
   									if ((*words)[nnRightSis->lexical_head].word==g_word_order){
   										thirdInCondition=false;   										
   									}
   								}
   							}


   							if (secInCondition && thirdInCondition){
//   								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   								if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
   									addLinked(&node,sbarTarg);
   								   	return true;
   								}
   							}
   						}
   					}
   				}

   			}
   			childsSSqS=childsSSqS->next;
   		}
   	}
   	return false;
    }


    //"SBARQ < (SBAR|SBAR-TMP|SBAR-ADV=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) $+ /^,$/ $++ @SQ|S|SBARQ)",
inline const bool &advcl5(const unsigned long &cons){
     	 if (cons==PENN_CON_SBARQ){
     		 CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
     		 while(childsSbarq!=0){
     			 const CStateNode* sbarTarg=childsSbarq->node;
     			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))){ //there are no subclasses, such as tmp or adv.
     				 bool secondCondition=false;
     				 bool thirdCondition=false;

     				 CStateNodeList* rightSistersSbr=childsSbarq;
     				 int cont=0;
     				 while(rightSistersSbr!=0){
     					 const CStateNode* rightSister=rightSistersSbr->node;
     					 if (cont==0){
     						 if ((*words)[rightSister->lexical_head].word==g_word_comma){
     							 secondCondition=true;
     						 }
     					 }
     					 if (CConstituent::clearTmp(rightSister->constituent.code())==PENN_CON_S
     							 ||CConstituent::clearTmp(rightSister->constituent.code())==PENN_CON_SQ
     							 ||CConstituent::clearTmp(rightSister->constituent.code())==PENN_CON_SBARQ){
     						 thirdCondition=true;
     					 }
     					 rightSistersSbr=rightSistersSbr->next;
     					 cont++;
     				 }

     				 if (secondCondition && thirdCondition){
     					 CStateNodeList* childsSbarTarg=sbarTarg->m_umbinarizedSubNodes;
     					 if (childsSbarTarg!=0){
     						 const CStateNode* fstChildSbar=childsSbarTarg->node;
     						 if (((*words)[fstChildSbar->lexical_head].tag.code()==PENN_TAG_IN)) {
     							 bool secInCondition=true;
     							 bool thirdInCondition=true;
     							 //<, (IN !< /^(?i:that|whether)$/ !$+ (NN < order))

     							 if (childsSbarTarg->next!=0){ //$+
     								 const CStateNode* nnSister=childsSbarTarg->next->node;
     	    						 if (((*words)[nnSister->lexical_head].tag.code()==PENN_TAG_NOUN)) {
     	    							if ((*words)[nnSister->lexical_head].word==g_word_order){
     	    								thirdInCondition=false;
     	    							}
     	    						 }

     							 }
     							 if (thirdInCondition){
     								if (((*words)[fstChildSbar->lexical_head].word==g_word_that)||
     								    ((*words)[fstChildSbar->lexical_head].word==g_word_whether)){
     									secInCondition=true;
     								}
     							 }

     							 if (secInCondition && thirdInCondition){
//     								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
     								 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
     									 addLinked(&node,sbarTarg);
     								     return true;
     								 }
     							 }
     						 }
     					 }
     				 }
     			 }
     			 childsSbarq=childsSbarq->next;
     		 }
     	 }
     	 return false;
      }

      //"VP < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))", // added the (S < (VP <TO)) part so that "I tell them how to do so" doesn't get a
inline const bool &advcl6(const unsigned long &cons){
     	 if (cons==PENN_CON_VP){
     		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		 while(childsVp!=0){
     			 const CStateNode* sbarTarg=childsVp->node;
     			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))) {
     				 bool secondCondition=true;
     				 CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
     				 while(childsSbar!=0){
     					 const CStateNode* schild=childsSbar->node;
     					 if (CConstituent::clearTmp(schild->constituent.code())==PENN_CON_S){
     						 CStateNodeList* childsS=schild->m_umbinarizedSubNodes;
     						 while(childsS!=0){
     							 if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
     								 CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
     								 while(childsVp!=0){
     									 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)) {
     										 secondCondition=false;
     									 }
     									 childsVp=childsVp->next;
     								 }
     							 }
     							 childsS=childsS->next;
     						 }
     					 }
     					 childsSbar=childsSbar->next;
     				 }
     				 childsSbar=sbarTarg->m_umbinarizedSubNodes;
     				 if (childsSbar!=0){ //first child. //<, (WHADVP|WHNP < (WRB !< /^(?i:how)$/))
     					 const CStateNode* whChild=childsSbar->node;
     					 if (CConstituent::clearTmp(whChild->constituent.code())==PENN_CON_WHNP
     							 || CConstituent::clearTmp(whChild->constituent.code())==PENN_CON_WHADVP){
     						 CStateNodeList* childsWh=whChild->m_umbinarizedSubNodes;
     						 while(childsWh!=0){
     							 const CStateNode* wrbChild=childsWh->node;
     							 if (((*words)[wrbChild->lexical_head].tag.code()==PENN_TAG_WRB)) {
     								 bool inCondition=true;
     								if ((*words)[wrbChild->lexical_head].word==g_word_how){
     									inCondition=false;
     								}
     								 if (inCondition){
//     									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
     									 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
     										 addLinked(&node,sbarTarg);
     									     return true;
     									 }
     								 }
     							 }
     							 childsWh=childsWh->next;
     						 }
     					 }
     				 }


     			 }
     			 childsVp=childsVp->next;
     		 }
     	 }
     	 return false;
      }

//"S|SQ < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))",
inline const bool &advcl7(const unsigned long &cons){
    	 if (cons==PENN_CON_S || cons==PENN_CON_SQ){
    		 CStateNodeList* childsSSQ=node.m_umbinarizedSubNodes;
    		 while(childsSSQ!=0){
    			 const CStateNode* sbarTarg=childsSSQ->node;
    			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR || !(isLinked(&node,sbarTarg))){
    				 CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				 bool secondCondition=true;
    				 while(childsSbar!=0){
    					 const CStateNode* sChildSbar=childsSbar->node;
    					 if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
    						 CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
    						 while(childsS!=0){
    							 if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
    								 CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
    								 while(childsVp!=0){
    									 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)) {
    										 secondCondition=false;
    									 }
    									 childsVp=childsVp->next;
    								 }
    							 }
    							 childsS=childsS->next;
    						 }
    					 }
    					 childsSbar=childsSbar->next;
    				 }
    				 if (secondCondition){
    					 childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					 if (childsSbar!=0){
    						 //<, (WHADVP|WHNP < (WRB !< /^(?i:how)$/))
    						 const CStateNode* whChildS=childsSbar->node;
    						 if (CConstituent::clearTmp(whChildS->constituent.code())==PENN_CON_WHADJP
    								 ||CConstituent::clearTmp(whChildS->constituent.code())==PENN_CON_WHNP){
    							 CStateNodeList* childsWh=whChildS->m_umbinarizedSubNodes;
    							 while(childsWh!=0){
    								 const CStateNode* wrbChildwh=childsWh->node;
    								 if (((*words)[wrbChildwh->lexical_head].tag.code()==PENN_TAG_WRB)) {
    									 bool howCond=true;
    									 if ((*words)[wrbChildwh->lexical_head].word==g_word_how){
    										 howCond=false;
    									 }
    									 if (howCond){
//    										 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
    										 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
    											 addLinked(&node,sbarTarg);
    										     return true;
    										 }
    									 }
    								 }
    								 childsWh=childsWh->next;
    							 }
    						 }

    					 }
    				 }

    			 }

    			 childsSSQ=childsSSQ->next;
    		 }
    	 }
    	 return false;
     }

//"@S < (@SBAR=target $++ @NP $++ @VP)",
inline const bool &advcl8(const unsigned long &cons){
   	 if (cons==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sbarTarg=childsS->node;
   			 if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))) {
   				 CStateNodeList* rightSistersSbar=childsS;
   				 bool firstCondition=false;
   				 while(rightSistersSbar!=0){
   					 if (CConstituent::clearTmp(rightSistersSbar->node->constituent.code())==PENN_CON_NP){
   						 firstCondition=true;
   					 }
   					 rightSistersSbar=rightSistersSbar->next;
   				 }
   				 if (firstCondition){
   					 rightSistersSbar=childsS;
   					 while(rightSistersSbar!=0){
   						 if (CConstituent::clearTmp(rightSistersSbar->node->constituent.code())==PENN_CON_VP){
//   							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   							 if (buildStanfordLink(STANFORD_DEP_ADVCL, sbarTarg->lexical_head, node.lexical_head)) {
   								 addLinked(&node,sbarTarg);
   							     return true;
   							 }
   						 }
   						 rightSistersSbar=rightSistersSbar->next;
   					 }
   				 }
   			 }
   			 childsS=childsS->next;
   		 }
   	 }
   	 return false;
    }


    //"@S < (@S=target < (VP < TO) $+ (/^,$/ $++ @NP))",
inline const bool &advcl9(const unsigned long &cons){
   	 if (cons==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sTarg=childsS->node;
   			 if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && !(isLinked(&node,sTarg))){
   				 bool secondCondition=false;
   				 if (childsS->next!=0){
   					 const CStateNode* commaRightSister=childsS->next->node;
   					 if ((*words)[commaRightSister->lexical_head].word==g_word_comma){
   						 if (childsS->next->next!=0){
   							 CStateNodeList* rightSisters=childsS->next->next;
   							 while(rightSisters!=0){
   								 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){
   									 secondCondition=true;
   								 }
   								 rightSisters=rightSisters->next;
   							 }
   						 }
   					 }
   				 }
   				 if (secondCondition){
   					 CStateNodeList* childsSIn=sTarg->m_umbinarizedSubNodes;
   					 while(childsSIn!=0){
   						 const CStateNode* vpChildS=childsSIn->node;
   						 if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP){
   							 CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
   							 while(childsVp!=0){
   								 if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
//   									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   									 if (buildStanfordLink(STANFORD_DEP_ADVCL, sTarg->lexical_head, node.lexical_head)) {
   										 addLinked(&node,sTarg);
   									     return true;
   									 }
   								 }
   								 childsVp=childsVp->next;
   							 }
   						 }
   						 childsSIn=childsSIn->next;
   					 }
   				 }

   			 }
   			 childsS=childsS->next;
   		 }

   	 }
   	 return false;
    }

