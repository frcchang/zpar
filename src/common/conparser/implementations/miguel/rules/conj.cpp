//conj rules. There are
      //(stf comment) this is more ugly, but the first 3 patterns are now duplicated and for clausal things, that daughter to the left of the CC/CONJP can't be a PP or RB or ADVP either
      //(stf comment) non-parenthetical or comma in suitable phrase with conjunction to left

 //"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
      bool buildConj1(){
    	  if (node.constituent==PENN_CON_VP || node.constituent==PENN_CON_S || node.constituent==PENN_CON_SBAR ||
    			  node.constituent==PENN_CON_SBARQ || node.constituent==PENN_CON_SINV || node.constituent==PENN_CON_SQ){
    		
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  if (childs->node->constituent==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
    				  bool leftCondition=false;
    				  CStateNodeList* leftSisters=childs->node->m_umbinarizedSubNodes;
    				  if (leftSisters->next!=0){
    					  leftCondition=true;
    				  }
    				  while(leftSisters!=0){
    					  if (leftSisters->node->constituent==PENN_CON_PP || leftSisters->node->constituent==PENN_CON_ADVP ||
    						(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || leftSisters->node->constituent==PENN_CON_PRN ||
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
    								  (*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || targ->constituent==PENN_CON_PRN||
    								  (*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    							  targCond=false;
    						  }  
    					  }
    					  if (targCond){
    						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
    						  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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
         bool buildConj2(){
       	  if (node.constituent==PENN_CON_VP || node.constituent==PENN_CON_S || node.constituent==PENN_CON_SBAR ||
       			  node.constituent==PENN_CON_SBARQ || node.constituent==PENN_CON_SINV || node.constituent==PENN_CON_SQ){
       		
       		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		  while(childs!=0){
       			  if (childs->node->constituent==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
       				  bool leftCondition=false;
       				  CStateNodeList* leftSisters=childs->node->m_umbinarizedSubNodes;
       				  if (leftSisters->next!=0){
       					  leftCondition=true;
       				  }
       				  while(leftSisters!=0){
       					  if (leftSisters->node->constituent==PENN_CON_PP || leftSisters->node->constituent==PENN_CON_ADVP ||
       						(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET || leftSisters->node->constituent==PENN_CON_PRN ||
       						(*words)[leftSisters->node->lexical_head].word==g_word_quotes){
       						  leftCondition=false; 
       						  //ASK JOHN what the ! symbol mean before the !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ 
       						  //I guess that it is a negation, so this thing matches every node but the thing that is negated.
       					  }
       					  leftSisters=leftSisters->previous;
       				  }
       				  
       				  
       				  if (leftCondition && childs->next!=0){
       					  if (childs->next->node->constituent==PENN_CON_ADVP){
       						  if (childs->next->next!=0){
       							  bool targCond=true;
       							  const CStateNode* targ=childs->next->next->node;
       							  if (!isLinked(&node,targ)){
       								  //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
       								  if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
       										  (*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
       										  (*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || targ->constituent==PENN_CON_PRN||
       										  (*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
       									  targCond=false;
       								  }  
       							  }
       							  if (targCond){
       								  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
       								  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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
//"VP|S|SBAR|SBARQ|SINV|SQ=root < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ (/^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target $-- (/^CC|CONJP|:|,$/ $-- (__ ># =root))) )",
bool buildConj3(){}

	// non-parenthetical or comma in suitable phrase with conjunction to left
//"/^(?:ADJP|JJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
bool buildConj4(){}

//"/^(?:VP|S|SBAR|SBARQ|SINV|ADJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC $++ (CC|CONJP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
    bool buildConj5(){
    	if (node.constituent==PENN_CON_VP || node.constituent==PENN_CON_S ||
    			node.constituent==PENN_CON_SBAR || node.constituent==PENN_CON_SBARQ||
    			node.constituent==PENN_CON_SINV || node.constituent==PENN_CON_ADJP||
    			node.constituent==PENN_CON_PP || node.constituent==PENN_CON_QP||
    			node.constituent==PENN_CON_NP || node.constituent==PENN_CON_WHNP||
    			node.constituent==PENN_CON_ADVP || node.constituent==PENN_CON_NX||
    			node.constituent==PENN_CON_UCP){
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		
    		while(childs!=0){
    			if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
    				CStateNodeList* rightSisters=childs->next;
    				while(rightSisters!=0){
    					if (rightSisters->node->constituent==PENN_CON_CONJP || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						if (rightSisters->next!=0){
    							bool targCond=true;
    							const CStateNode* targ=rightSisters->next->node;
    							if (!isLinked(&node,targ)){
    							 //!/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)
    								if ((*words)[targ->lexical_head].word==g_word_quotes || (*words)[targ->lexical_head].word==g_word_squotes || 
    										(*words)[targ->lexical_head].word==g_word_two_dots || (*words)[targ->lexical_head].word==g_word_dot ||
    										(*words)[targ->lexical_head].tag.code()==PENN_TAG_R_BRACKET || targ->constituent==PENN_CON_PRN||
    										(*words)[targ->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    									targCond=false;
    								}  
    							}
    							if (targCond){
    								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CONJ);
    								if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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
bool buildConj6(){}

//"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ /^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target)",
                // content phrase to the left of a comma for at least NX
bool buildConj7(){}




