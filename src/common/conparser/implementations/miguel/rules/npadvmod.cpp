
//"@ADVP|ADJP|WHADJP|WHADVP|PP|WHPP <# (JJ|JJR|IN|RB|RBR !< notwithstanding $- (@NP=target !< NNP|NNPS))",
bool npadvmod1(){

}

//"@ADJP < (NN=target $++ /^JJ/) !< CC|CONJP",
bool npadvmod2(){
	 if (node.constituent==PENN_CON_ADJP){
		 CStateNodeList* childsAdjp=node.m_umbinarizedSubNodes;
		 bool firstCondition=true;
		 
		 while(childsAdjp!=0){
			 if (((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_CC) || childsAdjp->node->constituent==PENN_CON_CONJP) {
				 firstCondition=false;
			 }
			 childsAdjp=childsAdjp->next;
		 }
		 if (firstCondition){
			 childsAdjp=node.m_umbinarizedSubNodes;
			 while(childsAdjp!=0){
				 const CStateNode* nnTarg=childsAdjp->node;
				 if (((*words)[nnTarg->lexical_head].tag.code()==PENN_TAG_NOUN)&& !(isLinked(&node,nnTarg))) {
					 CStateNodeList* rightSistersNN=childsAdjp;
					 while(rightSistersNN!=0){
						 //PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
						 if (((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)||
								 ((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE)||
								 ((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)) {
							 CDependencyLabel* label=new CDependencyLabel(0);
							 if (buildStanfordLink(label, nnTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,nnTarg);
							 	return true;
							 }
							 
						 }
						 rightSistersNN=rightSistersNN->next;
					 }
				 }
				 childsAdjp=childsAdjp->next;
			 }
		 }
		 
	 }
	 return false;
}


//"@NP|WHNP < /^NP-ADV/=target",
bool npadvmod3(){

}


//"@NP <1 (@NP <<# /^%$/) <2 (@NP=target <<# days|month|months) !<3 __",
bool npadvmod4(){

}

//"@VP < /^NP-ADV/=target",
bool npadvmod5(){

}
