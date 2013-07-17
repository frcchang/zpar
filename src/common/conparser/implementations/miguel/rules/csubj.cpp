    
    //"S < (SBAR|S=target !$+ /^,$/ $++ (VP !$-- NP))"
    bool csubj1(){
    	if (node.constituent==PENN_CON_S){
    		CStateNodeList* childsS=node.m_umbinarizedSubNodes;
    		while (childsS!=0){
    			const CStateNode* sbarsTarg=childsS->node;
    			if ((sbarsTarg->constituent==PENN_CON_SBAR ||sbarsTarg->constituent==PENN_CON_S) && (!isDangling(&node,sbarsTarg))){ 
    				bool firstCondition=false;
    				bool secondCondition=false;
    				if (childsS->next!=0){
    					if ((*words)[childsS->next->node->lexical_head].word==g_word_comma) {
    						firstCondition=true;
    					}
    				}
    				if (firstCondition) {
    					CStateNodeList* sistersSbarS=childsS->next;
    					while(sistersSbarS!=0){
    						const CStateNode* vp=sistersSbarS->node;
    						if (vp->constituent==PENN_CON_VP){
    							bool leftNp=false;
    							CStateNodeList* sistersVP=sistersSbarS->previous;
    							while(sistersVP!=0){
    								if (sistersVP->node->constituent==PENN_CON_NP){
    									leftNp=true;
    								}
    								sistersVP=sistersVP->previous;
    							}
    							
    							if (!leftNp) secondCondition=true;
    						}
    						sistersSbarS=sistersSbarS->next;
    					}
    				}
    				
    				if (firstCondition && secondCondition){
    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_CSUBJ);
    					 if (buildStanfordLink(label, sbarsTarg->lexical_head, node.lexical_head)) {
    						 addDangling(&node,sbarsTarg);
    					     return true;
    					}   
    				}
    			}
    			childsS=childsS->next;
    		}
    	}
    	return false;
    }
    
    