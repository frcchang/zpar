
//"VP < (@SBAR=target [ < (IN !< /^(?i:that|whether)$/) | <: (SINV <1 /^(?:VB|MD|AUX)/) | < (IN < that) < (RB|IN < so) ] )",

bool advcl1(){

}


//"S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/ !$+ VP)",
bool advcl2(){

}


//"S|SQ|SINV < (SBAR|SBAR-TMP=target <2 (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?$).*$/)",
bool advcl3(){

}

//"S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$+ @VP $+ /^,$/ $++ @NP)"
    bool advcl4(){

   	if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_SINV){
   		CStateNodeList* childsSSqS=node.m_umbinarizedSubNodes;
   		while(childsSSqS!=0){
   			const CStateNode* sbarTarg=childsSSqS->node;
   			if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))){
   				bool secondCondition=true;
   				bool thirdCondition=false;
   				bool fourthCondition=false;

   				if (childsSSqS->next!=0){
   					if (childsSSqS->next->node->constituent==PENN_CON_VP){
   						secondCondition=false;
   					}
   					else if ((*words)[childsSSqS->next->node->lexical_head].word==g_word_comma){
   						thirdCondition=true;
   					}
   				}
   				if (secondCondition && thirdCondition){
   					CStateNodeList* rightSisters=childsSSqS;
   					while(rightSisters!=0){
   						if (rightSisters->node->constituent==PENN_CON_NP){
   							fourthCondition=true;
   						}
   						rightSisters=rightSisters->next;
   					}
   				}

   				if (secondCondition && thirdCondition && fourthCondition){
   					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
   					if (childsSbar->next!=0){
   						const CStateNode* inChild=childsSbar->node;
   						if ((*words)[inChild->lexical_head].tag.code()==PENN_TAG_IN){
   							//(IN !< /^(?i:that|whether)$/ !$+ (NN < order))
   							bool secInCondition=true;
   							bool thirdInCondition=true;
   							CStateNodeList* childsIn=inChild->m_umbinarizedSubNodes;
   							while(childsIn!=0){
   								if (((*words)[childsIn->node->lexical_head].word==g_word_that) ||((*words)[childsIn->node->lexical_head].word==g_word_whether)){
   									secInCondition=false;
   								}
   								childsIn=childsIn->next;
   							}
   							if (childsSbar->next!=0){
   								const CStateNode* nnRightSis=childsSbar->next->node;
   								if ((*words)[nnRightSis->lexical_head].tag.code()==PENN_TAG_NOUN){
   									CStateNodeList* childsNN=nnRightSis->m_umbinarizedSubNodes;
   									while(childsNN!=0){
   										if ((*words)[childsNN->node->lexical_head].word==g_word_order){
   											thirdInCondition=false;
   										}
   										childsNN=childsNN->next;
   									}
   								}
   							}


   							if (secInCondition && thirdInCondition){
   								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   								if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
   									addLinked(&node,sbarTarg);
   								   	return true;
   								}
   							}
   						}
   					}
   				}

   			}
   			childsSSqS=childsSSqS->next;
   		}
   	}
   	return false;
    }


//"SBARQ < (SBAR|SBAR-TMP|SBAR-ADV=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) $+ /^,$/ $++ @SQ|S|SBARQ)",
bool advcl5(){

}

//"VP < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))", // added the (S < (VP <TO)) part so that "I tell them how to do so" doesn't get a
bool advcl6(){

}

//"S|SQ < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))",
     bool advcl7(){
    	 if (node.constituent==PENN_CON_S || node.constituent==PENN_CON_SQ){
    		 CStateNodeList* childsSSQ=node.m_umbinarizedSubNodes;
    		 while(childsSSQ!=0){
    			 const CStateNode* sbarTarg=childsSSQ->node;
    			 if (sbarTarg->constituent==PENN_CON_SBAR || !(isLinked(&node,sbarTarg))){
    				 CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
    				 bool secondCondition=true;
    				 while(childsSbar!=0){
    					 const CStateNode* sChildSbar=childsSbar->node;
    					 if (sChildSbar->constituent==PENN_CON_S){
    						 CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
    						 while(childsS!=0){
    							 if (childsS->node->constituent==PENN_CON_VP){
    								 CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
    								 while(childsVp!=0){
    									 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)) {
    										 secondCondition=false;
    									 }
    									 childsVp=childsVp->next;
    								 }
    							 }
    							 childsS=childsS->next;
    						 }
    					 }
    					 childsSbar=childsSbar->next;
    				 }
    				 if (secondCondition){
    					 childsSbar=sbarTarg->m_umbinarizedSubNodes;
    					 if (childsSbar!=0){
    						 //<, (WHADVP|WHNP < (WRB !< /^(?i:how)$/))
    						 const CStateNode* whChildS=childsSbar->node;
    						 if (whChildS->constituent==PENN_CON_WHADJP ||whChildS->constituent==PENN_CON_WHNP){
    							 CStateNodeList* childsWh=whChildS->m_umbinarizedSubNodes;
    							 while(childsWh!=0){
    								 const CStateNode* wrbChildwh=childsWh->node;
    								 if (((*words)[wrbChildwh->lexical_head].tag.code()==PENN_TAG_WRB)) {
    									 bool howCond=true;
    									 CStateNodeList* childsWrb=wrbChildwh->m_umbinarizedSubNodes;
    									 while(childsWrb!=0){
    										 if ((*words)[childsWrb->node->lexical_head].word==g_word_how){
    											 howCond=false;
    										 }
    										 childsWrb=childsWrb->next;
    									 }
    									 if (howCond){
    										 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
    										 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
    											 addLinked(&node,sbarTarg);
    										     return true;
    										 }
    									 }
    								 }
    								 childsWh=childsWh->next;
    							 }
    						 }

    					 }
    				 }

    			 }

    			 childsSSQ=childsSSQ->next;
    		 }
    	 }
    	 return false;
     }

//"@S < (@SBAR=target $++ @NP $++ @VP)",
    bool advcl8(){
   	 if (node.constituent==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sbarTarg=childsS->node;
   			 if (sbarTarg->constituent==PENN_CON_SBAR && !(isLinked(&node,sbarTarg))) {
   				 CStateNodeList* rightSistersSbar=childsS;
   				 bool firstCondition=false;
   				 while(rightSistersSbar!=0){
   					 if (rightSistersSbar->node->constituent==PENN_CON_NP){
   						 firstCondition=true;
   					 }
   					 rightSistersSbar=rightSistersSbar->next;
   				 }
   				 if (firstCondition){
   					 rightSistersSbar=childsS;
   					 while(rightSistersSbar!=0){
   						 if (rightSistersSbar->node->constituent==PENN_CON_VP){
   							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   							 if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
   								 addLinked(&node,sbarTarg);
   							     return true;
   							 }
   						 }
   						 rightSistersSbar=rightSistersSbar->next;
   					 }
   				 }
   			 }
   			 childsS=childsS->next;
   		 }
   	 }
   	 return false;
    }


    //"@S < (@S=target < (VP < TO) $+ (/^,$/ $++ @NP))",
    bool advcl9(){
   	 if (node.constituent==PENN_CON_S){
   		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
   		 while(childsS!=0){
   			 const CStateNode* sTarg=childsS->node;
   			 if (sTarg->constituent==PENN_CON_S && !(isLinked(&node,sTarg))){
   				 bool secondCondition=false;
   				 if (childsS->next!=0){
   					 const CStateNode* commaRightSister=childsS->next->node;
   					 if ((*words)[commaRightSister->lexical_head].word==g_word_comma){
   						 if (childsS->next->next!=0){
   							 CStateNodeList* rightSisters=childsS->next->next;
   							 while(rightSisters!=0){
   								 if (rightSisters->node->constituent==PENN_CON_NP){
   									 secondCondition=true;
   								 }
   								 rightSisters=rightSisters->next;
   							 }
   						 }
   					 }
   				 }
   				 if (secondCondition){
   					 CStateNodeList* childsSIn=sTarg->m_umbinarizedSubNodes;
   					 while(childsSIn!=0){
   						 const CStateNode* vpChildS=childsSIn->node;
   						 if (vpChildS->constituent==PENN_CON_VP){
   							 CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
   							 while(childsVp!=0){
   								 if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
   									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   									 if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
   										 addLinked(&node,sTarg);
   									     return true;
   									 }
   								 }
   								 childsVp=childsVp->next;
   							 }
   						 }
   						 childsSIn=childsSIn->next;
   					 }
   				 }

   			 }
   			 childsS=childsS->next;
   		 }

   	 }
   	 return false;
    }

