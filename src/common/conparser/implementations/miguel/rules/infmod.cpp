
   //"/^NP(?:-[A-Z]+)?$/ < (S=target < (VP < TO) $-- NP|NN|NNP|NNS)",
inline const bool &infmod1(const unsigned long &cons){
    	  if (cons==PENN_CON_NP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_S && !isLinked(&node,targ)){
    				  bool sistCond=false;
    				  bool childsCond=false;
    				  
    				  CStateNodeList* leftSisters=childs->previous;
    				  while(leftSisters!=0){
    					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN
    							  || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL){
    						  sistCond=true;
    					  }
    					  leftSisters=leftSisters->previous;
    				  }
    				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    				  while(childsT!=0){
    					  if (CConstituent::clearTmp(childsT->node->constituent.code())==PENN_CON_VP){
    						  CStateNodeList* childsVp=childsT->node->m_umbinarizedSubNodes;
    						  while(childsVp!=0){
    							  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
    								  sistCond=true;
    							  }
    							  childsVp=childsVp->next;
    						  }
    					  }
    					  childsT=childsT->next;
    				  }
    				  if (sistCond && childsCond){
//    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_INFMOD);
    					  if (buildStanfordLink(STANFORD_DEP_INFMOD, targ->lexical_head, node.lexical_head)) {
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

      //"/^NP(?:-[A-Z]+)?$/ < (SBAR=target < (S < (VP < TO)) $-- NP|NN|NNP|NNS)"
inline const bool &infmod2(const unsigned long &cons){
               	  if (cons==PENN_CON_NP){
               		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
               		  while(childs!=0){
               			  const CStateNode* targ=childs->node;
               			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_SBAR && !isLinked(&node,targ)){
               				  bool sistCond=false;
               				  bool childsCond=false;
               				  
               				  CStateNodeList* leftSisters=childs->previous;
               				  while(leftSisters!=0){
               					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN
               							  || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL){
               						  sistCond=true;
               					  }
               					  leftSisters=leftSisters->previous;
               				  }
               				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
               				  while(childsT!=0){
               					if (CConstituent::clearTmp(childsT->node->constituent.code())==PENN_CON_S){
               						CStateNodeList* childsS=childsT->node->m_umbinarizedSubNodes;
               						while(childsS!=0){
               							if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
               								CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
               								while(childsVp!=0){
               									if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
               										sistCond=true;
               									}
               									childsVp=childsVp->next;
               								}
               							}
               							childsS=childsS->next;
               						}
               				  }
               					  childsT=childsT->next;
               				  }
               				  if (sistCond && childsCond){
//               					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_INFMOD);
               					  if (buildStanfordLink(STANFORD_DEP_INFMOD, targ->lexical_head, node.lexical_head)) {
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

