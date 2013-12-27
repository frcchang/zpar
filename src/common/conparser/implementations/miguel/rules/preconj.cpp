
//"NP|NP-TMP|NP-ADV|NX|NML < (PDT|CC|DT=target < /^(?i:either|neither|both)$/ $++ CC)",
    inline const bool &preconj1(const unsigned long &cons){
     	if (cons==PENN_CON_NP ||cons==PENN_CON_NX){
     		CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
     		while(childsNp!=0){
     			const CStateNode* pdtTarg=childsNp->node;
     			if ((((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_PDT) || ((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_CC) || ((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_DT)) && !(isLinked(&node, pdtTarg))){
     				//CStateNodeList* childsPdt=pdtTarg->m_umbinarizedSubNodes;
     				//while(childsPdt!=0){
     					if (((*words)[pdtTarg->lexical_head].word==g_word_either)||((*words)[pdtTarg->lexical_head].word==g_word_neither)||((*words)[pdtTarg->lexical_head].word==g_word_both)) {
     						CStateNodeList* rightSistersPdt=childsNp;
     						while(rightSistersPdt!=0){
     							if ((*words)[rightSistersPdt->node->lexical_head].tag.code()==PENN_TAG_CC) {
//     								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
         							if (buildStanfordLink(STANFORD_DEP_PRECONJ, pdtTarg->lexical_head, node.lexical_head)) {
         								addLinked(&node,pdtTarg);
         								return true;
         							}
     								
     							}
     							rightSistersPdt=rightSistersPdt->next;
     						}
     						
     					}
     					//childsPdt=childsPdt->next;	
     				//}
     			
     		}
     		childsNp=childsNp->next;
     	}
     }
     return false;
    }

     
     //"NP|NP-TMP|NP-ADV|NX|NML < (CONJP=target < (RB < /^(?i:not)$/) < (RB|JJ < /^(?i:only|merely|just)$/) $++ CC|CONJP)",
    inline const bool & preconj2(const unsigned long &cons){
    	 if (cons==PENN_CON_NP || cons==PENN_CON_NX){
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 const CStateNode* conjpTarg=childsNp->node;
    			 if (CConstituent::clearTmp(conjpTarg->constituent.code())==PENN_CON_CONJP && !(isLinked(&node,conjpTarg))){
    				 bool secCond=false;
    				 bool thirdCond=false;
    				 bool fourthCond=false;
    				 CStateNodeList* rightSisters=childsNp->next;
    				 while(rightSisters!=0){
    					 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_CONJP ||(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						 fourthCond=true;
    					 }
    					 rightSisters=rightSisters->next;
    				 }
    				 if (fourthCond){
    					 CStateNodeList* childsConjp=conjpTarg->m_umbinarizedSubNodes;
    					 while(childsConjp!=0){
    						 //PENN_TAG_ADJECTIVE
    						 if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADVERB||(*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE){
    							 //CStateNodeList* childsRB=childsConjp->node->m_umbinarizedSubNodes;
    							 //while(childsRB!=0){
    								 if (((*words)[childsConjp->node->lexical_head].word==g_word_not)||((*words)[childsConjp->node->lexical_head].word==g_word_merely)||((*words)[childsConjp->node->lexical_head].word==g_word_just)){
    									 secCond=true;
    								 }
    								// childsRB=childsRB->next;
    							 //}
    						 }
    						 childsConjp=childsConjp->next;
    					 }
    					 if (secCond){
    						 childsConjp=conjpTarg->m_umbinarizedSubNodes;
    						 while(childsConjp!=0){
    							 if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADVERB){
    								 //CStateNodeList* childsRB=childsConjp->node->m_umbinarizedSubNodes;
    								 //while(childsRB!=0){
    									 if (((*words)[childsConjp->node->lexical_head].word==g_word_not)){
    										 thirdCond=true;
    									 }
    									// childsRB=childsRB->next;
    								 //}
    						     }
    							 childsConjp=childsConjp->next;
    						 }
    					 }
    				 }
    				 
    				 
    				 if (secCond && thirdCond && fourthCond){
//    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
    					 if (buildStanfordLink(STANFORD_DEP_PRECONJ, conjpTarg->lexical_head, node.lexical_head)) {
    						 addLinked(&node,conjpTarg);
    					     return true;
    					 }
    				 }
    			 }
    			 childsNp=childsNp->next;
    			 
    		 }
    	 }
    	 return false;
     }


     //"NP|NP-TMP|NP-ADV|NX|NML < (PDT|CC|DT=target < /^(?i:either|neither|both)$/ ) < (NP < CC)",
    inline const bool &preconj3(const unsigned long &cons){
    	 if (cons==PENN_CON_NP||cons==PENN_CON_NX){
    		 bool lastCondition=false;
    		 CStateNodeList* childsNP=node.m_umbinarizedSubNodes;
    		 while(childsNP!=0){
    			 if (CConstituent::clearTmp(childsNP->node->constituent.code())==PENN_CON_NP){
    				 CStateNodeList* childsL=childsNP->node->m_umbinarizedSubNodes;
    				 while(childsL!=0){
    					 if ((*words)[childsL->node->lexical_head].tag.code()==PENN_TAG_CC){
    						 lastCondition=true;
    					 }
    					 childsL=childsL->next;
    				 }
    			 }
    			 childsNP=childsNP->next;
    		 }

    		 childsNP=node.m_umbinarizedSubNodes;
    		 while(childsNP!=0){
    			 const CStateNode* targ=childsNP->node;
    			 if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_CC
    					 ||(*words)[targ->lexical_head].tag.code()==PENN_TAG_DT||
    					 (*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT) && !isLinked(&node,targ)){
    				 //CStateNodeList* childsTarg=targ->m_umbinarizedSubNodes;
    				 //while(childsTarg!=0){
    					 if ((*words)[targ->lexical_head].word==g_word_either ||
    							 (*words)[targ->lexical_head].word==g_word_neither||
    							 (*words)[targ->lexical_head].word==g_word_both){
//    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
    						 if (buildStanfordLink(STANFORD_DEP_PRECONJ, targ->lexical_head, node.lexical_head)) {
    							 addLinked(&node,targ);
    						     return true;
    						 }

    					 }
    					// childsTarg=childsTarg->next;
    				 //}
    			 }
    			 childsNP=childsNP->next;
    		 }
    	 }
    	 return false;
     }

     //"/^S|VP|ADJP|PP|ADVP|UCP(?:-TMP|-ADV)?|NX|NML|SBAR$/ < (PDT|DT|CC=target < /^(?i:either|neither|both)$/ $++ CC)",
    inline const bool &preconj4(const unsigned long &cons){
    	 if (cons==PENN_CON_S ||cons==PENN_CON_VP ||cons==PENN_CON_ADJP ||
    			 cons==PENN_CON_PP ||cons==PENN_CON_ADVP ||cons==PENN_CON_UCP||
    		 cons==PENN_CON_NX ||cons==PENN_CON_SBAR){
    		CStateNodeList* childsH=node.m_umbinarizedSubNodes;
    		while(childsH!=0){
    			const CStateNode* targ=childsH->node;
				if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT
						||(*words)[targ->lexical_head].tag.code()==PENN_TAG_DT
						||(*words)[targ->lexical_head].tag.code()==PENN_TAG_CC) && !isLinked(&node,targ)){
					bool sisterCond=false;
					CStateNodeList* rightSisters=childsH->next;
					while(rightSisters!=0){
						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC) {
							sisterCond=true;
						}
						rightSisters=rightSisters->next;
					}

					if (sisterCond){
						//CStateNodeList* childsDt=targ->m_umbinarizedSubNodes;
						//while(childsDt!=0){
							if (((*words)[targ->lexical_head].word==g_word_either)
									||((*words)[targ->lexical_head].word==g_word_neither)
									||((*words)[targ->lexical_head].word==g_word_both)){
//								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
								if (buildStanfordLink(STANFORD_DEP_PRECONJ, targ->lexical_head, node.lexical_head)) {
									addLinked(&node,targ);
								    return true;
								}
							}
							//childsDt=childsDt->next;
						//}
					}
				}
    			childsH=childsH->next;
    		}
    	 }
    	 return false;
     }



     //"/^S|VP|ADJP|PP|ADVP|UCP(?:-TMP|-ADV)?|NX|NML|SBAR$/ < (CONJP=target < (RB < /^(?i:not)$/) < (RB|JJ < /^(?i:only|merely|just)$/) $++ CC|CONJP)"
    inline const bool &preconj5(const unsigned long &cons){
    	 if (cons==PENN_CON_S ||cons==PENN_CON_VP ||cons==PENN_CON_ADJP ||
    	    			 cons==PENN_CON_PP ||cons==PENN_CON_ADVP ||cons==PENN_CON_UCP||
    	    		 cons==PENN_CON_NX ||cons==PENN_CON_SBAR){

    		 CStateNodeList* childsH=node.m_umbinarizedSubNodes;
    		 while(childsH!=0){
    			 const CStateNode* targ=childsH->node;
    			 if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_CONJP){
    				 bool secCond=false;
    				 bool thirdCond=false;
    				 bool fourthCond=false;
    				 CStateNodeList* rightSisters=childsH->next;
    				 while(rightSisters!=0){
    					 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC) {
    						 fourthCond=true;
    				 	}
    				 	rightSisters=rightSisters->next;
    				 }

    				 if (fourthCond) {
    					 CStateNodeList* childsT=targ->m_umbinarizedSubNodes;

    					 while(childsT!=0){
    						 if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_ADVERB) {
    							 //CStateNodeList* childsRb=childsT->node->m_umbinarizedSubNodes;
    							 //while(childsRb!=0){
    								 if ((*words)[childsT->node->lexical_head].word==g_word_not){
    									 secCond=true;
    								 }
    								// childsRb=childsRb->next;
    							 //}
    						 }
    					 	 childsT=childsT->next;
    				 	 }
    					 if (secCond){
    						 childsT=targ->m_umbinarizedSubNodes;
    						 while(childsT!=0){
    							 if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_ADVERB || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE) {
    								 //CStateNodeList* childsRb=childsT->node->m_umbinarizedSubNodes;
    						     	 //while(childsRb!=0){
    						     		 if ((*words)[childsT->node->lexical_head].word==g_word_only|| (*words)[childsT->node->lexical_head].word==g_word_merely || (*words)[childsT->node->lexical_head].word==g_word_just){
    						     			 thirdCond=true;
    						     		 }
    						     		// childsRb=childsRb->next;
    						     	 //}
    						      }
    						     childsT=childsT->next;
    						 }
    					 }
    				 }
    				 if (secCond && thirdCond && fourthCond){
//    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
    					 if (buildStanfordLink(STANFORD_DEP_PRECONJ, targ->lexical_head, node.lexical_head)) {
    						 addLinked(&node,targ);
    					 	 return true;
    					 }
    				 }
    			 }
    			 childsH=childsH->next;
    		 }
    	 }
    	 return false;
     }






