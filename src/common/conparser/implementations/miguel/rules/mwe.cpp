
//"PP|XS < (IN|TO < as|of|at|to|in) < (JJ|IN|JJR|JJS|NN=target < such|because|Because|least|instead|due|Due|addition|to)",
inline const bool &mwe1(const unsigned long &cons) {

}

//"ADVP < (RB|IN < well) < (IN|RB|JJS=target < as)",
inline const bool &mwe2(const unsigned long &cons) {

}

//"ADVP < (DT < all) < (CC < but)",
inline const bool &mwe3(const unsigned long &cons) {

}

//"CONJP < (RB < rather|well|instead) < (RB|IN=target < as|than|of)",
inline const bool &mwe4(const unsigned long &cons) {

}

//CONJP < (IN < in) < (NN|TO=target < addition|to)",
inline const bool &mwe5(const unsigned long &cons) {

	if (cons==PENN_CON_CONJP) {
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
							if (buildStanfordLink(STANFORD_DEP_MWE, nntoTarg->lexical_head, node.lexical_head)) {
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
inline const bool &mwe6(const unsigned long &cons) {

}
