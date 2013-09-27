
    //VP < (NP=target !< /\\$/ !<# (/^NN/ < " + timeWordRegex + ") $+ (NP !<# (/^NN/ < " + timeWordRegex + ")))
    bool builIObj1() {
    	if (node.constituent==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* npTarg=childsVp->node;
    			if (npTarg->constituent==PENN_CON_NP && !(isLinked(&node,npTarg))){
    				bool firstCondition=true; //!< /\\$/
    				bool secondCondition=true; //!<# (/^NN/ < " + timeWordRegex + ")
    				CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
    				while(childsNp!=0){
    					const CStateNode* nnChildNp=childsNp->node;
    					if ((*words)[nnChildNp->lexical_head].word==g_word_slash){
    						firstCondition=false;
    					}
    					if ((nnChildNp->type==CStateNode::LEAF)
    							&& (nnChildNp->lexical_head==npTarg->lexical_head) //<#
    					    	&& (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
    					    	&& ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
    					    	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
    					    	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
    					    	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
    					    	secondCondition=false;
    					}
    					childsNp=childsNp->next;
    				}
    				if (!firstCondition && !secondCondition){
    					if (childsVp->previous!=0){
    						const CStateNode* npSister=childsVp->previous->node;
    						if (npSister->constituent==PENN_CON_NP){
    							CStateNodeList* childsNp2=npSister->m_umbinarizedSubNodes;
    							while(childsNp2!=0){
    								const CStateNode* nnChildNp2=childsNp2->node;
    							    if ((nnChildNp2->type==CStateNode::LEAF)
    							    		&& (nnChildNp2->lexical_head==npSister->lexical_head) //<#
    							    		&& (compareWordToTimeWordRegex((*words)[nnChildNp2->lexical_head].word))
    							    		&& ((((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN))
    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {

    							    		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
    							    	  	if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
    							    	  		addLinked(&node,npTarg);
    							    	  		return true;
    							    	  	}
    							    }
    							    childsNp2=childsNp2->next;
    							}
    						}
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    }


//"VP < (NP=target < (NP !< /\\$/ $++ (NP !< (/^NN/ < " + timeWordLotRegex + ")) !$ CC|CONJP !$ /^,$/ !$++ /^:$/))",
bool builIObj2() {

}

