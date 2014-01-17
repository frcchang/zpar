/* 
    	"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"

          // match "give it next week"
          
          "VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
          
         // basic direct object cases
          "VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )",
          // cdm Jul 2010: No longer require WHNP as first child of SBARQ below: often not because of adverbials, quotes, etc., and removing restriction does no harm
          // this next pattern used to assume no empty NPs. Corrected.  If you adjust this pattern, also adjust the corresponding one for attr!
         
         "SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
         
          // matches direct object in relative clauses "I saw the book that you bought"
          "SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",

          // matches direct object in relative clauses "I saw the book that you said you bought"
          "SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",

          // matches direct object for long dependencies in relative clause without explicit relative pronouns
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
          
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"
*/



    /*
       *"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")"
       *
       */
       inline const bool & buildDobj1(const unsigned long &cons) {
    	   if (cons==PENN_CON_VP) {
    		   CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		   bool secondCondition=true;
    		   while(childsVp!=0){
    			   const CStateNode* vbChildVp=childsVp->node;
    			   if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
    				   CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
    			       while(childsVB!=0){
    			    	   if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
    			    		   secondCondition=false;
    			    	   }
    			       	   childsVB=childsVB->next;
    			       	}
    			   }
    			   childsVp=childsVp->next;
    		   }

    		   if (secondCondition) { //VP does not a have child that matches (/^(?:VB|AUX)/ < " + copularWordRegex + ")
    			   childsVp=node.m_umbinarizedSubNodes;
    			   while(childsVp!=0){
    				   const CStateNode* npChildVp=childsVp->node;

    				   if (CConstituent::clearTmp(npChildVp->constituent.code())==PENN_CON_NP) {
    					   	  //now the immediate left sister must look like (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))
    					   	   CStateNodeList* previous=childsVp->previous;
    					   	   if (previous!=0) {
    					   		   const CStateNode* leftSisterNpTarg=previous->node;

    					   		   if ((CConstituent::clearTmp(leftSisterNpTarg->constituent.code())==PENN_CON_NP
    					   				   || CConstituent::clearTmp(leftSisterNpTarg->constituent.code())==PENN_CON_WHNP)
    					   				   && !(isLinked(&node, leftSisterNpTarg))) {
    					   			   bool noChildNNTime=true;
    					   			   CStateNodeList* childsOfTarg=leftSisterNpTarg->m_umbinarizedSubNodes;
    					   			   while(childsOfTarg!=0){
    					   				   const CStateNode* nnChildNp=childsOfTarg->node;
    					   				   if ((nnChildNp->type==CStateNode::LEAF)
    					   					   && (nnChildNp->lexical_head==leftSisterNpTarg->lexical_head) //<#
    					   					   && (compareWordToTimeLotWordRegex((*words)[nnChildNp->lexical_head].word))
    					   					   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
    					   					   noChildNNTime=false;
    					   					}
    					   				   childsOfTarg=childsOfTarg->next;
    					   			   }


    					   			   if (noChildNNTime) {
//    					   				   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
    					   				   if (buildStanfordLink(STANFORD_DEP_DOBJ, leftSisterNpTarg->lexical_head, node.lexical_head)) {
    					   					   addLinked(&node,leftSisterNpTarg);
    					   					   return true;
    					   				   }
    					   			   }
    					   	   	   }
    					   	   }

    				   }
    				   childsVp=childsVp->next;
    			   }
    		   }
    	   }
    	   return false;

       }

       //"VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")"
       inline const bool &buildDobj2(const unsigned long &cons) {
          	if (cons==PENN_CON_VP) {
          		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
          		bool secondCondition=true;
          		while(childsVp!=0){
          			const CStateNode* vbChildVp=childsVp->node;
          		    if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
          		    	CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
          		    	//while(childsVB!=0){
          		    		if ((compareWordToCopularWordRegex((*words)[vbChildVp->lexical_head].word))) {
          		    			secondCondition=false;
          		    		}
          		    		//childsVB=childsVB->next;
          		    	//}
          		    }
          		    childsVp=childsVp->next;
          		}
          		if (secondCondition){
          			childsVp=node.m_umbinarizedSubNodes;
          			while(childsVp!=0){
          				const CStateNode* fstnpChildVp=childsVp->node;
          				if (CConstituent::clearTmp(fstnpChildVp->constituent.code())==PENN_CON_NP) {
          					CStateNodeList* childsNP=fstnpChildVp->m_umbinarizedSubNodes;
          					while(childsNP!=0){
          						const CStateNode* npChildVp=childsNP->node;
          						if (CConstituent::clearTmp(npChildVp->constituent.code())==PENN_CON_NP) {
          						//now the immediate left sister must look like (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))
          							CStateNodeList* previous=childsVp->previous;
          						    if (previous!=0) {
          						    	const CStateNode* leftSisterNpTarg=previous->node;
          						    	if ((CConstituent::clearTmp(leftSisterNpTarg->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(leftSisterNpTarg->constituent.code())==PENN_CON_WHNP) && !(isLinked(&node, leftSisterNpTarg))) {
          						    		bool noChildNNTime=true;
          						    		CStateNodeList* childsOfTarg=leftSisterNpTarg->m_umbinarizedSubNodes;
          						    		while(childsOfTarg!=0){
          						    			const CStateNode* nnChildNp=childsOfTarg->node;
          						    			if ((nnChildNp->type==CStateNode::LEAF)
          						    					&& (nnChildNp->lexical_head==leftSisterNpTarg->lexical_head) //<#
          						    					&& (compareWordToTimeLotWordRegex((*words)[nnChildNp->lexical_head].word))
          						    					&& ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
          						    					||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
          						    						noChildNNTime=false;
          						    			}
          						    			childsOfTarg=childsOfTarg->next;
          						    		}
          						            if (noChildNNTime) {
//          						            	CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
          						    			if (buildStanfordLink(STANFORD_DEP_DOBJ, leftSisterNpTarg->lexical_head, node.lexical_head)) {
          						    				addLinked(&node,leftSisterNpTarg);
          						    				return true;
          						    			}
          						    		}
          						    	}
          						    }
          						    				   }
          						childsNP=childsNP->next;
          					}
          				}
          				childsVp=childsVp->next;
          			}
          		}
          	}
          	return false;

          }

          //"VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )"
       inline const bool &buildDobj3(const unsigned long &cons) {
             	 if (cons==PENN_CON_VP) {
             		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
             	     bool firstCondition=true;
             	     while(childsVp!=0){
             	     const CStateNode* vbChildVp=childsVp->node;
             	     //PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
             	     	 if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PAST)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_PRES)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)
             	     			 || ((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
             	     		 //CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
             	     	 	 //while(childsVB!=0){
             	     	 		 if ((compareWordToCopularWordRegex((*words)[vbChildVp->lexical_head].word))) {
             	     	 			 firstCondition=false;
             	     			  }
             	     			  //childsVB=childsVB->next;
             	     	 	 //}
             	     	 }
             	     	 childsVp=childsVp->next;
             	     }
             	     if (firstCondition){
             	    	 childsVp=node.m_umbinarizedSubNodes;
             	         while(childsVp!=0){
             	        	 const CStateNode* targ=childsVp->node;
             	        	 if ((CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP
             	        			 || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHNP) && !isLinked(&node,targ)){
             	        		 bool firstConditionIn=true; //[ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ]
             	        		 bool secCondition=false; //$+ NP-TMP it will be false, ever and ever, there is no np-tmp in our data set.
             	        		 bool thirdCondition=false; //$+ (NP <# (/^NN/ < " + timeWordRegex + "))

             	        		 CStateNodeList* childsNp=targ->m_umbinarizedSubNodes;
             	        		 bool nnTime=false;
             	        		 while(childsNp!=0){
             	        			 const CStateNode* nnChildNp=childsNp->node;
             	        			 if ((nnChildNp->type==CStateNode::LEAF)
             	        			 	&& (nnChildNp->lexical_head==targ->lexical_head) //<#
             	        			 	&& (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
             	        			 	&& ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
             	        			 	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
             	        			 	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
             	        			 	||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
             	        				 firstConditionIn=false;
             	        			 }
             	        			 childsNp=childsNp->next;
             	        		 }
             	        		 CStateNodeList* rightSisters=childsVp->next;
             	        		 if (rightSisters!=0){
             	        			 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){
             	        				 firstConditionIn=false;
             	        				 CStateNodeList* childsNp2=rightSisters->node->m_umbinarizedSubNodes;
             	        				 while(childsNp2!=0){
             	        					 const CStateNode* nnChildNp2=childsNp2->node;
             	        				     if ((nnChildNp2->type==CStateNode::LEAF)
             	        				      	&& (nnChildNp2->lexical_head==rightSisters->node->lexical_head) //<#
             	        				     	&& (compareWordToTimeWordRegex((*words)[nnChildNp2->lexical_head].word))
             	        				     	&& ((((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN))
             	        				      	||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
             	        				      	||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
             	        				      	||(((*words)[nnChildNp2->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
             	        				    	 	 thirdCondition=true;
             	        				     }
             	        				     childsNp2=childsNp2->next;
             	        				 }
             	        			 }

             	        		 }
             	        		 if (firstConditionIn || secCondition || thirdCondition){
             	        		//if (true){
//             	        			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
             	        			 if (buildStanfordLink(STANFORD_DEP_DOBJ, targ->lexical_head, node.lexical_head)) {
             	        				 addLinked(&node,targ);
             	        			  	 return true;
             	        			 }
             	        		 }

             	        	 }
             	        	 childsVp=childsVp->next;
             	         }
             	     }
             	 }


              }



 //"SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
       inline const bool & buildDobj4(const unsigned long &cons) {
          if (cons==PENN_CON_SBARQ){
             CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
             
             bool secCond=false;
             //findChainMultiCategory(CConstituentList* via_category, CConstituent target_category, const CStateNode* head, CStateNodeList*& candidates);
             CConstituentList* ccList=new CConstituentList();
             CConstituentList::add(ccList,PENN_CON_SQ);
             CConstituentList::add(ccList,PENN_CON_SINV);
             CConstituentList::add(ccList,PENN_CON_S);
             CConstituentList::add(ccList,PENN_CON_VP);
             CStateNodeList* candidates=new CStateNodeList();
             findChainMultiCategory(ccList, PENN_CON_VP, &node, candidates);
             if (candidates->node==0){
             	candidates->clear();
             	candidates=0;
             }
             //(VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
             bool firstCCond=true; //!< NP|TO
             bool secCCond=true; //!< (S < (VP < TO))
             bool thirdCCond=true; //!< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD))
             bool fourthCCond=true; //!<- PRT
             bool fifthCCond=true; //!<- (PP <: IN)
             bool sixthCCond=false; //$-- (NP !< /^-NONE-$/)
             while(candidates!=0){
             	CStateNodeList* childsCand=candidates->node->m_umbinarizedSubNodes;
             	while(childsCand!=0){
             		if (CConstituent::clearTmp(childsCand->node->constituent.code())==PENN_CON_NP || (*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_TO){
             			firstCCond=false;
             		}
             		else if (CConstituent::clearTmp(childsCand->node->constituent.code())==PENN_CON_S){
             			CStateNodeList* childsS=childsCand->node->m_umbinarizedSubNodes;
             			while(childsS!=0){
             				if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_VP){
             					CStateNodeList* childsVp=childsS->node->m_umbinarizedSubNodes;
             					while(childsVp!=0){
             						if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
             							secCCond=false;
             						}
             						childsVp=childsVp->next;
             					}
             				}
             				childsS=childsS->next;
             			}
             		}
             		//PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
             		//!< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD))
             		else if ((*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB ||
             				(*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST||
             				(*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG||
             				(*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE||
             				(*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES||
             				(*words)[childsCand->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
             			
             			bool copCond=false;
             			bool rsisCond=false;
             			//CStateNodeList* childsVb=childsCand->node->m_umbinarizedSubNodes;
             			//while(childsVb!=0){
             				if (compareWordToCopularWordRegex((*words)[childsCand->node->lexical_head].word)) {
             					copCond=true;
             				}
             				//childsVb=childsVb->next;
             			//}
             			CStateNodeList* rightSisters=childsCand->next;
             			while(rightSisters!=0){
             				if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_VP){
             					CStateNodeList* rsChilds=rightSisters->node->m_umbinarizedSubNodes;
             					while(rsChilds!=0){
             						if ((*words)[rsChilds->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE ||
             						    (*words)[rsChilds->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST){
             							rsisCond=true;
             						}
             						rsChilds=rsChilds->next;
             					}
             				}
             				rightSisters=rightSisters->next;
             			}
             			
             			if (copCond && rsisCond){
             				thirdCCond=false;
             			}
             			
             		}
             		//!<- PRT
             		else if (CConstituent::clearTmp(childsCand->node->constituent.code())==PENN_CON_PRT && childsCand->next==0){
             			fourthCCond=false;
             		}
             		//!<- (PP <: IN)
             		else if (CConstituent::clearTmp(childsCand->node->constituent.code())==PENN_CON_PP && childsCand->next==0){
             			CStateNodeList* childsPP=childsCand->node->m_umbinarizedSubNodes;
             			if (childsPP!=0){
             				if ((*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_IN && childsPP->next==0){
             					fifthCCond=false;
             				}
             			}
             		}
             		childsCand=childsCand->next;
             	}
             	const CStateNode* parent=findParent(&node, candidates->node);
             	if (parent!=0){
             		CStateNodeList* childsP=parent->m_umbinarizedSubNodes;
             		while(childsP!=0){
             			if (childsP->node==candidates->node){
             				CStateNodeList* leftSisters=childsP->previous;
             				//$-- (NP !< /^-NONE-$/)
             				while(leftSisters!=0){
             					if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
             						bool noneCond=true;
             						CStateNodeList* childsLNp=leftSisters->node->m_umbinarizedSubNodes;
             						while(childsLNp!=0){
             							if (CConstituent::clearTmp(childsLNp->node->constituent.code())==PENN_CON_NONE){
             								noneCond=false;
             							}
             							childsLNp=childsLNp->next;
             						}
             						sixthCCond=noneCond;
             					}
             					leftSisters=leftSisters->previous;
             				}
             			}
             			childsP=childsP->next;
             		}
             	}
             	
             	candidates=candidates->next;
             }
             
             
             secCond=firstCCond && secCCond && thirdCCond && fourthCCond && fifthCCond && sixthCCond;
             if (secCond){
             	childsSbarq=node.m_umbinarizedSubNodes;
             	while(childsSbarq!=0){
             		const CStateNode* targ=childsSbarq->node;
             		if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_SBARQ && !isLinked(&node,targ)){
             			bool firstInCond=true;
             			bool secInCond=true;
             			
             			CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
             			while(childsT!=0){
             				if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_WRB){
             					firstInCond=false;
             				}
             				//PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
             				else if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN||
             						(*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER||
             						(*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL||
             						(*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL) 
             						&& childsT->node->lexical_head==targ->lexical_head){
             					
             					//CStateNodeList* childsNN=childsT->node->m_umbinarizedSubNodes;
             					//while(childsNN!=0){
             						if (compareWordToTimeWordRegex((*words)[childsT->node->lexical_head].word)) {
             							secInCond=false;
             						}
             						//childsNN=childsNN->next;
             					//}
             					
             				}
             				childsT=childsT->next;
             			}
             			
             			if (firstInCond && secInCond){
//             				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
             				if (buildStanfordLink(STANFORD_DEP_DOBJ, targ->lexical_head, node.lexical_head)) {
             					addLinked(&node,targ); //this is not correct! but almost
             				 	return true;
             				}
             			}
             		}
             		childsSbarq=childsSbarq->next;
             	}

             }
          }
          return false;
       }


 //"SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",
       inline const bool &buildDobj5(const unsigned long &cons) {
    	 if (cons==PENN_CON_SBAR){
    		bool secondCondition=false;
    		CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
    		while(childsSbar!=0){
    			const CStateNode* schildSbar=childsSbar->node;
    			if (CConstituent::clearTmp(schildSbar->constituent.code())==PENN_CON_S){
    				CStateNodeList* childsS=schildSbar->m_umbinarizedSubNodes;
    				while(childsS!=0){
    					const CStateNode* npChildS=childsS->node;
    					if (CConstituent::clearTmp(npChildS->constituent.code())==PENN_CON_NP){
    						CStateNodeList* childsNp=npChildS->m_umbinarizedSubNodes;
    						while(childsNp!=0){
    							const CStateNode* vpChildNp=childsNp->node;
    							if (CConstituent::clearTmp(vpChildNp->constituent.code())==PENN_CON_VP){
    								bool firstInCond=true;
    								bool secondInCond=true;
    								bool thirdInCond=true;
    								CStateNodeList* childsVp=vpChildNp->m_umbinarizedSubNodes;
    								while(childsVp!=0){
    									if (CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_SBAR){
    										firstInCond=false;
    									}
    									if (CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_S){
    										CStateNodeList* childsSvp=childsVp->node->m_umbinarizedSubNodes;
    										while(childsSvp!=0){
    											const CStateNode* vpChildS=childsSvp->node;
    											if (CConstituent::clearTmp(vpChildS->constituent.code())==PENN_CON_VP){
    												CStateNodeList* childsVpTo=vpChildS->m_umbinarizedSubNodes;
    												while(childsVpTo!=0){
    													if (((*words)[childsVpTo->node->lexical_head].tag.code()==PENN_TAG_TO)) {
    														thirdInCond=false;
    													}
    													childsVpTo=childsVpTo->next;
    												}
    											}
    											childsSvp=childsSvp->next;
    										}
    									}
    									childsVp=childsVp->next;
    								}
    								//and now, the secondInCond
    								CStateNodeList* ppsChain=new CStateNodeList();
    								 findChain(PENN_CON_VP,PENN_CON_PP, vpChildNp, ppsChain);
    								 if (ppsChain->node==0) {
    									 ppsChain->clear();
    								     ppsChain=0;
    								 }
    								 //std::cout<<"4\n";
    								 while(ppsChain!=0){
    									 CStateNodeList* childsOfAPP=ppsChain->node->m_umbinarizedSubNodes;
    									 while(childsOfAPP){
    										 //A <- B 	B is the last child of A
    										 if (childsOfAPP->next==0 && ((*words)[childsOfAPP->node->lexical_head].tag.code()==PENN_TAG_IN)){
    											 secondInCond=false;
    										 }
    										 childsOfAPP=childsOfAPP->next;
    									 }
    									 ppsChain=ppsChain->next;
    								 }

    								if (firstInCond && secondInCond && thirdInCond){
    									secondCondition=true;
    								}
    							}
    							childsNp=childsNp->next;
    						}
    					}
    					childsS=childsS->next;
    				}
    			}
    			childsSbar=childsSbar->next;
    		}

    		if (secondCondition){
    			childsSbar=node.m_umbinarizedSubNodes;
    			while(childsSbar!=0){
    				const CStateNode* whnpTarg=childsSbar->node;
    				if (CConstituent::clearTmp(whnpTarg->constituent.code())==PENN_CON_WHNP){
    					bool wrbCond=true;
    					CStateNodeList* childsWhnp=whnpTarg->m_umbinarizedSubNodes;
    					while(childsWhnp!=0){
    						if (((*words)[childsWhnp->node->lexical_head].tag.code()==PENN_TAG_WRB)) {
    							wrbCond=false;
    						}
    						childsWhnp=childsWhnp->next;
    					}
    					if (wrbCond){
//    						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
    						if (buildStanfordLink(STANFORD_DEP_DOBJ, whnpTarg->lexical_head, node.lexical_head)) {
    							addLinked(&node,whnpTarg); //this is not correct! but almost
    						 	return true;
    						}
    					}
    				}
    				childsSbar=childsSbar->next;
    			}
    		}
    	 }
    	 return false;
     }

 //"SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",
  inline const bool &buildDobj6(const unsigned long &cons) {
 	//we do not care about what the head of S is, S is not the the head, but is the "lexical" head of VP, which is the head of SBAR which is the the "stanford" link head of WHNP
 	CStateNodeList* childs=node.m_umbinarizedSubNodes;
 	while(childs!=0){
 		const CStateNode* sChild=childs->node;
 		if (CConstituent::clearTmp(sChild->constituent.code())==PENN_CON_S){
 			if (childs->previous!=0){
 				const CStateNode* whnpTarg=childs->previous->node;
 				if (CConstituent::clearTmp(whnpTarg->constituent.code())==PENN_CON_WHNP){ //I'll check whether it is linked when I have the head (SBAR)
 					CStateNodeList* childsS=sChild->m_umbinarizedSubNodes;
 					bool npFound=false;
 					while(childsS!=0){
 						if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP){
 							npFound=true;
 						}
 						childsS=childsS->next;
 					}
 					if (npFound){
 						childsS=sChild->m_umbinarizedSubNodes;
 						while(childsS!=0){
 							const CStateNode* vpChilds=childsS->node;
 							if (CConstituent::clearTmp(vpChilds->constituent.code())==PENN_CON_VP){
 								CStateNodeList* childsVp=vpChilds->m_umbinarizedSubNodes;
 								while(childsVp!=0){
 									const CStateNode* sbarHead=childsVp->node;
 									if ((CConstituent::clearTmp(sbarHead->constituent.code())==PENN_CON_SBAR) && !(isLinked(sbarHead, whnpTarg))){
 										CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
 										bool firstCondition=true;
 										while(childsSbar!=0){
 											if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHNP ||CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHADVP){
 												firstCondition=false;
 											}
 											childsSbar=childsSbar->next;
 										}
 										if (firstCondition){
 											childsSbar=sbarHead->m_umbinarizedSubNodes;
 											while(childsSbar!=0){
 												const CStateNode* sChildSbar=childsSbar->node;
 												if (CConstituent::clearTmp(sChildSbar->constituent.code())==PENN_CON_S){
 													CStateNodeList* childsS=sChildSbar->m_umbinarizedSubNodes;
 													while(childsS!=0){
 														const CStateNode* npChildS=childsS->node;
 														if (CConstituent::clearTmp(npChildS->constituent.code())==PENN_CON_NP){ //again, @NP, basic "class" of NP. how come?
 															//A $++ B 	A is a left sister of B
 															CStateNodeList* rightSistersNp=childsS;
 															while(rightSistersNp!=0){
 																if (CConstituent::clearTmp(rightSistersNp->node->constituent.code())==PENN_CON_VP){
 																	CStateNodeList* rightSistersVp=rightSistersNp;
 																	bool noRightSister=true;
 																	while(rightSistersVp!=0){
 																		if (CConstituent::clearTmp(rightSistersVp->node->constituent.code())==PENN_CON_NP){
 																			noRightSister=false;
 																		}
 																		rightSistersVp=rightSistersVp->next;
 																	}
 																	if (noRightSister){
// 																		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
 																		if (buildStanfordLink(STANFORD_DEP_DOBJ, whnpTarg->lexical_head, sbarHead->lexical_head)) {
 																			addLinked(&node,whnpTarg); //this is not correct! but almost
 																			return true;
 																		}

 																	}
 																}
 																rightSistersNp=rightSistersNp->next;
 															}
 														}
 														childsS=childsS->next;
 													}
 												}
 												childsSbar=childsSbar->next;
 											}
 										}
 									}
 									childsVp=childsVp->next;
 								}
 							}
 							childsS=childsS->next;
 						}
 					}

 				}
 			}
 		}
			childs=childs->next;
		}
 	return false;
	}

  //"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
         inline const bool &buildDobj7(const unsigned long &cons) {
      	   if (cons==PENN_CON_NP){ //the first node is the target.
      		   CStateNodeList* childsTarget=node.m_umbinarizedSubNodes;
      		   while(childsTarget!=0){
      			   const CStateNode* head=childsTarget->node;
      			   if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_SBAR && !isLinked(&node,head)){ //note that here the order of the parameters should be the opposite, because the target is the first node you find.
      				   bool sisterCond1=true; //!$-- CC
      				   bool sisterCond2=false;//$-- NP
      				   CStateNodeList* leftSisters=childsTarget->previous;
      				   while(leftSisters!=0){
      					   if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
      						   sisterCond2=true;
      					   }
      					   if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
      						   sisterCond1=false;
      					   }
      					   leftSisters=leftSisters->previous;
      				   }
      				   if (sisterCond1 && sisterCond2){
      					   bool firstCond=true; //(WHPP|WHNP|WHADVP)
      					   CStateNodeList* childsSbar=head->m_umbinarizedSubNodes;
      					   while(childsSbar!=0){
      						   if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHPP
      								   || CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHNP
      								   || CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHADVP){
      							   firstCond=false;
      						   }
      						   childsSbar=childsSbar->next;
      					   }

      					   if (firstCond){
      						   childsSbar=head->m_umbinarizedSubNodes;
      						   while(childsSbar!=0){
      							   if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_S){
      								   CStateNodeList* childsS=childsSbar->node->m_umbinarizedSubNodes;
      								   while(childsS!=0){
      									   if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP) {
      										   if (childsS->next!=0){
      											   if (CConstituent::clearTmp(childsS->next->node->constituent.code())==PENN_CON_VP){
      												   bool firstCondition=true;
      												   bool fourthCondition=true; //!< SBAR
      												   CStateNodeList* childsVp=childsS->next->node->m_umbinarizedSubNodes;
      												   while(childsVp!=0){
      													   if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB
      															   && (compareWordToCopularWordRegex((*words)[childsVp->node->lexical_head].word))){

      														   if (childsVp->next!=0){
      															   if (!CConstituent::clearTmp(childsVp->next->node->constituent.code())==PENN_CON_VP){
      																   firstCondition=false;
      															   }
      														   }
      														   else {
      															   firstCondition=false;
      														   }
      													   }
      													   if (!CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_SBAR){
      														   fourthCondition=false;
      													   }
      													   childsVp=childsVp->next;
      												   }
      												   if (firstCondition && fourthCondition) {
      													   bool secondCondition=true; //!<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD))
      													   bool thirdCondition=true; //!<+(VP) NP
      													   bool fifthCondition=true; //!<+(VP) (PP <- IN)

      													   CStateNodeList* candidates=new CStateNodeList();
      													   findChain(PENN_CON_VP, PENN_CON_NP, childsS->next->node, candidates);
      													   if (candidates->node==0) {
      														   candidates->clear();
      													       candidates=0;
      													   }
      													   else {
      														   thirdCondition=false;
      													   }
      													   if (thirdCondition) {
      														   CStateNodeList* candidates2=new CStateNodeList();
      														   findChain(PENN_CON_VP, PENN_CON_PP, childsS->next->node, candidates2);
      														   if (candidates2->node==0) {
      															   candidates2->clear();
      														       candidates2=0;
      														   }
      														   while(candidates2!=0){
      															   CStateNodeList* childsC2=candidates2->node->m_umbinarizedSubNodes;
      															   while(childsC2!=0){
      																   if ((*words)[childsC2->node->lexical_head].tag.code()==PENN_TAG_IN) {
      																	   fifthCondition=false;
      																   }
      																   childsC2=childsC2->next;
      															   }
      															   candidates2=candidates2->next;
      														   }
      														   if (fifthCondition){
      															   CStateNodeList* candidates3=new CStateNodeList();
      															   findChainTargetPos(PENN_CON_VP, PENN_TAG_VERB, childsS->next->node, candidates3);
      															   if (candidates3->node==0) {
      																   candidates3->clear();
      															       candidates3=0;
      															   }
      															   while (candidates3!=0){
      																   //!<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD))
      																   if (compareWordToCopularWordRegex((*words)[candidates3->node->lexical_head].word)){
      																	   const CStateNode* parent=findParent(childsS->next->node, candidates3->node);
      																	   if (parent!=0){
      																		   CStateNodeList* childs=parent->m_umbinarizedSubNodes;
      																		   while(childs!=0){
      																			   if (childs->node==candidates3->node){
      																				   if (childs->next!=0){
      																					   if (CConstituent::clearTmp(childs->next->node->constituent.code())==PENN_CON_VP){
      																						   CStateNodeList* childsVpCand=childs->next->node->m_umbinarizedSubNodes;
      																						   while(childsVpCand!=0){
      																							   if ((*words)[childsVpCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST
      																									   || (*words)[childsVpCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
      																								   secondCondition=false;
      																							   }
      																							   childsVpCand=childsVpCand->next;
      																						   }
      																					   }
      																				   }
      																			   }
      																			   childs=childs->next;
      																		   }
      																	   }
      																   }
      																   candidates3=candidates3->next;
      															   }
      														   }
      													   }

      													   if (secondCondition && thirdCondition && fourthCondition && fifthCondition){
      														   if (buildStanfordLink(STANFORD_DEP_DOBJ, node.lexical_head, head->lexical_head)) { //note that here the order of the parameters is the opposite, because the head is the last one.
      															   addLinked(&node,head);
      														       return true;
      														   }
      													   }
      												   }

      											   }
      										   }
      									   }
      									   childsS=childsS->next;
      								   }
      							   }
      							   childsSbar=childsSbar->next;
      						   }
      					   }
      				   }
      			   }
      			   childsTarget=childsTarget->next;
      		   }

      	   }
      	   return false;
         }

//"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"
         inline const bool &buildDobj8(const unsigned long &cons) {
      	   if (cons==PENN_CON_NP){ //the first node is the target.
      		   CStateNodeList* childsTarget=node.m_umbinarizedSubNodes;
      		   while(childsTarget!=0){
      			   const CStateNode* head=childsTarget->node;
      			   if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_SBAR && !isLinked(&node,head)){ //note that here the order of the parameters should be the opposite, because the target is the first node you find.
      				   bool sisterCond1=true; //!$-- CC
      				   bool sisterCond2=false;//$-- NP
      				   CStateNodeList* leftSisters=childsTarget->previous;
      				   while(leftSisters!=0){
      					   if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
      						   sisterCond2=true;
      					   }
      					   if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
      						   sisterCond1=false;
      					   }
      					   leftSisters=leftSisters->previous;
      				   }
      				   if (sisterCond1 && sisterCond2){
      					   bool firstCond=true; //(WHPP|WHNP|WHADVP)
      					   CStateNodeList* childsSbar=head->m_umbinarizedSubNodes;
      					   while(childsSbar!=0){
      						   if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHPP
      								   || CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHNP
      								   || CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_WHADVP){
      							   firstCond=false;
      						   }
      						   childsSbar=childsSbar->next;
      					   }

      					   if (firstCond){
      						   childsSbar=head->m_umbinarizedSubNodes;
      						   while(childsSbar!=0){
      							   if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_S){
      								   CStateNodeList* childsS=childsSbar->node->m_umbinarizedSubNodes;
      								   while(childsS!=0){
      									   if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP) {
      										   if (childsS->next!=0){
      											   if (CConstituent::clearTmp(childsS->next->node->constituent.code())==PENN_CON_ADVP){
      												   bool firstCondition=true;
      												   bool fourthCondition=true; //!< SBAR
      												   CStateNodeList* childsVp=childsS->next->node->m_umbinarizedSubNodes;
      												   while(childsVp!=0){
      													   if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB
      															   && (compareWordToCopularWordRegex((*words)[childsVp->node->lexical_head].word))){

      														   if (childsVp->next!=0){
      															   if (!CConstituent::clearTmp(childsVp->next->node->constituent.code())==PENN_CON_VP){
      																   firstCondition=false;
      															   }
      														   }
      														   else {
      															   firstCondition=false;
      														   }
      													   }
      													   if (!CConstituent::clearTmp(childsVp->node->constituent.code())==PENN_CON_SBAR){
      														   fourthCondition=false;
      													   }
      													   childsVp=childsVp->next;
      												   }
      												   if (firstCondition && fourthCondition) {
      													   bool secondCondition=true; //!<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD))
      													   bool thirdCondition=true; //!<+(VP) NP
      													   bool fifthCondition=true; //!<+(VP) (PP <- IN)

      													   CStateNodeList* candidates=new CStateNodeList();
      													   findChain(PENN_CON_VP, PENN_CON_NP, childsS->next->node, candidates);
      													   if (candidates->node==0) {
      														   candidates->clear();
      													       candidates=0;
      													   }
      													   else {
      														   thirdCondition=false;
      													   }
      													   if (thirdCondition) {
      														   CStateNodeList* candidates2=new CStateNodeList();
      														   findChain(PENN_CON_VP, PENN_CON_PP, childsS->next->node, candidates2);
      														   if (candidates2->node==0) {
      															   candidates2->clear();
      														       candidates2=0;
      														   }
      														   while(candidates2!=0){
      															   CStateNodeList* childsC2=candidates2->node->m_umbinarizedSubNodes;
      															   while(childsC2!=0){
      																   if ((*words)[childsC2->node->lexical_head].tag.code()==PENN_TAG_IN) {
      																	   fifthCondition=false;
      																   }
      																   childsC2=childsC2->next;
      															   }
      															   candidates2=candidates2->next;
      														   }
      														   if (fifthCondition){
      															   CStateNodeList* candidates3=new CStateNodeList();
      															   findChainTargetPos(PENN_CON_VP, PENN_TAG_VERB, childsS->next->node, candidates3);
      															   if (candidates3->node==0) {
      																   candidates3->clear();
      															       candidates3=0;
      															   }
      															   while (candidates3!=0){
      																   //!<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD))
      																   if (compareWordToCopularWordRegex((*words)[candidates3->node->lexical_head].word)){
      																	   const CStateNode* parent=findParent(childsS->next->node, candidates3->node);
      																	   if (parent!=0){
      																		   CStateNodeList* childs=parent->m_umbinarizedSubNodes;
      																		   while(childs!=0){
      																			   if (childs->node==candidates3->node){
      																				   if (childs->next!=0){
      																					   if (CConstituent::clearTmp(childs->next->node->constituent.code())==PENN_CON_VP){
      																						   CStateNodeList* childsVpCand=childs->next->node->m_umbinarizedSubNodes;
      																						   while(childsVpCand!=0){
      																							   if ((*words)[childsVpCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST
      																									   || (*words)[childsVpCand->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
      																								   secondCondition=false;
      																							   }
      																							   childsVpCand=childsVpCand->next;
      																						   }
      																					   }
      																				   }
      																			   }
      																			   childs=childs->next;
      																		   }
      																	   }
      																   }
      																   candidates3=candidates3->next;
      															   }
      														   }
      													   }

      													   if (secondCondition && thirdCondition && fourthCondition && fifthCondition){
      														   if (buildStanfordLink(STANFORD_DEP_DOBJ, node.lexical_head, head->lexical_head)) { //note that here the order of the parameters is the opposite, because the head is the last one.
      															   addLinked(&node,head);
      														       return true;
      														   }
      													   }
      												   }

      											   }
      										   }
      									   }
      									   childsS=childsS->next;
      								   }
      							   }
      							   childsSbar=childsSbar->next;
      						   }
      					   }
      				   }
      			   }
      			   childsTarget=childsTarget->next;
      		   }

      	   }
      	   return false;
         }
