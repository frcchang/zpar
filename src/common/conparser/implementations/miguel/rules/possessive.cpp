
 //"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ < POS=target",
     inline const bool &possessive1(const unsigned long &cons){
    	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_POS) && !isLinked(&node,targ)){
//    				  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSSESSIVE);
    				  if (buildStanfordLink(STANFORD_DEP_POSSESSIVE, targ->lexical_head, node.lexical_head)) {
    					  addLinked(&node,targ);
    				      return true;
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }  

      //"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ < (VBZ=target < /^'s$/)"
     inline const bool &possessive2(const unsigned long &cons){
               	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP){
               		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
               		  while(childs!=0){
               			  const CStateNode* targ=childs->node;
               			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) && !isLinked(&node,targ)){
               				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
               				  //while(childsT!=0){
               					  if ((*words)[targ->lexical_head].word==g_word_aps){
//               						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSSESSIVE);
               						  if (buildStanfordLink(STANFORD_DEP_POSSESSIVE, targ->lexical_head, node.lexical_head)) {
               							  addLinked(&node,targ);
               						      return true;
               						  } 
               					  }
               					//  childsT=childsT->next;
               				  //}
               				  
               				  
               			  }
               			  childs=childs->next;
               		  }
               	  }
               	  return false;
                 }  
