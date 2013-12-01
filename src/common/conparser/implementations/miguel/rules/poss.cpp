
//"/^(?:WH)?(?:NP|INTJ|ADJP|PRN|NAC|NX|NML)(?:-TMP|-ADV)?$/ < /^(?:W|PR)P\\$$/=target",
bool poss1(){

}

//"/^(?:WH)?(?:NP|NML)(?:-TMP|-ADV)?$/ [ < (WHNP|WHNML|NP|NML=target [ < POS | < (VBZ < /^'s$/) ] ) !< (CC|CONJP $++ WHNP|WHNML|NP|NML) |  < (WHNP|WHNML|NP|NML=target < (CC|CONJP $++ WHNP|WHNML|NP|NML) < (WHNP|WHNML|NP|NML [ < POS | < (VBZ < /^'s$/) ] )) ]",
bool poss2(){

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


