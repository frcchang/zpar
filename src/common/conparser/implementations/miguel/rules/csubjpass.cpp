
      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP < (VP < VBN|VBD) < (/^(?:VB|AUXG?)/ < " + passiveAuxWordRegex + ") !$-- NP))",
      bool csubjpass1(const unsigned long &cons){
    	  if (cons==PENN_CON_S){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (CConstituent::clearTmp(CConstituent::clearTmp(targ->constituent.code())==PENN_CON_SBAR
    					  || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_S)){
    				  bool commaCond=true;
    				  CStateNodeList* rightSisters=childs->next;
    				  if (rightSisters!=0){
    					  if ((*words)[rightSisters->node->lexical_head].word==g_word_comma){
    						  commaCond=false;
    					  }
    				  }
    				  if (commaCond){
    					  while(rightSisters!=0){
    						  if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_VP){
    							  bool leftSisCond=true;
    							  CStateNodeList* leftSisters=rightSisters->previous;
    							  while(leftSisters!=0){
    								  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
    									  leftSisCond=false;
    								  }
    								  leftSisters=leftSisters->previous;
    							  }
    							  if (leftSisCond){
    								  CStateNodeList* childsVp=rightSisters->node->m_umbinarizedSubNodes;
    								  bool firstCond=false;
    								  bool secCond=false;
    								  //< (VP < VBN|VBD) < (/^(?:VB|AUXG?)/ < " + passiveAuxWordRegex + ")
    								  while(childsVp!=0){
    									  if (CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_VP){
    										  CStateNodeList* childsVpIn=childsVp->node->m_umbinarizedSubNodes;
    										  while(childsVpIn!=0){
    											  if ((*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST
    													  || (*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){
    												  firstCond=true;
    											  }
    											  childsVpIn=childsVpIn->next;
    										  }
    									  }
    									  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB){
    										  if (compareWordToPassiveAuxWordRegex((*words)[childsVp->node->lexical_head].word)) {
    											  secCond=true;
    										  }
    									  }
    									  childsVp=childsVp->next;
    								  }
    								  
    								  if (firstCond && secCond){
//    									  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
    									  if (buildStanfordLink(STANFORD_DEP_CSUBJPASS, targ->lexical_head, node.lexical_head)) {
    										  addLinked(&node,targ);
    									  	  return true;
    									  }
    								  }
    							  }
    						  }
    						  rightSisters=rightSisters->next;
    					  }
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }
      

      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP <+(VP) (VP < VBN|VBD > (VP < (/^(?:VB|AUX)/ < " + passiveAuxWordRegex + "))) !$-- NP))"
      bool csubjpass2(const unsigned long &cons){
    	  if (cons==PENN_CON_S){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if ((CConstituent::clearTmp(targ->constituent.code())==PENN_CON_SBAR || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_S) && !isLinked(&node,targ)){
    				  bool commaCond=true;
    				  bool npCond=true;
    				  CStateNodeList* leftSisters=childs->previous;
    				  while(leftSisters!=0){
    					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
    						  npCond=false;
    					  }
    					  leftSisters=leftSisters->previous;
    				  }
    				  if (npCond&& childs->next!=0){
    					  if ((*words)[childs->next->node->lexical_head].word==g_word_comma){
    						  commaCond=false;
    					  }
    				  }
    				  if (commaCond && npCond){
    					  CStateNodeList* rightSisters=childs->next;
    					  while(rightSisters!=0){
    						  if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_VP){
    							  CStateNodeList* vpsChain=new CStateNodeList();
    							        						  //std::cout<<"findingchain\n";
    							  findChain(PENN_CON_VP,PENN_CON_VP, rightSisters->node, vpsChain);
    							  if (vpsChain->node==0) {
    								  vpsChain->clear();
    							      vpsChain=0;
    							  }
    							  while(vpsChain!=0){
    								  CStateNodeList* childsOfAVp=vpsChain->node->m_umbinarizedSubNodes;
    							      while(childsOfAVp!=0){
    							    	  if ((*words)[childsOfAVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE || (*words)[childsOfAVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST){
    							    		  const CStateNode* parent=findParent(rightSisters->node, childsOfAVp->node);
    							    		  if (parent!=0 && CConstituent::clearTmp(parent->constituent.code())==PENN_CON_VP){
    							    			  CStateNodeList* childsVp=parent->m_umbinarizedSubNodes;
    							    			  while(childsVp!=0){
    							    				  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB){
    							    					  //CStateNodeList* childsVb=childsVp->node->m_umbinarizedSubNodes;
    							    					  //while(childsVb!=0){
    							    						  if (compareWordToPassiveAuxWordRegex((*words)[childsVp->node->lexical_head].word)) {
//    							    							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
    							    							  if (buildStanfordLink(STANFORD_DEP_CSUBJPASS, targ->lexical_head, node.lexical_head)) {
    							    								  addLinked(&node,targ);
    							    							      return true;
    							    							  }
    							    						  }
    							    						  //childsVb=childsVb->next;
    							    					  //}
    							    				  }
    							    				  childsVp=childsVp->next;
    							    			  }
    							    		  }
    							    	  }
    							    	  childsOfAVp=childsOfAVp->next;
    							      }
    							      vpsChain=vpsChain->next;
    							  }
    						  }
    						  rightSisters=rightSisters->next;
    					  }
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }
