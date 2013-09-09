
//"VP < (S=target < (VP !<, TO|VBG|VBN) !$-- NP)",
 //"VP < (S=target < (VP !<, TO|VBG|VBN) !$-- NP)",
    bool buildCComp1() {
    	if (node.constituent==PENN_CON_VP) {
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp=0){
    			const CStateNode* sTarg=childsVp->node;
    			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))) {
    				bool inCondition=true; //!<, TO|VBG|VBN
    				//A <, B B is the first child of A
    				CStateNodeList* childsS=node.m_umbinarizedSubNodes;
    				while(childsS!=0) {
    					const CStateNode* vpChildS=childsS->node;
    					if (vpChildS->constituent==PENN_CON_VP) {
    						CStateNodeList* childsVpIn=vpChildS->m_umbinarizedSubNodes;
    						if (childsVpIn!=0) { //only the first one
    							const CStateNode* firstChild=childsVpIn->node;
    							if (((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_TO)
    								 || ((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
    								 || ((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) {

    								inCondition=false;

    							}
    						}
    					}
    					childsS=childsS->next;
    				}
    				if (inCondition) {
    					//A $-- B (A is a right sister of B)
    					bool secondCondition=true;
    					CStateNodeList* leftSistersS=childsVp->previous;
    					while(leftSistersS!=0){
    						if (leftSistersS->node->constituent==PENN_CON_NP){
    							secondCondition=false;
    						}
    						leftSistersS=leftSistersS->previous;
    					}
    					if (secondCondition) {
    						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
    						if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
    							addLinked(&node,sTarg);
    						    return true;
    						}
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;
    }


//"VP < (SBAR=target < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/))",
bool buildCComp2() {}



//"VP < (SBAR=target < (SBAR < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/)) < CC|CONJP)",
bool buildCComp3() {}


//"VP < (SBAR=target < (S < VP) !$-- NP !<, (IN|WHADVP))",
 bool buildCComp4() {
 	if (node.constituent==PENN_CON_VP) {
 		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
 		while(childsVp!=0){
 			const CStateNode* sbarTarg=childsVp->node;
 			if (sbarTarg->constituent==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
 				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
 				bool secondCondition=true;
 				bool thirdCondition=true;

 				//A $-- B A is a right sister of B
 				CStateNodeList* leftSistersSbar=childsVp->previous;
 				while(leftSistersSbar!=0){
 					if (leftSistersSbar->node->constituent==PENN_CON_NP){
 						secondCondition=false;
 					}
 					leftSistersSbar=leftSistersSbar->previous;
 				}
 				//A !<, B 	B is the first child of A
 				if (childsSbar!=0) { //only the first child
 					if (childsSbar->node->constituent==PENN_CON_WHADJP || (*words)[childsSbar->node->lexical_head].tag.code()==PENN_TAG_IN) {
 						thirdCondition=false;
 					}
 				}
 				if (secondCondition && thirdCondition) {
 					while(childsSbar!=0) {
 						const CStateNode* sChildSbar=childsSbar->node;
 						if (sChildSbar->constituent==PENN_CON_S){
 							CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
 							while(childsS!=0){
 								const CStateNode* vpChildS=childsS->node;
 								if (vpChildS->constituent==PENN_CON_VP){
 									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
 									if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
 										addLinked(&node,sbarTarg);
 									    return true;
 									}
 								}
 								childsS=childsS->next;
 							}
 						}
 						childsSbar=childsSbar->next;
 					}
 				}

 			}
 			childsVp=childsVp->next;
 		}
 	}
 	return false;

 }
//"VP < (SBAR=target < (S < VP) !$-- NP <, (WHADVP < (WRB < /^(?i:how)$/)))",
bool buildCComp5() {}
//"VP < (/^VB/ < " + haveRegex + ") < (S=target < @NP < VP)",
bool buildCComp6() {}
//"S|SINV < (S|SBARQ=target $+ /^(,|\\.|'')$/ !$- /^(?:CC|:)$/ !< (VP < TO|VBG|VBN))",
bool buildCComp7() {}
//"ADVP < (SBAR=target < (IN < /^(?i:as|that)/) < (S < (VP !< TO)))"
bool buildCComp8() {}

//"ADJP < (SBAR=target !< (IN < as) < S)"
    bool buildCComp9() {
    	if (node.constituent==PENN_CON_ADJP){
    		CStateNodeList* childsAdjp=node.m_umbinarizedSubNodes;
    		while(childsAdjp!=0){
    			const CStateNode* sbarTarg=childsAdjp->node;
    			if (sbarTarg->constituent==PENN_CON_SBAR && (!isLinked(&node,sbarTarg))) {
    				bool firstCondition=true;
    				CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				while(childsSbar!=0){
    					const CStateNode* inChildSbar=childsSbar->node;
    					if ((*words)[inChildSbar->lexical_head].tag.code()==PENN_TAG_IN) {
    						CStateNodeList* childsIn=inChildSbar->m_umbinarizedSubNodes;
    						while(childsIn!=0){
    							if ((*words)[inChildSbar->lexical_head].word==g_word_as) {

    							}
    							childsIn=childsIn->next;
    						}
    					}
    					childsSbar=childsSbar->next;
    				}
    				if (firstCondition){
    					childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					while(childsSbar!=0){
    						const CStateNode* schildSbar=childsSbar->node;
    						if (schildSbar->constituent==PENN_CON_S){
    							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CCOMP);
    							 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,sbarTarg);
    							 	 return true;
    							}
    						}
    						childsSbar=childsSbar->next;
    					}
    				}
    			}
    			childsAdjp=childsAdjp->next;
    		}
    	}
    	return false;
    }

//"S <, (SBAR=target <, (IN < /^(?i:that|whether)$/) !$+ VP)",
bool buildCComp10() {}
//"@NP < JJ|NN|NNS < (SBAR=target [ !<(S < (VP < TO )) | !$-- NP|NN|NNP|NNS ] )"
bool buildCComp11() {}
