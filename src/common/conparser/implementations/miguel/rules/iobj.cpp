
    //VP < (NP=target !< /\\$/ !<# (/^NN/ < " + timeWordRegex + ") $+ (NP !<# (/^NN/ < " + timeWordRegex + ")))
    inline const bool &builIObj1(const unsigned long &cons) {
    	if (cons==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* npTarg=childsVp->node;
    			if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,npTarg))){
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
    				if (firstCondition && secondCondition){
    					if (childsVp->previous!=0){
    						const CStateNode* npSister=childsVp->previous->node;
    						if (CConstituent::clearTmp(npSister->constituent.code())==PENN_CON_NP){
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

//    							    		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_IOBJ);
    							    	  	if (buildStanfordLink(STANFORD_DEP_IOBJ, npTarg->lexical_head, node.lexical_head)) {
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
    	return false;
    }


    //"VP < (NP=target < (NP !< /\\$/ $++ (NP !< (/^NN/ < " + timeWordLotRegex + ")) !$ CC|CONJP !$ /^,$/ !$++ /^:$/))",
    inline const bool &builIObj2(const unsigned long &cons) {
        	if (cons==PENN_CON_VP){
        		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
        		while(childsVp!=0){
        			const CStateNode* npTarg=childsVp->node;
        			if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && (!isLinked(&node,npTarg))) {
        				CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
        				while(childsNp!=0){
        					const CStateNode* npChildNp=childsNp->node;
        					if (CConstituent::clearTmp(npChildNp->constituent.code())==PENN_CON_NP){
        						bool firstCondition=true; //!< /\\$/
        						bool thirdAndFourthConds=true; //!$ CC|CONJP !$ /^,$/
        						bool fifthCondition=true; //!$++ /^:$/
        						CStateNodeList* childsNp2=npChildNp->m_umbinarizedSubNodes;
        						while(childsNp2!=0){
        							if ((*words)[childsNp2->node->lexical_head].word==g_word_slash){
        								firstCondition=false;
        							}
        							childsNp2=childsNp2->next;
        						}
        						if (firstCondition){
        							//A $ B 	A is a sister of B (and not equal to B)
        							//first right, then left. A in this case is npChildNp.
        							CStateNodeList* rightSistersNp=childsNp->next;
        							while(rightSistersNp!=0){
        								if ((*words)[rightSistersNp->node->lexical_head].word==g_word_comma){
        									thirdAndFourthConds=false;
        								}
        								if (CConstituent::clearTmp(rightSistersNp->node->constituent.code())==PENN_CON_CONJP){
        									thirdAndFourthConds=false;
        								}
        								if ((*words)[rightSistersNp->node->lexical_head].tag.code()==PENN_TAG_CC) {
        									thirdAndFourthConds=false;
        								}
        								rightSistersNp=rightSistersNp->next;
        							}
        							CStateNodeList* leftSistersNp=childsNp->previous;
        							while(leftSistersNp!=0){
        								if ((*words)[leftSistersNp->node->lexical_head].word==g_word_comma){
        									thirdAndFourthConds=false;
        								}
        								if ((*words)[leftSistersNp->node->lexical_head].word==g_word_two_dots){
        									fifthCondition=false;
        								}
        								if (CConstituent::clearTmp(leftSistersNp->node->constituent.code())==PENN_CON_CONJP){
        									thirdAndFourthConds=false;
        								}
        								if ((*words)[leftSistersNp->node->lexical_head].tag.code()==PENN_TAG_CC) {
        									thirdAndFourthConds=false;
        								}
        								leftSistersNp=leftSistersNp->previous;
        							}

        						}
        						if (firstCondition && thirdAndFourthConds && fifthCondition) {
        							//internal part.
        							//$++ (NP !< (/^NN/ < " + timeWordLotRegex + "))
        							CStateNodeList* leftSistersNp=childsNp->previous;
        							while(leftSistersNp!=0){
        								const CStateNode* npSister=leftSistersNp->node;
        								if (CConstituent::clearTmp(npSister->constituent.code())==PENN_CON_NP){
        									bool nnCondition=true;
        									CStateNodeList* childsNp2=npSister->m_umbinarizedSubNodes;
        	    							while(childsNp2!=0){
        	    								const CStateNode* nnChildNp2=childsNp2->node;
        	    							    if ((nnChildNp2->type==CStateNode::LEAF)
        	    							    		&& (nnChildNp2->lexical_head==npSister->lexical_head) //<#
        	    							    		&& (compareWordToTimeLotWordRegex((*words)[nnChildNp2->lexical_head].word))
        	    							    		&& ((((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN))
        	    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
        	    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
        	    							    		||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {

        	    							    		nnCondition=false;
        	    							    }
        	    							    childsNp2=childsNp2->next;
        	    							}



        									if (nnCondition){
//        										CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_IOBJ);
        										if (buildStanfordLink(STANFORD_DEP_IOBJ, npTarg->lexical_head, node.lexical_head)) {
        											addLinked(&node,npTarg);
        										    return true;
        										}
        									}
        								}
        								leftSistersNp=leftSistersNp->previous;
        							}
        						}
        					}
        					childsNp=childsNp->next;
        				}
        			}
        			childsVp=childsVp->next;
        		}
        	}
        	return false;
        }

