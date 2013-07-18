//"__ [ < (CC=target !< /^(?i:either|neither|both)$/ ) | < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]"
    //I'm going to divide this one in 2, it is clearer and better.
    //1. "__ < (CC=target !< /^(?i:either|neither|both)$/ )
    //__ matches any node ()
    bool cc1(){
    	CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	while(childs!=0){
    		const CStateNode* ccTarg=childs->node;
    		std::cerr<<"cctarg 1\n";
    		if ((*words)[ccTarg->lexical_head].tag.code()==PENN_TAG_CC && (!isDangling(&node,ccTarg))) {
    			std::cerr<<"cctarg 2\n";
    			CStateNodeList* childsCC=ccTarg->m_umbinarizedSubNodes;
    			bool secondCondition=true;
    			while (childsCC!=0){
    				std::cerr<<"before\n";
    				std::cerr<<childsCC->node->lexical_head;
    				std::cerr<<"after\n";
    				if (compareWordToEitherNeitherBoth((*words)[childsCC->node->lexical_head].word)) {
    					secondCondition=false;
    				}
    				std::cerr<<"while-childscc-2\n";
    				childsCC=childsCC->next;
    			}
    			if (secondCondition) {
    				std::cerr<<"second condition\n";
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
    	return false;
    }
    
    
    //2. "__ < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]
    bool cc2(){
        	CStateNodeList* childs=node.m_umbinarizedSubNodes;
        	while(childs!=0) {
        		const CStateNode* conjpTarg=childs->node;
        		if (conjpTarg->constituent==PENN_CON_CONJP && (!isDangling(&node,conjpTarg))) {
        			bool secondCondition=true;
        			CStateNodeList* childsConj=conjpTarg->m_umbinarizedSubNodes;
        			while(childsConj!=0) {
        				const CStateNode* childRb=childsConj->node;
        				if ((*words)[childRb->lexical_head].tag.code()==PENN_TAG_ADVERB) {
        					CStateNodeList* childsRb=childRb->m_umbinarizedSubNodes;
        					while(childsRb!=0){
        						const CStateNode* childNot=childsRb->node;
        						if ((*words)[childNot->lexical_head].word==g_word_not) {
        							if (childsConj->next!=0){
        								const CStateNode* rightSisterRB=childsConj->next->node;
        								if (((*words)[rightSisterRB->lexical_head].tag.code()==PENN_TAG_ADVERB) 
        										|| ((*words)[rightSisterRB->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)) {
        									CStateNodeList* childsRbJJ=rightSisterRB->m_umbinarizedSubNodes;
        									while(childsRbJJ!=0){
        										if (compareWordToOnlyJustMerely((*words)[childsRbJJ->node->lexical_head].word)) {
        											secondCondition=false;
        										}
        										childsRbJJ=childsRbJJ->next;
        									}
        								}
        							}
        						}
        						childsRb=childsRb->next;
        					}
        					
        				}
        				
        				childsConj=childsConj->next;
        			}
        			
        			if (secondCondition) {
        				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CC);
        				if (buildStanfordLink(label, conjpTarg->lexical_head, node.lexical_head)) {
        					//std::cout<<"nSubj2"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
        					addDangling(&node,conjpTarg);
        					return true;
        				}
        			}
        		}
        		childs=childs->next;
        	}
        	return false;
        }