
 //"VP < NP=target <(/^(?:VB|AUX)/ < " + copularWordRegex + ") !$ (NP < EX)",
    bool buildAttr1() {
    	if (node.constituent==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while (childsVp!=0){
    			const CStateNode* npTarg=childsVp->node;
    			if (npTarg->constituent==PENN_CON_NP && !(isLinked(&node, npTarg))){
    				//1st check whether there is no sister that looks like (NP < EX)
    				bool secondCondition=true;
    				CStateNodeList* leftSisters=childsVp->previous;
    				while(leftSisters!=0){
    					const CStateNode* npLeftSister=leftSisters->node;
    					if (npLeftSister->constituent==PENN_CON_NP){
    						CStateNodeList* childsExofNp=npLeftSister->m_umbinarizedSubNodes;
    						while(childsExofNp!=0){  //Big doubt: do I have to check the childs of NP, or the EX is the lexical head of the NP?????? Yue?
    							const CStateNode* childEx=childsExofNp->node;
    							if ((*words)[childEx->lexical_head].tag.code()==PENN_TAG_EX) {
    								secondCondition=false;
    							}
    							childsExofNp=childsExofNp->next;
    						}
    					}
    					leftSisters=leftSisters->previous;
    				}
    				CStateNodeList* rightSisters=childsVp->next;
    				while(rightSisters!=0){
    					const CStateNode* npRightSister=rightSisters->node;
    				    if (npRightSister->constituent==PENN_CON_NP){
    				    	CStateNodeList* childsExofNp=npRightSister->m_umbinarizedSubNodes;
    				    	while(childsExofNp!=0){
    				    		const CStateNode* childEx=childsExofNp->node;
    				    		if ((*words)[childEx->lexical_head].tag.code()==PENN_TAG_EX) {
    				    			secondCondition=false;
    				    		}
    				    		childsExofNp=childsExofNp->next;
    				    	}
    				    }
    				    rightSisters=rightSisters->next;
    				}

    				if (secondCondition){
    					//now, check whether NP=target <(/^(?:VB|AUX)/ < " + copularWordRegex + ")
    					CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
    					while(childsNp!=0){
    						 const CStateNode* vb=childsNp->node;
    						 if (((*words)[vb->lexical_head].tag.code()==PENN_TAG_VERB)) {
    							 CStateNodeList* childsVB=vb->m_umbinarizedSubNodes;
    						     while(childsVB!=0){
    						    	 if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
    						    		 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ATTR);
    						    		 if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
    						    			 addLinked(&node,npTarg);
    						    		     return true;
    						    		 }
    						    	 }
    						    	 childsVB=childsVB->next;
    						     }
    						 }
    						 childsNp=childsNp->next;
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;
    }

    //"SBARQ < (WHNP|WHADJP=target $+ (SQ < (/^(?:VB|AUX)/ < " + copularWordRegex + " !$++ VP) !< (VP <- (PP <:IN)) !<- (PP <: IN)))
    bool buildAttr2() {
    // A<:B (B is the only child of A)
    //A <- B (B is the last child of A)
    // A $+ B (A is the immediate left sister of B)
    	if (node.constituent==PENN_CON_SBARQ){
    		CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
    		while(childsSbarq!=0){
    			const CStateNode* whTarg=childsSbarq->node;
    			if ((whTarg->constituent==PENN_CON_WHNP || whTarg->constituent==PENN_CON_WHADJP) && (!isLinked(&node,whTarg))) {
    				if (childsSbarq->previous!=0) {
    					const CStateNode* sqSisterWh=childsSbarq->previous->node;
    					if (sqSisterWh->constituent==PENN_CON_SQ) {
    						bool secondCondition=true;
    						bool thirdCondition=true;
    						CStateNodeList* childsSq=sqSisterWh->m_umbinarizedSubNodes;
    						while(childsSq!=0){
    							const CStateNode* vpChildSq=childsSq->node;
    							if (vpChildSq->constituent==PENN_CON_VP) {
    								CStateNodeList* childsVp=vpChildSq->m_umbinarizedSubNodes;
    								while(childsVp!=0){
    									const CStateNode* ppChildVp=childsVp->node;
    									if (childsVp->next==0 && ppChildVp->constituent==PENN_CON_PP) { //it is the last child of
    										CStateNodeList* childsPP=ppChildVp->m_umbinarizedSubNodes;
    										if (childsPP->next==0 && ((*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_IN)){
    											secondCondition=false;
    										}
    									}
    									childsVp=childsVp->next;
    								}
    							}
    							childsSq=childsSq->next;
    						}
    						if (secondCondition) {
    							childsSq=sqSisterWh->m_umbinarizedSubNodes;
    							while(childsSq!=0) {
    								const CStateNode* ppChildSq=childsSq->node;
    								if (childsSq->next==0 && ppChildSq->constituent==PENN_CON_PP){
    									CStateNodeList* childsPP=ppChildSq->m_umbinarizedSubNodes;
    									if (childsPP->next==0 && ((*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_IN)){
    										thirdCondition=false;
    									}
    								}
    								childsSq=childsSq->next;
    							}
    						}
    						if (secondCondition && thirdCondition){
    							//(SQ < (/^(?:VB|AUX)/ < " + copularWordRegex + " !$++ VP)
    							CStateNodeList* childsSq=sqSisterWh->m_umbinarizedSubNodes;
    							while(childsSq!=0){
    								const CStateNode* vbChildSq=childsSq->node;

    								if ((*words)[vbChildSq->lexical_head].tag.code()==PENN_TAG_VERB) {
    									const CStateNodeList* childsVb=vbChildSq->m_umbinarizedSubNodes;
    									while(childsVb!=0){
    										if ((compareWordToCopularWordRegex((*words)[childsVb->node->lexical_head].word))) {
    											//A $++ B (A is a left sister of B)
    											bool insideCondition=true;
    											CStateNodeList* sistersVb=childsSq->next;
    											while(sistersVb!=0){
    												const CStateNode* vpSisterVb=sistersVb->node;
    												if (vpSisterVb->constituent==PENN_CON_VP){
    													insideCondition=false;
    												}
    												sistersVb=sistersVb->next;
    											}
    											if (insideCondition){
    												CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ATTR);
    												if (buildStanfordLink(label, whTarg->lexical_head, node.lexical_head)) {
    													addLinked(&node,whTarg);
    												    return true;
    												}
    											}
    										}
    										childsVb=childsVb->next;
    									}
    								}
    								childsSq=childsSq->next;
    							}
    						}
    					}
    				}
    			}
    			childsSbarq=childsSbarq->next;
    		}
    	}
    	return false;
    }

//"SBARQ <, (WHNP|WHADJP=target !< WRB) <+(SQ|SINV|S|VP) (VP !< (S < (VP < TO)) < (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
bool buildAttr3() {

}
//"SQ <, (/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP=target $-- (NP !< EX))"
  bool buildAttr4() {
  	//A <, B  (B is the FIRST child of A)
  	if (node.constituent==PENN_CON_SQ) {
  		if (node.m_umbinarizedSubNodes!=0) {
  		const CStateNode* firstChild=node.m_umbinarizedSubNodes->node;
  		bool firstCondition=false; //SQ<, (/^(?:VB|AUX)/ < " + copularWordRegex + ")
  		if (firstChild!=0 && ((*words)[firstChild->lexical_head].tag.code()==PENN_TAG_VERB)) {
  			CStateNodeList* childsVB=firstChild->m_umbinarizedSubNodes;
  			while(childsVB!=0){
  				if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
  					firstCondition=true;
  				}
  			    childsVB=childsVB->next;
  			}
  		}
  		if (firstCondition){
  			CStateNodeList* childsSQ=node.m_umbinarizedSubNodes;
  			while(childsSQ!=0){
  				const CStateNode* npTargChildSq=childsSQ->node;
  				if (npTargChildSq->constituent==PENN_CON_NP && (!isLinked(&node,npTargChildSq))) {
  					//A $--B (A is RIGHT sister of B)
  					CStateNodeList* leftSistersNp=childsSQ->previous;
  					while(leftSistersNp!=0){
  						const CStateNode* sisterNp=leftSistersNp->node;
  						if (sisterNp->constituent==PENN_CON_NP) {
  							CStateNodeList* childsNpNp=sisterNp->m_umbinarizedSubNodes;
  							bool noEx=true;
  							while(childsNpNp!=0) {
  								const CStateNode* exChildNp= childsNpNp->node;
  								if ((*words)[exChildNp->lexical_head].tag.code()==PENN_TAG_EX) {
  									noEx=false;
  								}
  								childsNpNp=childsNpNp->next;
  							}
  							if (noEx){
  								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ATTR);
  								if (buildStanfordLink(label, npTargChildSq->lexical_head, node.lexical_head)) {
  									addLinked(&node,npTargChildSq);
  								    return true;
  								}
  							}
  						}
  						leftSistersNp=leftSistersNp->previous;
  					}
  				}
  				childsSQ=childsSQ->next;
  			}
  		}
  	}
  }
  	return false;
  }



