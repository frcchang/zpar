

     //"VP < (@SBAR=target [ < (IN !< /^(?i:that|whether)$/) | <: (SINV <1 /^(?:VB|MD|AUX)/) | < (IN < that) < (RB|IN < so) ] )",

     bool advcl1(){
    	 if (node.constituent==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* sbarTarg=childsVp->node;
    			if ((sbarTarg->constituent==PENN_CON_SBAR) && !(isLinked(&node,sbarTarg))) {
    				bool firstCondition=false;
    				bool secondCondition=false;
    				bool thirdCondition=false;

    				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				while(childsSbar!=0){
    					const CStateNode* inChildSbar=childsSbar->node;
    					if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_NOUN){
    						CStateNodeList* childsIn=inChildSbar->m_umbinarizedSubNodes;
    						bool inCondition=true;
    						while(childsIn!=0){
    							if (((*words)[childsIn->node->lexical_head].word==g_word_that) ||((*words)[childsIn->node->lexical_head].word==g_word_whether)){
    								inCondition=false;
    							}
    							childsIn=childsIn->next;
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
    						if (sinvChild->constituent==PENN_CON_SBAR && childsSbar->next==0){
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
    							CStateNodeList* childsIn=childsSbar->node->m_umbinarizedSubNodes;
    							while(childsIn!=0){
    								if ((*words)[childsIn->node->lexical_head].word==g_word_that){
    									firstInCond=true;
    								}
    								childsIn=childsIn->next;
    							}
    							childsSbar=childsSbar->next;
    						}
    					}
    					if (firstInCond){
    						childsSbar=sbarTarg->m_umbinarizedSubNodes;
    						while(childsSbar!=0){
    							const CStateNode* rbInChildSbar=childsSbar->node;
    							if ((*words)[rbInChildSbar->lexical_head].tag.code()==PENN_TAG_IN ||
    									(*words)[rbInChildSbar->lexical_head].tag.code()==PENN_TAG_ADVERB){

    								CStateNodeList* childsRbin=rbInChildSbar->m_umbinarizedSubNodes;
    								while(childsRbin!=0){
    									if ((*words)[childsRbin->node->lexical_head].word==g_word_so){
    										thirdCondition=true;
    									}
    									childsRbin=childsRbin->next;
    								}
    							}
    							childsSbar=childsSbar->next;
    						}
    					}
    				}



    				if (firstCondition || secondCondition || thirdCondition){
    					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
    					if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
        bool advcl2(){
       	 if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_SINV){
       		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		 while(childs!=0){
       			 const CStateNode* sbarTarg=childs->node;
       			 if (sbarTarg->constituent==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
       				 bool firstCond=false; //<, (IN !< /^(?i:that|whether)$/ !$+ (NN < order))
       				 bool secCond=true; //!$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/
       				 bool thirdCond=true; //!$+ VP

       				 if (childs->next!=0){
       					 if (childs->next->node->constituent==PENN_CON_VP){
       						 thirdCond=false;
       					 }
       				 }
       				 if (thirdCond){
       					 CStateNodeList* leftSisters=childs->previous;
       					 while(leftSisters!=0){
       						 const CStateNode* lSis=leftSisters->node;
       						 if (lSis->constituent==PENN_CON_CONJP ||lSis->constituent==PENN_CON_INTJ ||lSis->constituent==PENN_CON_PP|| (*words)[lSis->lexical_head].tag.code()==PENN_TAG_CC||
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
       								CStateNodeList* childsIn=inChild->m_umbinarizedSubNodes;
       								while(childsIn!=0){
       									if ((*words)[childsIn->node->lexical_head].word==g_word_whether||(*words)[childsIn->node->lexical_head].word==g_word_that){
       										secInCond=false;
       									}
       									childsIn=childsIn->next;
       								}
       								if (secInCond){
       									if (childs->next!=0){
       										if ((*words)[childs->next->node->lexical_head].tag.code()==PENN_TAG_NOUN){
       											CStateNodeList* childsNN=childs->next->node->m_umbinarizedSubNodes;
       											while(childsNN!=0){
       												if ((*words)[childsNN->node->lexical_head].word==g_word_order){
       													thirdInCond=false;
       												}
       												childsNN=childsNN->next;
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
       					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
       					 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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

             bool advcl3(){
            	 if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_SINV){
            		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
            		 while(childs!=0){
            			 const CStateNode* sbarTarg=childs->node;
            			 if (sbarTarg->constituent==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
            				 bool firstCond=false; //<, (IN !< /^(?i:that|whether)$/ !$+ (NN < order))
            				 bool secCond=true; //!$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/
            				 //bool thirdCond=true; //!$+ VP (this is from advcl2)

            				 CStateNodeList* leftSisters=childs->previous;
            				 while(leftSisters!=0){
            					 const CStateNode* lSis=leftSisters->node;
            					 if (lSis->constituent==PENN_CON_CONJP ||lSis->constituent==PENN_CON_INTJ ||lSis->constituent==PENN_CON_PP|| (*words)[lSis->lexical_head].tag.code()==PENN_TAG_CC||
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
            								CStateNodeList* childsIn=inChild->m_umbinarizedSubNodes;
            								while(childsIn!=0){
            									if ((*words)[childsIn->node->lexical_head].word==g_word_whether||(*words)[childsIn->node->lexical_head].word==g_word_that){
            										secInCond=false;
            									}
            									childsIn=childsIn->next;
            								}
            								if (secInCond){
            									if (childs->next!=0){
            										if ((*words)[childs->next->node->lexical_head].tag.code()==PENN_TAG_NOUN){
            											CStateNodeList* childsNN=childs->next->node->m_umbinarizedSubNodes;
            											while(childsNN!=0){
            												if ((*words)[childsNN->node->lexical_head].word==g_word_order){
            													thirdInCond=false;
            												}
            												childsNN=childsNN->next;
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
            					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
            					 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
    bool advcl4(){

   	if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_SINV){
   		CStateNodeList* childsSSqS=node.m_umbinarizedSubNodes;
   		while(childsSSqS!=0){
   			const CStateNode* sbarTarg=childsSSqS->node;
   			if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))){
   				bool secondCondition=true;
   				bool thirdCondition=false;
   				bool fourthCondition=false;

   				if (childsSSqS->next!=0){
   					if (childsSSqS->next->node->constituent==PENN_CON_VP){
   						secondCondition=false;
   					}
   					else if ((*words)[childsSSqS->next->node->lexical_head].word==g_word_comma){
   						thirdCondition=true;
   					}
   				}
   				if (secondCondition && thirdCondition){
   					CStateNodeList* rightSisters=childsSSqS;
   					while(rightSisters!=0){
   						if (rightSisters->node->constituent==PENN_CON_NP){
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
   							CStateNodeList* childsIn=inChild->m_umbinarizedSubNodes;
   							while(childsIn!=0){
   								if (((*words)[childsIn->node->lexical_head].word==g_word_that) ||((*words)[childsIn->node->lexical_head].word==g_word_whether)){
   									secInCondition=false;
   								}
   								childsIn=childsIn->next;
   							}
   							if (childsSbar->next!=0){
   								const CStateNode* nnRightSis=childsSbar->next->node;
   								if ((*words)[nnRightSis->lexical_head].tag.code()==PENN_TAG_NOUN){
   									CStateNodeList* childsNN=nnRightSis->m_umbinarizedSubNodes;
   									while(childsNN!=0){
   										if ((*words)[childsNN->node->lexical_head].word==g_word_order){
   											thirdInCondition=false;
   										}
   										childsNN=childsNN->next;
   									}
   								}
   							}


   							if (secInCondition && thirdInCondition){
   								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   								if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
      bool advcl5(){
     	 if (node.constituent==PENN_CON_SBARQ){
     		 CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
     		 while(childsSbarq!=0){
     			 const CStateNode* sbarTarg=childsSbarq->node;
     			 if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))){ //there are no subclasses, such as tmp or adv.
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
     					 if (rightSister->constituent==PENN_CON_S||rightSister->constituent==PENN_CON_SQ||rightSister->constituent==PENN_CON_SBARQ){
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
     	    							 CStateNodeList* childsNN=nnSister->m_umbinarizedSubNodes;
     	    							 while(childsNN!=0){
     	    	    						 if ((*words)[childsNN->node->lexical_head].word==g_word_order){
     	    	    							 thirdInCondition=false;
     	    	    						 }
     	    								 childsNN=childsNN->next;
     	    							 }
     	    						 }

     							 }
     							 if (thirdInCondition){
     								 CStateNodeList* childsIn=fstChildSbar->m_umbinarizedSubNodes;
     								 while(childsIn!=0){
     									 if (((*words)[childsIn->node->lexical_head].word==g_word_that)||
     											 ((*words)[childsIn->node->lexical_head].word==g_word_whether)){
     										 secInCondition=true;
     									 }
     									 childsIn=childsIn->next;
     								 }
     							 }

     							 if (secInCondition && thirdInCondition){
     								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
     								 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
      bool advcl6(){
     	 if (node.constituent==PENN_CON_VP){
     		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		 while(childsVp!=0){
     			 const CStateNode* sbarTarg=childsVp->node;
     			 if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))) {
     				 bool secondCondition=true;
     				 CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
     				 while(childsSbar!=0){
     					 const CStateNode* schild=childsSbar->node;
     					 if (schild->constituent==PENN_CON_S){
     						 CStateNodeList* childsS=schild->m_umbinarizedSubNodes;
     						 while(childsS!=0){
     							 if (childsS->node->constituent==PENN_CON_VP){
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
     					 if (whChild->constituent==PENN_CON_WHNP || whChild->constituent==PENN_CON_WHADVP){
     						 CStateNodeList* childsWh=whChild->m_umbinarizedSubNodes;
     						 while(childsWh!=0){
     							 const CStateNode* wrbChild=childsWh->node;
     							 if (((*words)[wrbChild->lexical_head].tag.code()==PENN_TAG_WRB)) {
     								 CStateNodeList* childsWrb=wrbChild->m_umbinarizedSubNodes;
     								 bool inCondition=true;
     								 while(childsWrb!=0){
     									 if ((*words)[childsWrb->node->lexical_head].word==g_word_how){
     										 inCondition=false;
     									 }
     									 childsWrb=childsWrb->next;
     								 }
     								 if (inCondition){
     									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
     									 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
     bool advcl7(){
    	 if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ){
    		 CStateNodeList* childsSSQ=node.m_umbinarizedSubNodes;
    		 while(childsSSQ!=0){
    			 const CStateNode* sbarTarg=childsSSQ->node;
    			 if (sbarTarg->constituent==PENN_CON_SBAR || !(isLinked(&node,sbarTarg))){
    				 CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				 bool secondCondition=true;
    				 while(childsSbar!=0){
    					 const CStateNode* sChildSbar=childsSbar->node;
    					 if (sChildSbar->constituent==PENN_CON_S){
    						 CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
    						 while(childsS!=0){
    							 if (childsS->node->constituent==PENN_CON_VP){
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
    						 if (whChildS->constituent==PENN_CON_WHADJP ||whChildS->constituent==PENN_CON_WHNP){
    							 CStateNodeList* childsWh=whChildS->m_umbinarizedSubNodes;
    							 while(childsWh!=0){
    								 const CStateNode* wrbChildwh=childsWh->node;
    								 if (((*words)[wrbChildwh->lexical_head].tag.code()==PENN_TAG_WRB)) {
    									 bool howCond=true;
    									 CStateNodeList* childsWrb=wrbChildwh->m_umbinarizedSubNodes;
    									 while(childsWrb!=0){
    										 if ((*words)[childsWrb->node->lexical_head].word==g_word_how){
    											 howCond=false;
    										 }
    										 childsWrb=childsWrb->next;
    									 }
    									 if (howCond){
    										 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
    										 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
    bool advcl8(){
   	 if (node.constituent==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sbarTarg=childsS->node;
   			 if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))) {
   				 CStateNodeList* rightSistersSbar=childsS;
   				 bool firstCondition=false;
   				 while(rightSistersSbar!=0){
   					 if (rightSistersSbar->node->constituent==PENN_CON_NP){
   						 firstCondition=true;
   					 }
   					 rightSistersSbar=rightSistersSbar->next;
   				 }
   				 if (firstCondition){
   					 rightSistersSbar=childsS;
   					 while(rightSistersSbar!=0){
   						 if (rightSistersSbar->node->constituent==PENN_CON_VP){
   							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   							 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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
    bool advcl9(){
   	 if (node.constituent==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sTarg=childsS->node;
   			 if (sTarg->constituent==PENN_CON_S && !(isLinked(&node,sTarg))){
   				 bool secondCondition=false;
   				 if (childsS->next!=0){
   					 const CStateNode* commaRightSister=childsS->next->node;
   					 if ((*words)[commaRightSister->lexical_head].word==g_word_comma){
   						 if (childsS->next->next!=0){
   							 CStateNodeList* rightSisters=childsS->next->next;
   							 while(rightSisters!=0){
   								 if (rightSisters->node->constituent==PENN_CON_NP){
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
   						 if (vpChildS->constituent==PENN_CON_VP){
   							 CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
   							 while(childsVp!=0){
   								 if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
   									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   									 if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
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

