     //"VP < (PRN=target < S|SINV|SBAR)",
     inline const bool &parataxis1(const unsigned long &cons){
     	if (cons==PENN_CON_VP){
     		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		while(childsVp!=0){
     			const CStateNode* prnTarg=childsVp->node;
     			if (CConstituent::clearTmp(prnTarg->constituent.code())==PENN_CON_PRN && !(isLinked(&node, prnTarg))){
     				CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
     				while(childsPrn!=0){
     					if (CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_S ||CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SINV
     							|| CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SBAR) {
//     						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
     						if (buildStanfordLink(STANFORD_DEP_PARATAXIS, prnTarg->lexical_head, node.lexical_head)) {
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
     inline const bool &parataxis2(const unsigned long &cons){
         	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
         	 while(childs!=0){
         		 const CStateNode* vpHead=childs->node;
         		 if (CConstituent::clearTmp(vpHead->constituent.code())==PENN_CON_VP){
         			 CStateNodeList* leftSisters=childs->previous;
         			 CStateNodeList* rightSisters=childs->next;
         			 while(leftSisters!=0){
         				 const CStateNode* prnTarg=leftSisters->node;
         				 if (CConstituent::clearTmp(prnTarg->constituent.code())==PENN_CON_PRN && !(isLinked(&node,prnTarg))){
         					 bool secCondition=false;
         					 bool thirdCondition=false;
         					 CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
         					 while(childsPrn!=0){
         						 if (CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_S ||CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SINV || CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SBAR) {
         							 secCondition=true;
         						 }
         						 else if (CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_VP){
         							 CStateNodeList* childsVp=childsPrn->node->m_umbinarizedSubNodes;
         							 while(childsVp!=0){
         								 if (CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_NP){
         									 thirdCondition=true;
         								 }
         								 childsVp=childsVp->next;
         							 }
         						 }
         						 childsPrn=childsPrn->next;
         					 }
         					 if (secCondition||thirdCondition){
//         						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
         						 if (buildStanfordLink(STANFORD_DEP_PARATAXIS, prnTarg->lexical_head, vpHead->lexical_head)) {
         							 addLinked(&node,prnTarg);
         						     return true;
         						 }
         					 }
         				 }
         				 leftSisters=leftSisters->previous;
         			 }
         			 while(rightSisters!=0){
         			 	 const CStateNode* prnTarg=rightSisters->node;
         			 	 if (CConstituent::clearTmp(prnTarg->constituent.code())==PENN_CON_PRN && !(isLinked(&node,prnTarg))){
         			 		 bool secCondition=false;
         			     	 bool thirdCondition=false;
         			    	 CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
         			    	 while(childsPrn!=0){
         			    		 if (CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_S ||CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SINV || CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_SBAR) {
         			    			 secCondition=true;
         			     		 }
         			     		 else if (CConstituent::clearTmp(childsPrn->node->constituent.code())==PENN_CON_VP){
         			     		 CStateNodeList* childsVp=childsPrn->node->m_umbinarizedSubNodes;
         			     		 while(childsVp!=0){
         			     			 if (CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_NP){
         			     				 thirdCondition=true;
         			     			 }
         			     			 childsVp=childsVp->next;
         			     		 }
         			     	 }
         			     	 childsPrn=childsPrn->next;
         			     }
         			     if (secCondition||thirdCondition){
//         			    	 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
         			     	 if (buildStanfordLink(STANFORD_DEP_PARATAXIS, prnTarg->lexical_head, vpHead->lexical_head)) {
         			     		 addLinked(&node,prnTarg);
         			     		 return true;
         			     	 }
         			     }
         			    }
         			    rightSisters=rightSisters->next;
         			 }
         		 }
         		 childs=childs->next;
         	 }
         	 return false;
          }
          

          //"S|VP < (/^:$/ $+ /^S/=target) !<, (__ $++ CC|CONJP)",
     inline const bool &parataxis3(const unsigned long &cons){
              	 if (cons==PENN_CON_S || cons==PENN_CON_VP){
              		 
              		 bool secondCondition=true;
              		 CStateNodeList* childsSVP=node.m_umbinarizedSubNodes;
              		 if (childsSVP!=0){
              			 CStateNodeList* rightSisters=childsSVP->next;
              			 while(rightSisters!=0){
              				 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_CONJP || ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC)) {
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
              						 if ((CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S
              								 ||CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_SBAR
              								 ||CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_SBARQ
              								 ||CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_SINV
              								 ||CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_SQ) && !(isLinked(&node,sTarg)) ){
//              							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
              							 if (buildStanfordLink(STANFORD_DEP_PARATAXIS, sTarg->lexical_head, node.lexical_head)) {
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
     inline const bool &parataxis4(const unsigned long &cons){
                  	 if (cons==PENN_CON_S){
                  		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
                  		 bool secCond=true;
                  		 while(childsS!=0){
                  			 if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_CONJP || ((*words)[childsS->node->lexical_head].tag.code()==PENN_TAG_CC)) {
                  				 secCond=false;
                  			 }
                  			 childsS=childsS->next;
                  		 }
                  		 if (secCond){
                  			 childsS=node.m_umbinarizedSubNodes;
                  		 		while(childsS!=0){
                  		 			if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_S){
                  		 				CStateNodeList* rightSisters=childsS->next;
                  		 				while(rightSisters!=0){
                  		 					const CStateNode* sTarg=rightSisters->node;
                  		 					if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && !isLinked(&node,sTarg)){
//                  		 						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
                  		 						if (buildStanfordLink(STANFORD_DEP_PARATAXIS, sTarg->lexical_head, node.lexical_head)) {
                  		 							addLinked(&node,sTarg);
                  		 						    return true;
                  		 						}
                  		 					}
                  		 					rightSisters=rightSisters->next;
                  		 				}
                  		 			}
                  		 			childsS=childsS->next;
                  		 		}
                  		 	}
                  	 	}
                  	 return false;
                   }

               
               
