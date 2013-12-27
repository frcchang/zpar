
//"QP < IN|RB|RBR|RBS|PDT|DT|JJ|JJR|JJS|XS=target"
inline const bool & quantmod1(const unsigned long &cons){
	if (cons==PENN_CON_QP){
		CStateNodeList* childsQp=node.m_umbinarizedSubNodes;
		while(childsQp!=0){
			const CStateNode* targ=childsQp->node;
			if ((((*words)[targ->lexical_head].tag.code()==PENN_TAG_IN) || 
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE)||
				    			((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)) && !(isLinked(&node,targ))) {
				
				
//				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_QUANTMOD);
				if (buildStanfordLink(STANFORD_DEP_QUANTMOD, targ->lexical_head, node.lexical_head)) {
					addLinked(&node,targ);
					return true;
				}
				    		
			}
			childsQp=childsQp->next;
		}
	}
	return false;
}
