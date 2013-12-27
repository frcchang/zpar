
//"VP|ADJP < NP-TMP=target",
inline const bool &tmod1(const unsigned long &cons){
//we can safely ignore it...NP-TMP is not in our data set.
}

//"VP|ADJP < (NP=target <# (/^NN/ < " + timeWordRegex + ") !$+ (/^JJ/ < old))",
inline const bool &tmod2(const unsigned long &cons){
  	  if (cons==PENN_CON_VP || cons==PENN_CON_ADJP){
  		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
  		  while(childs!=0){
  			  const CStateNode* targ=childs->node;
  			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP && !isLinked(&node,targ)){
  				  bool cond1=false;
  				  bool cond2=true;
  				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
  				  while(childsT!=0){
  					  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
  					  if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
  							  || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)
  							  && targ->lexical_head==childsT->node->lexical_head){
  						  //CStateNodeList* childsNN=childsT->node->m_umbinarizedSubNodes;
  						  //while(childsNN!=0){
  							  if (compareWordToTimeWordRegex((*words)[childsT->node->lexical_head].word)) {
  								  cond1=true;
  							  }
  							//  childsNN=childsNN->next;
  						  //}
  					  }
  					  if (childsT->next!=0){
  						  //PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
  						  if ((*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE || (*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE
  						      || (*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE){

  							  //CStateNodeList* childsJJ=childsT->next->node->m_umbinarizedSubNodes;
  							  //while(childsJJ!=0){
  								  if ((*words)[childsT->next->node->lexical_head].word==g_word_old){
  									  cond2=false;
  								  }
  								//  childsJJ=childsJJ->next;
  							  //}


  						  }
  					  }
  					  childsT=childsT->next;
  				  }

  				  if (cond1 && cond2){
//  					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
  					  if (buildStanfordLink(STANFORD_DEP_TMOD, targ->lexical_head, node.lexical_head)) {
  						  addLinked(&node,targ);
  					      return true;
  					  }
  				  }
  			  }
  			  childs=childs->next;
  		  }
  	  }
  	  return false;
    }



//"@PP < (IN|TO|VBG|FW $++ (@NP $+ NP-TMP=target))",
inline const bool &tmod3(const unsigned long &cons){
//we can safely ignore it, NP-TMP is not in our data set.
}

//"@PP < (IN|TO|VBG|FW $++ (@NP $+ (NP=target <# (/^NN/ < " + timeWordRegex + "))))",
inline const bool & tmod4(const unsigned long &cons){
    	  if (cons==PENN_CON_PP){// ||cons==PENN_CON_WHPP){ //??
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
    				  (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_TO || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_FW){
    				  CStateNodeList* rightSisters=childs->next;
    				  while(rightSisters!=0){
    					  if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){ //|| rightSisters->node->constituent.code())==PENN_CON_WHNP){ //???
    						  if (rightSisters->next!=0){
    							  const CStateNode* targ=rightSisters->next->node;
    							  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP && !isLinked(&node,targ)){
    								  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    								  while(childsT!=0){
    									  if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
    										  || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)
    									      && targ->lexical_head==childsT->node->lexical_head){
    										  //CStateNodeList* childsNN=childsT->node->m_umbinarizedSubNodes;
    										 // while(childsNN!=0){
    											  if (compareWordToTimeWordRegex((*words)[childsT->node->lexical_head].word)) {
//    												  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
    												  if (buildStanfordLink(STANFORD_DEP_TMOD, targ->lexical_head, node.lexical_head)) {
    													  addLinked(&node,targ);
    												      return true;
    												  }
    											  }
    											//  childsNN=childsNN->next;
    										  //}
    									  }
    									  childsT=childsT->next;
    								  }
    							  }
    						  }
    					  }
    					  rightSisters=rightSisters->next;
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }


//"S < (NP-TMP=target $++ VP [ $++ NP | $-- NP] )",
inline const bool &tmod5(const unsigned long &cons){
//we can safely ignore it....
}


//"S < (NP=target <# (/^NN/ < " + timeWordRegex + ") $++ (NP $++ VP))",
inline const bool &tmod6(const unsigned long &cons){
    	  if (cons==PENN_CON_S){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP && !isLinked(&node,targ)){
    				  CStateNodeList* childsNp=targ->m_umbinarizedSubNodes;
    				  while(childsNp!=0){
    					  if (((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
    					     || (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)
    				      	 && targ->lexical_head==childsNp->node->lexical_head){
    				      	 //CStateNodeList* childsNN=childsNp->node->m_umbinarizedSubNodes;
    				      	 //while(childsNN!=0){
    				      		 if (compareWordToTimeWordRegex((*words)[childsNp->node->lexical_head].word)) {

    				      			 CStateNodeList* rightSisters=childs->next;
    				      			 while(rightSisters!=0){
    				      				 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){
    				      					 CStateNodeList* rightSisters2=rightSisters->next;
    				      					 while(rightSisters2!=0){
    				      						 if (CConstituent::clearTmp(rightSisters2->node->constituent.code())==PENN_CON_VP){
//    				      							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
    				      							if (buildStanfordLink(STANFORD_DEP_TMOD, targ->lexical_head, node.lexical_head)) {
    				      								addLinked(&node,targ);
    				      							    return true;
    				      							 }
    				      						 }
    				      						 rightSisters2=rightSisters2->next;
    				      					 }
    				      				 }
    				      				 rightSisters=rightSisters->next;
    				      			 }

    				      		 }
    				      		 //childsNN=childsNN->next;
    				      	 //}
    				      }
    				      childsNp=childsNp->next;
    				   }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }


      //THIS ONE IS VERY VERY WEIRD (MIGUEL)
      //"SBAR < (@WHADVP < (WRB < when)) < (S < (NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) ))) !$-- CC $-- NP > NP=target",
inline const bool & tmod7(const unsigned long &cons){
    	  if (cons==PENN_CON_NP){ //this the target!
    		  CStateNodeList* childsT=node.m_umbinarizedSubNodes;
    		  while(childsT!=0){
    			  const CStateNode* head=childsT->node;
    			  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_SBAR && !isLinked(&node,head)){ //the other way around, this is a relaxation of what we really want
    				  CStateNodeList* leftSisters=childsT->previous;
    				  bool leftSistCond1=true;
    				  bool leftSistCond2=false;
    				  while(leftSisters!=0){
    					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
    						  leftSistCond2=true;
    					  }
    					  if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						  leftSistCond1=false;
    					  }
    					  leftSisters=leftSisters->previous;
    				  }

    				  CStateNodeList* childsH=head->m_umbinarizedSubNodes;
    				  bool childsCond1=false;
    				  bool childsCond2=false;
    				  while(childsH!=0){
    					  if (CConstituent::clearTmp(childsH->node->constituent.code())==PENN_CON_WHNP){
    						  CStateNodeList* childsWh=childsH->node->m_umbinarizedSubNodes;
    						  while(childsWh!=0){
    							  if ((*words)[childsWh->node->lexical_head].tag.code()==PENN_TAG_WRB){
    								  //CStateNodeList* childsWr=childsWh->node->m_umbinarizedSubNodes;
    								  //while(childsWr!=0){
    									  if ((*words)[childsWh->node->lexical_head].word==g_word_when){
    										  childsCond1=true;
    									  }
    									//  childsWr=childsWr->next;
    								  //}
    							  }
    							  childsWh=childsWh->next;
    						  }
    					  }
    					  //(S < (NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) )))
    					  if (CConstituent::clearTmp(childsH->node->constituent.code())==PENN_CON_S){
    						  CStateNodeList* childsS=childsH->node->m_umbinarizedSubNodes;
    						  while(childsS!=0){
    							  if (CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_NP){
    								  if (childsS->next!=0){
    									  if (CConstituent::clearTmp(childsS->next->node->constituent.code())==PENN_CON_VP){
    										  CStateNodeList* childsVpnps=childsS->next->node->m_umbinarizedSubNodes;
    										  bool inCond=true; //we expect this to be true, but if the conditions inside the parenthesis match, them it will be false
    										  while(childsVpnps!=0){
    											  if ((*words)[childsVpnps->node->lexical_head].tag.code()==PENN_TAG_VERB){ //only this?
    												  bool copCond=false; //< " + copularWordRegex +
    												  bool sistCond=true; //!$+ VP
    												  if (childsVpnps->next!=0){
    													  if (CConstituent::clearTmp(childsVpnps->next->node->constituent.code())==PENN_CON_VP){
    														  sistCond=false;
    													  }
    												  }
    												  //CStateNodeList* childsVb=childsVpnps->node->m_umbinarizedSubNodes;
    												  //while(childsVb!=0){
    													  if (compareWordToCopularWordRegex((*words)[childsVpnps->node->lexical_head].word)) {
    														  copCond=true;
    													  }
    													//  childsVb=childsVb->next;
    												  //}

    												  if (copCond && sistCond){
    													  inCond=false;
    												  }
    											  }
    											  childsVpnps=childsVpnps->next;
    										  }
    										  childsCond2=inCond;

    									  }
    								  }
    							  }
    							  childsS=childsS->next;
    						  }
    					  }
    					  childsH=childsH->next;
    				  }
    				  if (childsCond1 && childsCond2){
//    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
    					  if (buildStanfordLink(STANFORD_DEP_TMOD, node.lexical_head, head->lexical_head)) { //the other way around for this rule-the ARC direction is correct! we normally have buildStanfordLink(label, head->lexical_head, node.lexical_head))
    						  addLinked(&node,head); //this is a relaxation of what we need, but I think that perhaps it could work.
    					      return true;
    					  }
    				  }

    			  }
    			  childsT=childsT->next;
    		  }
    	  }
    	  return false;
      }


//"SBARQ < (@WHNP=target <# (/^NN/ < " + timeWordRegex + ")) < (SQ < @NP)",
inline const bool & tmod8(const unsigned long &cons){
    	  if (cons==PENN_CON_SBARQ){
    		  bool secCond=false;
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_SQ){
    				  CStateNodeList* childsSq=childs->node->m_umbinarizedSubNodes;
    				  while(childsSq!=0){
    					  if (CConstituent::clearTmp(childsSq->node->constituent.code())==PENN_CON_NP){
    						  secCond=true;
    					  }
    					  childsSq=childsSq->next;
    				  }
    			  }
    			  childs=childs->next;
    		  }
    		  if (secCond){
    			CStateNodeList* childs2=node.m_umbinarizedSubNodes;
    			while(childs2!=0){
    				const CStateNode* targ=childs2->node;
    				if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHNP && !isLinked(&node,targ)){
    					CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    						while(childsT!=0){
    							if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
    									|| (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)
    							      	&& targ->lexical_head==childsT->node->lexical_head){
    							    //CStateNodeList* childsNN=childsT->node->m_umbinarizedSubNodes;
    							    //while(childsNN!=0){
    							    	if (compareWordToTimeWordRegex((*words)[childsT->node->lexical_head].word)) {
//    							    		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
    							      		if (buildStanfordLink(STANFORD_DEP_TMOD, targ->lexical_head, node.lexical_head)) {
    							      			addLinked(&node,targ);
    							      			return true;
    							      		}
    							      	}
    							    	//childsNN=childsNN->next;
    							      //}
    							  }
    							  childsT=childsT->next;
    							}
    						  }
    						  childs2=childs2->next;
    					  }
    				  }
    	  }
    	  return false;
      }


//"NP < NP-TMP=target"
inline const bool &tmod9(const unsigned long &cons){
//we can safely ignore it...
}
