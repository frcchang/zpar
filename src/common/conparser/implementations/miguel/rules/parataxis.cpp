     //"VP < (PRN=target < S|SINV|SBAR)",
     bool parataxis1(){
     	if (node.constituent==PENN_CON_VP){
     		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		while(childsVp!=0){
     			const CStateNode* prnTarg=childsVp->node;
     			if (prnTarg->constituent==PENN_CON_PRN && !(isLinked(&node, prnTarg))){
     				CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
     				while(childsPrn!=0){
     					if (childsPrn->node->constituent==PENN_CON_S ||childsPrn->node->constituent==PENN_CON_SINV|| childsPrn->node->constituent==PENN_CON_SBAR) {
     						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
     						if (buildStanfordLink(label, prnTarg->lexical_head, node.lexical_head)) {
     							addLinked(&node,prnTarg);
     							return true;
     						}
     					}
     					childsPrn=childsPrn->next;
     				}
     			}
     			childsVp=childsVp->next;
     		}
     	}
     	return false;
     }
     //"VP $ (PRN=target [ < S|SINV|SBAR | < VP < @NP ] )"
          bool parataxis2(){
         	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
         	 while(childs!=0){
         		 const CStateNode* vpHead=childs->node;
         		 if (vpHead->constituent==PENN_CON_VP){
         			 CStateNodeList* leftSisters=childs->previous;
         			 CStateNodeList* rightSisters=childs->next;
         			 while(leftSisters!=0){
         				 const CStateNode* prnTarg=leftSisters->node;
         				 if (prnTarg->constituent==PENN_CON_PRN && !(isLinked(&node,prnTarg))){
         					 bool secCondition=false;
         					 bool thirdCondition=false;
         					 CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
         					 while(childsPrn!=0){
         						 if (childsPrn->node->constituent==PENN_CON_S ||childsPrn->node->constituent==PENN_CON_SINV || childsPrn->node->constituent==PENN_CON_SBAR) {
         							 secCondition=true;
         						 }
         						 else if (childsPrn->node->constituent==PENN_CON_VP){
         							 CStateNodeList* childsVp=childsPrn->node->m_umbinarizedSubNodes;
         							 while(childsVp!=0){
         								 if (childsVp->node->constituent==PENN_CON_NP){
         									 thirdCondition=true;
         								 }
         								 childsVp=childsVp->next;
         							 }
         						 }
         						 childsPrn=childsPrn->next;
         					 }
         					 if (secCondition||thirdCondition){
         						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
         						 if (buildStanfordLink(label, prnTarg->lexical_head, vpHead->lexical_head)) {
         							 addLinked(&node,prnTarg);
         						     return true;
         						 }
         					 }
         				 }
         				 leftSisters=leftSisters->previous;
         			 }
         			 while(rightSisters!=0){
         			 	 const CStateNode* prnTarg=leftSisters->node;
         			 	 if (prnTarg->constituent==PENN_CON_PRN && !(isLinked(&node,prnTarg))){
         			 		 bool secCondition=false;
         			     	 bool thirdCondition=false;
         			    	 CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
         			    	 while(childsPrn!=0){
         			    		 if (childsPrn->node->constituent==PENN_CON_S ||childsPrn->node->constituent==PENN_CON_SINV || childsPrn->node->constituent==PENN_CON_SBAR) {
         			    			 secCondition=true;
         			     		 }
         			     		 else if (childsPrn->node->constituent==PENN_CON_VP){
         			     		 CStateNodeList* childsVp=childsPrn->node->m_umbinarizedSubNodes;
         			     		 while(childsVp!=0){
         			     			 if (childsVp->node->constituent==PENN_CON_NP){
         			     				 thirdCondition=true;
         			     			 }
         			     			 childsVp=childsVp->next;
         			     		 }
         			     	 }
         			     	 childsPrn=childsPrn->next;
         			     }
         			     if (secCondition||thirdCondition){
         			    	 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
         			     	 if (buildStanfordLink(label, prnTarg->lexical_head, vpHead->lexical_head)) {
         			     		 addLinked(&node,prnTarg);
         			     		 return true;
         			     	 }
         			     }
         			    }
         			    rightSisters=rightSisters->previous;
         			 }
         		 }
         		 childs=childs->next;
         	 }
         	 return false;
          }
          

          //"S|VP < (/^:$/ $+ /^S/=target) !<, (__ $++ CC|CONJP)",
               bool parataxis3(){
              	 if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_VP){
              		 
              		 bool secondCondition=true;
              		 CStateNodeList* childsSVP=node.m_umbinarizedSubNodes;
              		 if (childsSVP!=0){
              			 CStateNodeList* rightSisters=childsSVP->next;
              			 while(rightSisters!=0){
              				 if (rightSisters->node->constituent==PENN_CON_CONJP || ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC)) {
              					 secondCondition=false;
              				 }
              				 rightSisters=rightSisters->next;
              			 }
              		 }
              		 if (secondCondition){
              			 childsSVP=node.m_umbinarizedSubNodes;
              			 while(childsSVP!=0){
              				 if ((*words)[childsSVP->node->lexical_head].word==g_word_two_dots) {
              					 if (childsSVP->next!=0){
              						 const CStateNode* sTarg=childsSVP->node;
              						 //PENN_CON_S, PENN_CON_SBAR, PENN_CON_SBARQ, PENN_CON_SINV, PENN_CON_SQ
              						 if ((sTarg->constituent==PENN_CON_S 
              								 ||sTarg->constituent==PENN_CON_SBAR 
              								 ||sTarg->constituent==PENN_CON_SBARQ 
              								 ||sTarg->constituent==PENN_CON_SINV
              								 ||sTarg->constituent==PENN_CON_SQ) && !(isLinked(&node,sTarg)) ){
              							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
              							 if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
              								 addLinked(&node,sTarg);
              							     return true;
              							 }
              						 }
              					 }
              				 }
              				 childsSVP=childsSVP->next;
              			 }
              		 }
              	 }
              	 return false;
               }



               
               //"@S < (@S $.. @S=target) !< @CC|CONJP",
               bool parataxis4(){

               }
