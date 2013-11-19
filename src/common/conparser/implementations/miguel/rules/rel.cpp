    //"SBAR <, WHNP|WHPP|WHADJP=target > /^NP/ [ !<, /^WHNP/ | < (S < (VP $-- (/^NP/ !< /^-NONE-$/)))]"
     bool rel1(){
    	 if (node.constituent==PENN_CON_NP){ //this is not the head.
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 const CStateNode* sbarHead=childsNp->node;
    			 if (sbarHead->constituent==PENN_CON_SBAR){
    				 bool cond1=true;
    				 bool cond2=false;
    				 CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
    				 if (childsSbar!=0){
    					 if (childsSbar->node->constituent==PENN_CON_WHNP){
    						 cond1=false;
    					 }
    				 }
    				 if (!cond1){
    					 while(childsSbar!=0){
    						 if (childsSbar->node->constituent==PENN_CON_S){
    							 CStateNodeList* childsS=childsSbar->node->m_umbinarizedSubNodes;
    							 while(childsS!=0){
    								 if (childsS->node->constituent==PENN_CON_VP){
    									 CStateNodeList* leftSisters=childsS->previous;
    									 while(leftSisters!=0){

    										 if (leftSisters->node->constituent==PENN_CON_NP){
    											CStateNodeList* childsL=leftSisters->node->m_umbinarizedSubNodes;
    											bool noneCondition=true;
    											while(childsL!=0){
    												if ((*words)[childsL->node->lexical_head].tag.code()==PENN_TAG_NONE){
    													noneCondition=false;
    												}
    												childsL=childsL->next;
    											}
    											if (noneCondition){
    												cond2=true;
    											}
    										 }
    										 leftSisters=leftSisters->previous;
    									 }
    								 }
    								 childsS=childsS->next;
    							 }
    						 }
    						 childsSbar=childsSbar->next;
    					 }
    				 }

    				 if (cond1 || cond2){
    					 childsSbar=sbarHead->m_umbinarizedSubNodes;
    					 while(childsSbar!=0){
    						 const CStateNode* whTarg=childsSbar->node;
    						 if ((whTarg->constituent==PENN_CON_WHNP ||whTarg->constituent==PENN_CON_WHPP ||whTarg->constituent==PENN_CON_WHADJP) && !isLinked(&node,whTarg)){
    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_REL);
    							 if (buildStanfordLink(label, whTarg->lexical_head, sbarHead->lexical_head)) {
    								 addLinked(&node,whTarg);
    							     return true;
    							 }
    						 }
    						 childsSbar=childsSbar->next;
    					 }
    				 }
    			 }
    			 childsNp=childsNp->next;
    		 }
    	 }
    	 return false;
     }

