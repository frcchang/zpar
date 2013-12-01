
 //"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ < POS=target",
      bool possessive1(){
    	  if (node.constituent==PENN_CON_WHNP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT) && !isLinked(&node,targ)){
    				  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSSESSIVE);
    				  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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
                 bool possessive2(){
               	  if (node.constituent==PENN_CON_WHNP){
               		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
               		  while(childs!=0){
               			  const CStateNode* targ=childs->node;
               			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) && !isLinked(&node,targ)){
               				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
               				  while(childsT!=0){
               					  if ((*words)[childsT->node->lexical_head].word==g_word_aps){
               						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSSESSIVE);
               						  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
               							  addLinked(&node,targ);
               						      return true;
               						  } 
               					  }
               					  childsT=childsT->next;
               				  }
               				  
               				  
               			  }
               			  childs=childs->next;
               		  }
               	  }
               	  return false;
                 }  
