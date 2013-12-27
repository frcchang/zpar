//"__ [ < (CC=target !< /^(?i:either|neither|both)$/ ) | < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]"
    //I'm going to divide this one in 2, it is clearer and better.
    //1. "__ < (CC=target !< /^(?i:either|neither|both)$/ )
    //__ matches any node ()
inline const bool &cc1(const unsigned long &cons){
    	CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	while(childs!=0){
    		const CStateNode* ccTarg=childs->node;
    		if ((*words)[ccTarg->lexical_head].tag.code()==PENN_TAG_CC && (!isLinked(&node,ccTarg))) {
    			CStateNodeList* childsCC=ccTarg->m_umbinarizedSubNodes;
    			bool secondCondition=true;
    			if (compareWordToEitherNeitherBoth((*words)[ccTarg->lexical_head].word)) {
    				secondCondition=false;
    			}
    			if (secondCondition) {
//    				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CC);
    				if (buildStanfordLink(STANFORD_DEP_CC, ccTarg->lexical_head, node.lexical_head)) {
    					//std::cout<<"CC1\n";
    					addLinked(&node,ccTarg);
    					return true;
    				}
    			}
    		}
    		
    		childs=childs->next;
    	}
    	return false;
    }
    
    
    //2. "__ < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]
inline const bool &cc2(const unsigned long &cons){
        	CStateNodeList* childs=node.m_umbinarizedSubNodes;
        	while(childs!=0) {
        		const CStateNode* conjpTarg=childs->node;
        		if (CConstituent::clearTmp(conjpTarg->constituent.code())==PENN_CON_CONJP && (!isLinked(&node,conjpTarg))) {
        			bool secondCondition=true;
        			CStateNodeList* childsConj=conjpTarg->m_umbinarizedSubNodes;
        			while(childsConj!=0) {
        				const CStateNode* childRb=childsConj->node;
        				if ((*words)[childRb->lexical_head].tag.code()==PENN_TAG_ADVERB) {
        					CStateNodeList* childsRb=childRb->m_umbinarizedSubNodes;
        					//while(childsRb!=0){
        						//const CStateNode* childNot=childsRb->node;
        						if ((*words)[childRb->lexical_head].word==g_word_not) {
        							if (childsConj->next!=0){
        								const CStateNode* rightSisterRB=childsConj->next->node;
        								if (((*words)[rightSisterRB->lexical_head].tag.code()==PENN_TAG_ADVERB) 
        										|| ((*words)[rightSisterRB->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)) {
        									//CStateNodeList* childsRbJJ=rightSisterRB->m_umbinarizedSubNodes;
        									//while(childsRbJJ!=0){
        										if (compareWordToOnlyJustMerely((*words)[rightSisterRB->lexical_head].word)) {
        											secondCondition=false;
        										}
        										//childsRbJJ=childsRbJJ->next;
        									//}
        								}
        							}
        						}
        						//childsRb=childsRb->next;
        					//}
        					
        				}
        				
        				childsConj=childsConj->next;
        			}
        			
        			if (secondCondition) {
//        				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CC);
        				if (buildStanfordLink(STANFORD_DEP_CC, conjpTarg->lexical_head, node.lexical_head)) {
        					//std::cout<<"nSubj2"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
        					//std::cout<<"CC2\n";
        					addLinked(&node,conjpTarg);
        					return true;
        				}
        			}
        		}
        		childs=childs->next;
        	}
        	return false;
        }
