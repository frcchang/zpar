
//"PP|XS < (IN|TO < as|of|at|to|in) < (JJ|IN|JJR|JJS|NN=target < such|because|Because|least|instead|due|Due|addition|to)",
inline const bool &mwe1(const unsigned long &cons) {
    	if (cons==PENN_CON_PP){ //no xs
    		bool firstCond=false;
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		bool firstCondition=false;
    		while(childs!=0){
    			if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_TO)
    					|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN))
    					&& ((*words)[childs->node->lexical_head].word==g_word_as || (*words)[childs->node->lexical_head].word==g_word_of || (*words)[childs->node->lexical_head].word==g_word_at
    							|| (*words)[childs->node->lexical_head].word==g_word_to || (*words)[childs->node->lexical_head].word==g_word_in)) {
    				firstCondition=true;
    			}
    			childs=childs->next;
    		}

    		if (firstCondition){
    			childs=node.m_umbinarizedSubNodes;
    			while(childs!=0){
    				if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE) || ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN)
    						|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE)
    						|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE) || ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN))
    						&& ((*words)[childs->node->lexical_head].word==g_word_such || (*words)[childs->node->lexical_head].word==g_word_because || (*words)[childs->node->lexical_head].word==g_word_Because ||
    								(*words)[childs->node->lexical_head].word==g_word_least || (*words)[childs->node->lexical_head].word==g_word_instead || (*words)[childs->node->lexical_head].word==g_word_due ||
    								(*words)[childs->node->lexical_head].word==g_word_Due || (*words)[childs->node->lexical_head].word==g_word_addition || (*words)[childs->node->lexical_head].word==g_word_to)
    						&& !isLinked(&node,childs->node)) {
    					if (buildStanfordLink(STANFORD_DEP_MWE, childs->node->lexical_head, node.lexical_head)) {
    						addLinked(&node,childs->node);
    					    return true;
    					}
    				}
    				childs=childs->next;
    			}
    		}
    	}
    }

//"ADVP < (RB|IN < well) < (IN|RB|JJS=target < as)",
    inline const bool &mwe2(const unsigned long &cons) {
    	if (cons==PENN_CON_ADVP){
    		bool firstCond=false;
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		bool firstCondition=false;
    		while(childs!=0){
    			if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB)
    					|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN)) && ((*words)[childs->node->lexical_head].word==g_word_well)) {
    				firstCondition=true;
    			}
    			childs=childs->next;
    		}

    		if (firstCondition){
    			childs=node.m_umbinarizedSubNodes;
    			while(childs!=0){
    				if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB)
    						|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE) || ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN))
    						&& ((*words)[childs->node->lexical_head].word==g_word_as) && !isLinked(&node,childs->node)) {
    					if (buildStanfordLink(STANFORD_DEP_MWE, childs->node->lexical_head, node.lexical_head)) {
    						addLinked(&node,childs->node);
    					    return true;
    					}
    				}
    				childs=childs->next;
    			}
    		}
    	}
    }

//"ADVP < (DT < all) < (CC < but)",
inline const bool &mwe3(const unsigned long &cons) {
	//no =target, thus no rule...
}

//"CONJP < (RB < rather|well|instead) < (RB|IN=target < as|than|of)",
inline const bool &mwe4(const unsigned long &cons) {
    	if (cons==PENN_CON_CONJP){
    		bool firstCond=false;
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		bool firstCondition=false;
    		while(childs!=0){
    			if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB))
    					 && ((*words)[childs->node->lexical_head].word==g_word_well || (*words)[childs->node->lexical_head].word==g_word_rather || (*words)[childs->node->lexical_head].word==g_word_instead)) {
    				firstCondition=true;
    			}
    			childs=childs->next;
    		}

    		if (firstCondition){
    			childs=node.m_umbinarizedSubNodes;
    			while(childs!=0){
    				if ((((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB)
    						|| ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN))
    						&& ((*words)[childs->node->lexical_head].word==g_word_as || (*words)[childs->node->lexical_head].word==g_word_than || (*words)[childs->node->lexical_head].word==g_word_of)
    						&& !isLinked(&node,childs->node)) {
    					if (buildStanfordLink(STANFORD_DEP_MWE, childs->node->lexical_head, node.lexical_head)) {
    						addLinked(&node,childs->node);
    					    return true;
    					}
    				}
    				childs=childs->next;
    			}
    		}
    	}
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
	//what is xs?
	//no xs, thus, no rule.
}
