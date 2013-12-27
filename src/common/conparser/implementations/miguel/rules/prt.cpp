
//VP < PRT=target"
inline const bool & prt1(const unsigned long &cons){
     	if (cons==PENN_CON_VP) {
     		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		while(childsVp!=0){
     			//I DON¡T know what prt is
     			childsVp=childsVp->next;
     		}
     	}
     }

//"ADJP < /^VB/ < RP=target"
inline const bool & prt2(const unsigned long &cons){
	if (cons==PENN_CON_ADJP) {
		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
		bool firstCondition=false;
	    while(childsVp!=0){
	    	//PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE
	    	if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB) || 
	    			((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||
	    			((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
	    			((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
	    			((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
	    			((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)) {
	    		firstCondition=true;
	    	}
	    	childsVp=childsVp->next;
	     }
	    if (firstCondition){
	    	childsVp=node.m_umbinarizedSubNodes;
	    	while (childsVp!=0){
	    		const CStateNode* rpTarg=childsVp->node;
	    		if (((*words)[rpTarg->lexical_head].tag.code()==PENN_TAG_RP)&& !(isLinked(&node,rpTarg))) {
//	    			CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PRT);
	    			if (buildStanfordLink(STANFORD_DEP_PRT, rpTarg->lexical_head, node.lexical_head)) {
	    				addLinked(&node,rpTarg);
	    			  	return true;
	    			}
	    		}
	    		childsVp=childsVp->next;
	    	}
	    }
	 }
}
