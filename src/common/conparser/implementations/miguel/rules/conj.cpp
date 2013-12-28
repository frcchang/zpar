//conj rules. There are
      //(stf comment) this is more ugly, but the first 3 patterns are now duplicated and for clausal things, that daughter to the left of the CC/CONJP can't be a PP or RB or ADVP either
      //(stf comment) non-parenthetical or comma in suitable phrase with conjunction to left

 //"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
      bool buildConj1(const unsigned long &cons){
    	  if (cons==PENN_CON_VP || cons==PENN_CON_S || cons==PENN_CON_SBAR ||
    			  cons==PENN_CON_SBARQ || cons==PENN_CON_SINV || cons==PENN_CON_SQ){
    		
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
    				  bool leftCondition=false;
    				  CStateNodeList* leftSisters=childs->previous;
    				  if (leftSisters!=0){
    					  leftCondition=true;
    				  }
    				  while(leftSisters!=0){
    					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PP || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_ADVP ||
    						(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PRN ||
    						(*words)[leftSisters->node->lexical_head].word==g_word_quotes){
    						  leftCondition=false; 
    						  //ASK JOHN what the ! symbol mean before the !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ 
    						  //I guess that it is a negation, so this thing matches every node but the thing that is negated.
    					  }
    					  leftSisters=leftSisters->previous;
    				  }
    				  
    				  
    				  if (leftCondition && childs->next!=0){
    					  bool targCond=true;
    					  const CStateNode* targ=childs->next->node;
    					  if (!isLinked(&node,targ)){
    						  //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
    						  if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
    								  (*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
    								  (*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN||
    								  (*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    							  targCond=false;
    						  }  
    					  }
    					  if (targCond){
//    						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
    						  if (buildStanfordLink(STANFORD_DEP_CONJ, targ->lexical_head, node.lexical_head)) {
    							  addLinked(&node,targ);
    						      return true;
    						  }
    					  }  
    				  }
    			  }
    			  childs=childs->next;
    		  }
    		  
    	  }
    	  return false;
      }
      


// non-parenthetical or comma in suitable phrase with conj then adverb to left
      //"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
         bool buildConj2(const unsigned long &cons){
       	  if (cons==PENN_CON_VP || cons==PENN_CON_S || cons==PENN_CON_SBAR ||
       			  cons==PENN_CON_SBARQ || cons==PENN_CON_SINV || cons==PENN_CON_SQ){
       		
       		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		  while(childs!=0){
       			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
       				  bool leftCondition=false;
       				  CStateNodeList* leftSisters=childs->previous;
       				  if (leftSisters!=0){
       					  leftCondition=true;
       				  }
       				  while(leftSisters!=0){
       					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PP || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_ADVP ||
       						(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PRN ||
       						(*words)[leftSisters->node->lexical_head].word==g_word_quotes){
       						  leftCondition=false; 
       						  //ASK JOHN what the ! symbol mean before the !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ 
       						  //I guess that it is a negation, so this thing matches every node but the thing that is negated.
       					  }
       					  leftSisters=leftSisters->previous;
       				  }
       				  
       				  
       				  if (leftCondition && childs->next!=0){
       					  if (CConstituent::clearTmp(childs->next->node->constituent.code())==PENN_CON_ADVP){
       						  if (childs->next->next!=0){
       							  bool targCond=true;
       							  const CStateNode* targ=childs->next->next->node;
       							  if (!isLinked(&node,targ)){
       								  //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
       								  if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
       										  (*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
       										  (*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN||
       										  (*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
       									  targCond=false;
       								  }  
       							  }
       							  if (targCond){
//       								  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
       								  if (buildStanfordLink(STANFORD_DEP_CONJ, targ->lexical_head, node.lexical_head)) {
       									  addLinked(&node,targ);
       									  return true;
       								  }
       							  }  
       						  }
       					  }
       				  }
       			  }
       			  childs=childs->next;
       		  }
       		  
       	  }
       	  return false;
         	
         } 
      // content phrase to the right of a comma or a parenthetical
      // The test at the end is to make sure that a conjunction or
      // comma etc actually show up between the target of the conj

      // dependency and the head of the phrase.  Otherwise, a
      // different relationship is probably more appropriate.



//"/^(?:ADJP|JJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
bool buildConj4(const unsigned long &cons){
	if (cons==PENN_CON_ADJP || cons==PENN_CON_PP || cons==PENN_CON_QP || cons==PENN_CON_WHNP 
			|| cons==PENN_CON_NP || cons==PENN_CON_ADVP || cons==PENN_CON_UCP || cons==PENN_CON_NX){
		CStateNodeList* childs=node.m_umbinarizedSubNodes;
		while(childs!=0){
			if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
				bool leftCond=false;
				CStateNodeList* leftSisters=childs->previous;
				if (leftSisters!=0) leftCond=true;
				while(leftSisters!=0){
					if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PRN ||
    						(*words)[leftSisters->node->lexical_head].word==g_word_quotes){
						//g_word_quotes
						leftCond=false;
					}
					leftSisters=leftSisters->previous;
				}
				if (leftCond && childs->next!=0){
					const CStateNode* targ=childs->next->node;
					if (!isLinked(&node,targ)){
						bool targCond=true;
						 //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
						 if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
						    (*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
						    (*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN||
						    (*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
						  	  targCond=false;
						 }
						 if (targCond){
//							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
							 if (buildStanfordLink(STANFORD_DEP_CONJ, targ->lexical_head, node.lexical_head)) {
								 addLinked(&node,targ);
							     return true;
							 }
						 }
					}
				}
			}
			childs=childs->next;
		}
		
		//if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_CONJP || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
	}
	return false;
}

	// non-parenthetical or comma in suitable phrase with conjunction to left

//"/^(?:VP|S|SBAR|SBARQ|SINV|ADJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC $++ (CC|CONJP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
    bool buildConj5(const unsigned long &cons){
    	if (cons==PENN_CON_VP || cons==PENN_CON_S ||
    			cons==PENN_CON_SBAR || cons==PENN_CON_SBARQ||
    			cons==PENN_CON_SINV || cons==PENN_CON_ADJP||
    			cons==PENN_CON_PP || cons==PENN_CON_QP||
    			cons==PENN_CON_NP || cons==PENN_CON_WHNP||
    			cons==PENN_CON_ADVP || cons==PENN_CON_NX||
    			cons==PENN_CON_UCP){
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		
    		while(childs!=0){
    			if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
    				CStateNodeList* rightSisters=childs->next;
    				while(rightSisters!=0){
    					if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_CONJP || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						if (rightSisters->next!=0){
    							bool targCond=true;
    							const CStateNode* targ=rightSisters->next->node;
    							if (!isLinked(&node,targ)){
    							 //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
    								if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
    										(*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
    										(*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN||
    										(*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    									targCond=false;
    								}  
    							}
    							if (targCond){
//    								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
    								if (buildStanfordLink(STANFORD_DEP_CONJ, targ->lexical_head, node.lexical_head)) {
    									addLinked(&node,targ);
    									return true;
    								}
    							}	  
    						}
    					}
    					rightSisters=rightSisters->next;
    				}
    			}
    			childs=childs->next;
    		}
    	}
    	return false;
    }
    

    //"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
          // content phrase to the right of a comma or a parenthetical
    bool buildConj6(const unsigned long &cons){
    	if (cons==PENN_CON_ADJP || cons==PENN_CON_PP || cons==PENN_CON_NP || cons==PENN_CON_WHNP || cons==PENN_CON_ADVP || cons==PENN_CON_UCP
    			|| cons==PENN_CON_NX) {
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		while(childs!=0){
    			if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
    				bool leftCond=false;
    				CStateNodeList* leftSisters=childs->previous;
    				if (leftSisters!=0) leftCond=true;
    					while(leftSisters!=0){
    						if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_PRN ||
    								(*words)[leftSisters->node->lexical_head].word==g_word_quotes){
    								//g_word_quotes
    								leftCond=false;
    						}
    						leftSisters=leftSisters->previous;
    					}
    					if (leftCond){
    						if (childs->next!=0){
    							if (childs->next->node->constituent==PENN_CON_ADVP){
    								if (childs->next->next!=0){
    									bool targCond=true;
    									const CStateNode* targ=childs->next->next->node;
    									if (!isLinked(&node,targ)){
    									//!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
    										if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
    												(*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
    												(*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN||
    												(*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    											targCond=false;
    										}  
    									}
    									if (targCond){
    										if (buildStanfordLink(STANFORD_DEP_CONJ, targ->lexical_head, node.lexical_head)) {
    											addLinked(&node,targ);
    									    	return true;
    									    }
    									}
    								}
    							}
    						}
    					}
    			}
    			childs=childs->next;
    		}
    		
    	}
    	return false;
    }








