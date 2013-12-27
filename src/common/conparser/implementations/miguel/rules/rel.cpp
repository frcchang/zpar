    //"SBAR <, WHNP|WHPP|WHADJP=target > /^NP/ [ !<, /^WHNP/ | < (S < (VP $-- (/^NP/ !< /^-NONE-$/)))]"
inline const bool &rel1(const unsigned long &cons){
    	 if (cons==PENN_CON_NP){ //this is not the head.
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 const CStateNode* sbarHead=childsNp->node;
    			 if (CConstituent::clearTmp(sbarHead->constituent.code())==PENN_CON_SBAR){
    				 bool cond1=true;
    				 bool cond2=false;
    				 CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
    				 if (childsSbar!=0){
    					 if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHNP){
    						 cond1=false;
    					 }
    				 }
    				 if (!cond1){
    					 while(childsSbar!=0){
    						 if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_S){
    							 CStateNodeList* childsS=childsSbar->node->m_umbinarizedSubNodes;
    							 while(childsS!=0){
    								 if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
    									 CStateNodeList* leftSisters=childsS->previous;
    									 while(leftSisters!=0){

    										 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
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
    						 if ((CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHNP ||CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHPP ||CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHADJP) && !isLinked(&node,whTarg)){
//    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_REL);
    							 if (buildStanfordLink(STANFORD_DEP_REL, whTarg->lexical_head, sbarHead->lexical_head)) {
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

