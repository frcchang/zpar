
//"PP|XS < (IN|TO < as|of|at|to|in) < (JJ|IN|JJR|JJS|NN=target < such|because|Because|least|instead|due|Due|addition|to)",
bool mwe1(){

}

//"ADVP < (RB|IN < well) < (IN|RB|JJS=target < as)",
bool mwe2(){

}

//"ADVP < (DT < all) < (CC < but)",
bool mwe3(){

}

//"CONJP < (RB < rather|well|instead) < (RB|IN=target < as|than|of)",
bool mwe4(){

}

//CONJP < (IN < in) < (NN|TO=target < addition|to)",
bool mwe5(){

	if (node.constituent==PENN_CON_CONJP) {
		CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
		bool firstCondition=false;
		while(childsConjp!=0) {
			const CStateNode* InChild=childsConjp->node;
			if ((*words)[InChild->lexical_head].tag.code()==PENN_TAG_IN) {
				CStateNodeList* childsIn=InChild->m_umbinarizedSubNodes;
				while(childsIn!=0){
					if ((*words)[childsIn->node->lexical_head].word==g_word_in) {
						firstCondition=true;
					}
					childsIn=childsIn->next;
				}
				
			}
			
			childsConjp=childsConjp->next;
		}
		if (firstCondition) {
			childsConjp=node.m_umbinarizedSubNodes;
			while(childsConjp!=0){
				const CStateNode* nntoTarg=childsConjp->node;
				if ((((*words)[nntoTarg->lexical_head].tag.code()==PENN_TAG_NOUN) ||((*words)[nntoTarg->lexical_head].tag.code()==PENN_TAG_TO)) && !(isLinked(&node,nntoTarg))) {
					CStateNodeList* childsNNTo=nntoTarg->m_umbinarizedSubNodes;
					while(childsNNTo!=0){
						if (((*words)[childsNNTo->node->lexical_head].word==g_word_addition) && ((*words)[childsNNTo->node->lexical_head].word==g_word_to)) {
//							CDependencyLabel* label=new CDependencyLabel(0);
							if (buildStanfordLink(0, nntoTarg->lexical_head, node.lexical_head)) {
								addLinked(&node,nntoTarg);
							  	return true;
							 }
						}
						
						childsNNTo=childsNNTo->next;
					}
					
				}
				childsConjp=childsConjp->next;
			}
		}
	}
}

//"XS < JJR|JJS=target"
bool mwe6(){

}
