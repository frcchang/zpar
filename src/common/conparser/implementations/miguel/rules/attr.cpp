
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
       	  if (node.constituent==PENN_CON_SBARQ){

       		  bool secFinalCond=false;
       		  CConstituentList* cl=new CConstituentList();
       		  CConstituentList::add(cl,PENN_CON_SQ);
       		  CConstituentList::add(cl,PENN_CON_SINV);
       		  CConstituentList::add(cl,PENN_CON_S);
       		  CConstituentList::add(cl,PENN_CON_VP);


       		  CStateNodeList* vpsChain=new CStateNodeList();
       		  findChainMultiCategory(cl, PENN_CON_VP, &node, vpsChain);
       		  bool chainCond=false;
       		  bool firstCond=true; //!< (S < (VP < TO))
       		  bool secCond=false; //< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD))
       		  bool thirdCond=true; //!<- PRT
       		  bool fourthCond=true; //!<- (PP <: IN)
       		  bool fifthCond=false; //$-- (NP !< /^-NONE-$/)

       		  while(vpsChain!=0){
       			  const CStateNode* parent=findParent(&node, vpsChain->node);
       			  if (parent!=0){
       				  CStateNodeList* childsParent=parent->m_umbinarizedSubNodes;
       				  while(childsParent!=0){
       					  if (childsParent->node==vpsChain->node){
       						  CStateNodeList* leftSisters=childsParent->previous;
       						  while(leftSisters!=0){
       							  if (leftSisters->node->constituent==PENN_CON_NP){
       								  CStateNodeList* childsNp=leftSisters->node->m_umbinarizedSubNodes;
       								  bool noneCond=true;
       								  while(childsNp!=0){
       									  if (childsNp->node->constituent==PENN_CON_NONE){
       										  noneCond=false;
       									  }
       									  childsNp=childsNp->next;
       								  }
       								  if (noneCond){
       									  fifthCond=true;
       								  }
       							  }
       							  leftSisters=leftSisters->previous;
       						  }
       					  }
       					  childsParent=childsParent->next;
       				  }
       			  }
       			  if (fifthCond){
       				  CStateNodeList* childs=vpsChain->node->m_umbinarizedSubNodes;
       				  while(childs!=0){
       					  if (childs->next==0 && childs->node->constituent==PENN_CON_PRN){
       						  thirdCond=false;
       					  }
       					  else if (childs->next==0 && childs->node->constituent==PENN_CON_PP){
       						  CStateNodeList* childsPP=childs->node->m_umbinarizedSubNodes;
       						  while(childsPP!=0){
       							  if ((*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_IN){
       								  fourthCond=false;
       							  }
       							  childsPP=childsPP->next;
       						  }
       					  }
       					  else if (childs->node->constituent==PENN_CON_S){ //!< (S < (VP < TO))
       						  CStateNodeList* childsS=childs->node->m_umbinarizedSubNodes;
       						  while(childsS!=0){
       							  if (childsS->node->constituent==PENN_CON_VP){
       								  CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
       								  while(childsVp!=0){
       									  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
       										  firstCond=false;
       									  }
       									  childsVp=childsVp->next;
       								  }
       							  }
       							  childsS=childsS->next;
       						  }

       					  }
       					  //PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE
       					  else if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB ||  //(/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD))
       							  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
       							  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
       							  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE ||
       							  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES ||
       							  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) 	{
       						  bool copCond=false;
       						  bool rightSisCond=false;

       						  CStateNodeList* rightSisters=childs->next;
       						  while(rightSisters!=0){
       							  if (rightSisters->node->constituent==PENN_CON_VP){
       								  CStateNodeList* childsVp=rightSisters->node->m_umbinarizedSubNodes;
       								  while(childsVp!=0){
       									  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE || (*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST){
       										  rightSisCond=true;
       									  }
       									  childsVp=childsVp->next;
       								  }
       							  }
       							  rightSisters=rightSisters->next;
       						  }

       						  CStateNodeList* childsVb=childs->node->m_umbinarizedSubNodes;
       						  while(childsVb!=0){
       							  if (compareWordToCopularWordRegex((*words)[childsVb->node->lexical_head].word)){
       								  copCond=true;
       							  }
       							  childsVb=childsVb->next;
       						  }
       						  if (copCond && rightSisCond){
       							  secCond=true;
       						  }
       					  }
       					  childs=childs->next;
       				  }
       			  }

       			  vpsChain=vpsChain->next;
       		  }
       		  if (firstCond && secCond && thirdCond && fourthCond && fifthCond){
       			  CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
       			  if (childsSbar!=0){
       				 const CStateNode* targ=childsSbar->node;
       				 if ((targ->constituent==PENN_CON_WHNP || targ->constituent==PENN_CON_WHADJP) && isLinked(&node,targ)){
       					 bool wrbCond=true;
       					 CStateNodeList* childsTarg=targ->m_umbinarizedSubNodes;
       					 while(childsTarg!=0){
       						 if ((*words)[childsTarg->node->lexical_head].tag.code()==PENN_TAG_TO){
       							 wrbCond=false;
       						 }
       						 childsTarg=childsTarg->next;
       					 }
       					 if (wrbCond){
       						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ATTR);
       						 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
       							 addLinked(&node,targ);
       						     return true;
       						 }
       					 }
       				 }
       			  }
       		  }
       	  }
       	  return false;
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



