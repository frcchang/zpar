//"/^(?:WH)?(?:NP|INTJ|ADJP|PRN|NAC|NX|NML)(?:-TMP|-ADV)?$/ < /^(?:W|PR)P\\$$/=target",
      bool poss1(){
    	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_ADJP || node.constituent==PENN_CON_PRN || node.constituent==PENN_CON_INTJ||
    			  node.constituent==PENN_CON_NX || node.constituent==PENN_CON_NAC){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  //PENN_TAG_PRP, PENN_TAG_PRP_DOLLAR
    			  //PENN_TAG_WP, PENN_TAG_WP_DOLLAR
    			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_PRP || (*words)[targ->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR||
    					  (*words)[targ->lexical_head].tag.code()==PENN_TAG_WP||(*words)[targ->lexical_head].tag.code()==PENN_TAG_WP_DOLLAR) && !isLinked(&node,targ)){
    				  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSS);
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

      //"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ [ < (WHNP|WHNML|NP|NML=target [ < POS | < (VBZ < /^'s$/) ] ) !< (CC|CONJP $++ WHNP|WHNML|NP|NML)
            	  	  	  	  	  	  	  	  	  	  //|  < (WHNP|WHNML|NP|NML=target < (CC|CONJP $++ WHNP|WHNML|NP|NML) < (WHNP|WHNML|NP|NML [ < POS | < (VBZ < /^'s$/) ] )) ]",

            //Can be divided in 2

            ///^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ < (WHNP|WHNML|NP|NML=target [ < POS | < (VBZ < /^'s$/) ] ) !< (CC|CONJP $++ WHNP|WHNML|NP|NML)
            bool poss2(){
          	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP){
          		  bool lastCond=true;
          		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
          		  while(childs!=0){
          			  if (childs->node->constituent==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
          				  CStateNodeList* rightSisters=childs->next;
          				  while(rightSisters!=0){
          					  if (rightSisters->node->constituent==PENN_CON_WHNP || rightSisters->node->constituent==PENN_CON_NP){
          						  lastCond=false;
          					  }
          					  rightSisters=rightSisters->next;
          				  }
          			  }
          			  childs=childs->next;
          		  }
          		  if (lastCond){
          			  childs=node.m_umbinarizedSubNodes;
          			  while(childs!=0){
          				  const CStateNode* targ=childs->node;
          				  if ((targ->constituent==PENN_CON_WHNP || targ->constituent==PENN_CON_NP)&& !isLinked(&node,targ)){
          				  CStateNodeList* childsWnp=childs->node->m_umbinarizedSubNodes;
          				  bool opt1=false;
          				  bool opt2=false;
          				  while(childsWnp!=0){
          					  if ((*words)[childsWnp->node->lexical_head].tag.code()==PENN_TAG_POS){
          						  opt1=true;
          				      }
          				      else if ((*words)[childsWnp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
          				    	  CStateNodeList* childsVbz=childsWnp->node->m_umbinarizedSubNodes;
          				      	  while(childsVbz!=0){
          				      		if ((*words)[childsVbz->node->lexical_head].word==g_word_aps){
          				      			opt2=true;
          				      		}
          				      		childsVbz=childsVbz->next;
          				      	  }
          				     }
          				     childsWnp=childsWnp->next;
          				  }
          				  if (opt1||opt2){
          					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSS);
          					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
          						  addLinked(&node,targ);
          					      return true;
          					  }
          				  }
          			 }
          			childs=childs->next;
          		   }
          		 }
          	  }
          	  return false;
            }
            ///^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/  < (WHNP|WHNML|NP|NML=target < (CC|CONJP $++ WHNP|WHNML|NP|NML) < (WHNP|WHNML|NP|NML [ < POS | < (VBZ < /^'s$/) ] ))",
            bool poss21(){
          	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP){
          		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
          		  while(childs!=0){
          			  const CStateNode* targ=childs->node;
          			  if ((targ->constituent==PENN_CON_WHNP ||targ->constituent==PENN_CON_NP) && !isLinked(&node,targ)){
          				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
          				  bool firstCond=false;
          				  bool secCond=false;
          				  while(childsT!=0){
          					  if (childsT->node->constituent==PENN_CON_CONJP || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_CC){
          						  CStateNodeList* rightSisters=childsT->next;
          						  while(rightSisters!=0){
          							  if (rightSisters->node->constituent==PENN_CON_WHNP || rightSisters->node->constituent==PENN_CON_NP){
          								  firstCond=true;
          							  }
          							  rightSisters=rightSisters->next;
          						  }
          					  }
          					  else if (childsT->node->constituent==PENN_CON_WHNP || childsT->node->constituent==PENN_CON_NP){
          						  CStateNodeList* childsWnp=childsT->node->m_umbinarizedSubNodes;
          						  bool opt1=false;
          						  bool opt2=false;
          						  while(childsWnp!=0){
          							  if ((*words)[childsWnp->node->lexical_head].tag.code()==PENN_TAG_POS){
          								  opt1=true;
          							  }
          							  else if ((*words)[childsWnp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
          								  CStateNodeList* childsVbz=childsWnp->node->m_umbinarizedSubNodes;
          								  while(childsVbz!=0){
          									  if ((*words)[childsVbz->node->lexical_head].word==g_word_aps){
          										  opt2=true;
          									  }
          									  childsVbz=childsVbz->next;
          								  }
          							  }
          							  childsWnp=childsWnp->next;
          						  }
          						  secCond=opt1||opt2;
          					  }
          					  childsT=childsT->next;
          				  }

          				  if (firstCond && secCond){
          					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSS);
          					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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



//"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ < (/^NN/=target $+ (POS < /'/ $++ /^NN/))"
bool poss3(){
	  if (node.constituent==PENN_CON_WHNP) {
		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
		  while(childs!=0){
			  const CStateNode* targ=childs->node;
			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN)
				 ||((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER)
			     ||((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)
			     ||((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL) && !isLinked(&node,targ)){
				  
				  CStateNodeList* rightSisters=childs->next;
				  if (rightSisters!=0){
					  if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_POS){
						  bool sisCond=false;
						  bool childCond=false;
						  CStateNodeList* childsR=rightSisters->node->m_umbinarizedSubNodes;
						  rightSisters=rightSisters->next;
						  while(rightSisters!=0){
							  if (((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN)
							      ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER)
							      ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)
							      ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)){
								  sisCond=true;
							  }
							  rightSisters=rightSisters->next;
						  }
						  while(childsR!=0){
							  if ((*words)[childsR->node->lexical_head].word==g_word_quote){
								  childCond=true;
							  }
							  childsR=childsR->next;
						  }
						  if (sisCond && childCond){
							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POSS);
							  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
								  addLinked(&node,targ);
							      return true;
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


