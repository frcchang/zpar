
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
    					leftSisters->previous;
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
    				    rightSisters->next;
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
}
//"SBARQ <, (WHNP|WHADJP=target !< WRB) <+(SQ|SINV|S|VP) (VP !< (S < (VP < TO)) < (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
bool buildAttr3() {

}
//"SQ <, (/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP=target $-- (NP !< EX))"
  bool buildAttr4() {
  	//A <, B  (B is the FIRST child of A)
  	if (node.constituent==PENN_CON_SQ) {
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
  	return false;
  }



