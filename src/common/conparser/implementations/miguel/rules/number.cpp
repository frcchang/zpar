  //"QP|ADJP < (/^(?:CD|$|#)$/=target !$- CC)"
     bool number1(){
    	 if (node.constituent==PENN_CON_ADJP||node.constituent==PENN_CON_QP){
    		 CStateNodeList* childsQpAdj=node.m_umbinarizedSubNodes;
    		 while(childsQpAdj!=0){
    			 const CStateNode* cdTarg=childsQpAdj->node;
    			 if ((((*words)[cdTarg->lexical_head].word==g_word_dollar) ||
    					 ((*words)[cdTarg->lexical_head].word==g_word_hash)||
    					 ((*words)[cdTarg->lexical_head].tag.code()==PENN_TAG_CD))&& !isLinked(&node,cdTarg)){

    				 bool ccCond=true;
    				 if (childsQpAdj->previous!=0){
    					 if ((*words)[childsQpAdj->previous->node->lexical_head].tag.code()==PENN_TAG_CC) {
    						 ccCond=false;
    					 }
    				 }
    				 if (ccCond){
    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NUMBER);
    					 if (buildStanfordLink(label, cdTarg->lexical_head, node.lexical_head)) {
    						 addLinked(&node,cdTarg);
    					     return true;
    					 }
    				 }
    			 }
    			 childsQpAdj=childsQpAdj->next;
    		 }
    	 }
    	 return false;
     }
