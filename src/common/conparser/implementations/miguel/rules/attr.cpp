
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

}
//"SBARQ <, (WHNP|WHADJP=target !< WRB) <+(SQ|SINV|S|VP) (VP !< (S < (VP < TO)) < (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
bool buildAttr3() {

}
//"SQ <, (/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP=target $-- (NP !< EX))"
bool buildAttr4() {

}



