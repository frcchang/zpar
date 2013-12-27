
//"/^(?:VP|NP(?:-TMP|-ADV)?|ADJP|SQ|S|FRAG|CONJP|PP)$/< (RB=target < " + NOT_PAT + ")",
     inline const bool & neg1(const unsigned long &cons){
    	 if (cons==PENN_CON_VP ||cons==PENN_CON_NP ||cons==PENN_CON_ADJP ||cons==PENN_CON_SQ ||cons==PENN_CON_S
    			 ||cons==PENN_CON_FRAG ||cons==PENN_CON_CONJP ||cons==PENN_CON_PP){
    		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		 while(childs!=0){
    			 const CStateNode* targ=childs->node;
    			 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB && (!isLinked(&node,targ))){
    				 if (((*words)[childs->node->lexical_head].word==g_word_not) ||((*words)[childs->node->lexical_head].word==g_word_nt)){
//    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
    					 if (buildStanfordLink(STANFORD_DEP_NEG, targ->lexical_head, node.lexical_head)) {
    						 addLinked(&node,targ);
    					     return true;
    					 }
    				 }
    			 }
    			 childs=childs->next;
    		 }
    	 }
    	 return false;
     }

     //"VP|ADJP|S|SBAR|SINV|FRAG < (ADVP=target <# (RB < " + NOT_PAT + "))",
     inline const bool & neg2(const unsigned long &cons){
       	 if (cons==PENN_CON_VP || cons==PENN_CON_ADJP ||cons==PENN_CON_S || cons==PENN_CON_SBAR || cons==PENN_CON_SINV || cons==PENN_CON_FRAG) {
       		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		 while(childs!=0){
       			 const CStateNode* targ=childs->node;
       			 if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADVP && !(isLinked(&node,targ))){
       				 CStateNodeList* childsTarg=targ->m_umbinarizedSubNodes;
       				 while(childsTarg!=0){
       					 const CStateNode* adv=childsTarg->node;
       					 if ((*words)[adv->lexical_head].tag.code()==PENN_TAG_ADVERB && adv->lexical_head==targ->lexical_head){ //<#
       						if (((*words)[adv->lexical_head].word==g_word_not) ||((*words)[adv->lexical_head].word==g_word_nt)){
//       							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
       							if (buildStanfordLink(STANFORD_DEP_NEG, targ->lexical_head, node.lexical_head)) {
       								addLinked(&node,targ);
       						     	return true;
       						     	}
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
     inline const bool &neg3(const unsigned long &cons){
  	 if (cons==PENN_CON_SQ){

  		 CStateNodeList* childsSq=node.m_umbinarizedSubNodes;
  		 while(childsSq!=0){
  			 const CStateNode* head=childsSq->node;
  			 if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_VP){
  				 CStateNodeList* leftSisters=childsSq->previous;
  				 while(leftSisters!=0){
  					 const CStateNode* targ=leftSisters->node;
  					 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB && (!isLinked(&node,targ))){
  						 //CStateNodeList* childsRb=targ->m_umbinarizedSubNodes;
  						 //while(childsRb!=0){
  							 if (((*words)[targ->lexical_head].word==g_word_not) ||((*words)[targ->lexical_head].word==g_word_nt)){
//  								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NEG);
  								 if (buildStanfordLink(STANFORD_DEP_NEG, targ->lexical_head, head->lexical_head)) {
  									 addLinked(&node,targ);
  								     return true;
  								 }
  							 }
  							// childsRb=childsRb->next;
  						 //}
  					 }
  					 leftSisters=leftSisters->previous;
  				 }
  			 }
  			 childsSq=childsSq->next;
  		 }
  	 }
  	 return false;
   }
