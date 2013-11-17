
//"NP|NP-TMP|NP-ADV|NX|NML < (PDT|CC|DT=target < /^(?i:either|neither|both)$/ $++ CC)",
     bool preconj1(){
     	if (node.constituent==PENN_CON_NP ||node.constituent==PENN_CON_NX){
     		CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
     		while(childsNp!=0){
     			const CStateNode* pdtTarg=childsNp->node;
     			if ((((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_PDT) || ((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_CC) || ((*words)[pdtTarg->lexical_head].tag.code()==PENN_TAG_DT)) && !(isLinked(&node, pdtTarg))){
     				CStateNodeList* childsPdt=pdtTarg->m_umbinarizedSubNodes;
     				while(childsPdt!=0){
     					if (((*words)[childsPdt->node->lexical_head].word==g_word_either)||((*words)[childsPdt->node->lexical_head].word==g_word_neither)||((*words)[childsPdt->node->lexical_head].word==g_word_both)) {
     						CStateNodeList* rightSistersPdt=childsNp;
     						while(rightSistersPdt!=0){
     							if ((*words)[rightSistersPdt->node->lexical_head].tag.code()==PENN_TAG_CC) {
     								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
         							if (buildStanfordLink(label, pdtTarg->lexical_head, node.lexical_head)) {
         								addLinked(&node,pdtTarg);
         								return true;
         							}
     								
     							}
     							rightSistersPdt=rightSistersPdt->next;
     						}
     						
     					}
     					childsPdt=childsPdt->next;	
     				}
     			
     		}
     		childsNp=childsNp->next;
     	}
     }
     return false;
    }

     
     //"NP|NP-TMP|NP-ADV|NX|NML < (CONJP=target < (RB < /^(?i:not)$/) < (RB|JJ < /^(?i:only|merely|just)$/) $++ CC|CONJP)",
     bool preconj2(){
    	 if (node.constituent==PENN_CON_NP || node.constituent==PENN_CON_NX){
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 const CStateNode* conjpTarg=childsNp->node;
    			 if (conjpTarg->constituent==PENN_CON_CONJP && !(isLinked(&node,conjpTarg))){
    				 bool secCond=false;
    				 bool thirdCond=false;
    				 bool fourthCond=false;
    				 CStateNodeList* rightSisters=childsNp->next;
    				 while(rightSisters!=0){
    					 if (rightSisters->node->constituent==PENN_CON_CONJP ||(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						 fourthCond=true;
    					 }
    					 rightSisters=rightSisters->next;
    				 }
    				 if (fourthCond){
    					 CStateNodeList* childsConjp=conjpTarg->m_umbinarizedSubNodes;
    					 while(childsConjp!=0){
    						 //PENN_TAG_ADJECTIVE
    						 if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADVERB||(*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE){
    							 CStateNodeList* childsRB=childsConjp->node->m_umbinarizedSubNodes;
    							 while(childsRB!=0){
    								 if (((*words)[childsRB->node->lexical_head].word==g_word_not)||((*words)[childsRB->node->lexical_head].word==g_word_merely)||((*words)[childsRB->node->lexical_head].word==g_word_just)){
    									 secCond=true;
    								 }
    								 childsRB=childsRB->next;
    							 }
    						 }
    						 childsConjp=childsConjp->next;
    					 }
    					 if (secCond){
    						 childsConjp=conjpTarg->m_umbinarizedSubNodes;
    						 while(childsConjp!=0){
    							 if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_ADVERB){
    								 CStateNodeList* childsRB=childsConjp->node->m_umbinarizedSubNodes;
    								 while(childsRB!=0){
    									 if (((*words)[childsRB->node->lexical_head].word==g_word_not)){
    										 thirdCond=true;
    									 }
    									 childsRB=childsRB->next;
    								 }
    						     }
    							 childsConjp=childsConjp->next;
    						 }
    					 }
    				 }
    				 
    				 
    				 if (secCond && thirdCond && fourthCond){
    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRECONJ);
    					 if (buildStanfordLink(label, conjpTarg->lexical_head, node.lexical_head)) {
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
bool preconj3(){

}

//"/^S|VP|ADJP|PP|ADVP|UCP(?:-TMP|-ADV)?|NX|NML|SBAR$/ < (PDT|DT|CC=target < /^(?i:either|neither|both)$/ $++ CC)",
bool preconj4(){

}


//"/^S|VP|ADJP|PP|ADVP|UCP(?:-TMP|-ADV)?|NX|NML|SBAR$/ < (CONJP=target < (RB < /^(?i:not)$/) < (RB|JJ < /^(?i:only|merely|just)$/) $++ CC|CONJP)"
bool preconj5(){

}

