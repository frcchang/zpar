
//"/^(?:VP|NP(?:-TMP|-ADV)?|ADJP|SQ|S|FRAG|CONJP|PP)$/< (RB=target < " + NOT_PAT + ")",
     bool neg1(){
    	 if (node.constituent==PENN_CON_VP ||node.constituent==PENN_CON_NP ||node.constituent==PENN_CON_ADJP ||node.constituent==PENN_CON_SQ ||node.constituent==PENN_CON_S
    			 ||node.constituent==PENN_CON_FRAG ||node.constituent==PENN_CON_CONJP ||node.constituent==PENN_CON_PP){

    		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		 while(childs!=0){
    			 const CStateNode* targ=childs->node;
    			 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB && (!isLinked(&node,targ))){
    				 CStateNodeList* childsRb=targ->m_umbinarizedSubNodes;
    				 while(childsRb!=0){
    					 if (((*words)[childsRb->node->lexical_head].word==g_word_not) ||((*words)[childsRb->node->lexical_head].word==g_word_nt)){
    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
    						 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
    							 addLinked(&node,targ);
    							 return true;
    						 }
    				   	}
    				   	childsRb=childsRb->next;
    				 }
    			 }
    			 childs=childs->next;
    		 }
    	 }
    	 return false;
     }

     //"VP|ADJP|S|SBAR|SINV|FRAG < (ADVP=target <# (RB < " + NOT_PAT + "))",
        bool neg2(){
       	 if (node.constituent==PENN_CON_VP || node.constituent==PENN_CON_ADJP ||node.constituent==PENN_CON_S || node.constituent==PENN_CON_SBAR || node.constituent==PENN_CON_SINV || node.constituent==PENN_CON_FRAG) {
       		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		 while(childs!=0){
       			 const CStateNode* targ=childs->node;
       			 if (targ->constituent==PENN_CON_ADVP && !(isLinked(&node,targ))){
       				 CStateNodeList* childsTarg=targ->m_umbinarizedSubNodes;
       				 while(childsTarg!=0){
       					 const CStateNode* adv=childsTarg->node;
       					 if ((*words)[adv->lexical_head].tag.code()==PENN_TAG_ADVERB && adv->lexical_head==targ->lexical_head){ //<#
       						 CStateNodeList* childsRb=targ->m_umbinarizedSubNodes;
       						 while(childsRb!=0){
       							 if (((*words)[childsRb->node->lexical_head].word==g_word_not) ||((*words)[childsRb->node->lexical_head].word==g_word_nt)){
       								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
       						     	 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
       						     		 addLinked(&node,targ);
       						     		 return true;
       						     		 }
       						     	 }
       						   	 childsRb=childsRb->next;
       						 }
       					 }
       					 childsTarg=childsTarg->next;
       				 }
       			 }
       			 childs=childs->next;
       		 }
       	 }
       	 return false;
        }



// "VP > SQ $-- (RB=target < " + NOT_PAT + ")"
   bool neg3(){
  	 if (node.constituent==PENN_CON_SQ){

  		 CStateNodeList* childsSq=node.m_umbinarizedSubNodes;
  		 while(childsSq!=0){
  			 const CStateNode* head=childsSq->node;
  			 if (head->constituent==PENN_CON_VP){
  				 CStateNodeList* leftSisters=childsSq->previous;
  				 while(leftSisters!=0){
  					 const CStateNode* targ=leftSisters->node;
  					 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB && (!isLinked(&node,targ))){
  						 CStateNodeList* childsRb=targ->m_umbinarizedSubNodes;
  						 while(childsRb!=0){
  							 if (((*words)[childsRb->node->lexical_head].word==g_word_not) ||((*words)[childsRb->node->lexical_head].word==g_word_nt)){
  								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
  								 if (buildStanfordLink(label, targ->lexical_head, head->lexical_head)) {
  									 addLinked(&node,targ);
  								     return true;
  								 }
  							 }
  							 childsRb=childsRb->next;
  						 }
  					 }
  					 leftSisters=leftSisters->previous;
  				 }
  			 }
  			 childsSq=childsSq->next;
  		 }
  	 }
  	 return false;
   }
