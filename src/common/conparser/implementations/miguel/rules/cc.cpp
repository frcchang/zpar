//"__ [ < (CC=target !< /^(?i:either|neither|both)$/ ) | < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]"
    //I'm going to divide this one in 2, it is clearer and better.
    //1. "__ < (CC=target !< /^(?i:either|neither|both)$/ )
    //__ matches any node ()
    bool cc1(){
    	CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	while(childs!=0){
    		const CStateNode* ccTarg=childs->node;
    		if ((*words)[ccTarg->lexical_head].tag.code()==PENN_TAG_CC && (!isDangling(&node,ccTarg))) {
    			CStateNodeList* childsCC=ccTarg->m_umbinarizedSubNodes;
    			bool secondCondition=true;
    			while (childsCC!=0){
    				if (compareWordToEitherNeitherBoth((*words)[childsCC->node->lexical_head].word)) {
    					secondCondition=false;
    				}
    				childsCC=childsCC->next;
    			}
    			if (secondCondition) {
    				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CC);
    				if (buildStanfordLink(label, ccTarg->lexical_head, node.lexical_head)) {
    					//std::cout<<"nSubj2"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
    					addDangling(&node,ccTarg);
    					return true;
    				}
    			}
    		}
    		
    		childs=childs->next;
    	}
    }
    
    //2. "__ < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]
    /*bool cc2(){
    	CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	while(childs!=0) {
    		const CStateNode conjpTarg=childs->node;
    	}
    
    }*/