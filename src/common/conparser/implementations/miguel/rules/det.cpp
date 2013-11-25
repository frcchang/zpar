
     //"/^(?:NP(?:-TMP|-ADV)?|NAC|NML|NX|X)$/ < (DT=target !< /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:N[MNXP]|CD|JJ|FW|ADJP|QP|RB|PRP(?![$])|PRN)/=det !$++ (/^PRP[$]|POS/ $++ =det !$++ (/''/ $++ =det)))",
     bool det1(){
    	 if (node.constituent==PENN_CON_NP || node.constituent==PENN_CON_NX || node.constituent==PENN_CON_X || node.constituent==PENN_CON_NAC){
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    	 	 while(childsNp!=0){
    	 		 const CStateNode* targ=childsNp->node;
    	 		 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT && !(isLinked(&node,targ))){
    	 			 bool firstCond=true;
    	 			 bool secCond=true;
    	 			 bool thirdCond=true;
    	 			 bool fourthCond=false;
    	 			 bool fifthCond=true;
    	 			 
    	 			 CStateNodeList* childsDt=targ->m_umbinarizedSubNodes;
    	 			 while(childsDt!=0){
    	 				 if (((*words)[childsDt->node->lexical_head].word==g_word_either) 
    	 					 ||((*words)[childsDt->node->lexical_head].word==g_word_neither)
    	 					 ||((*words)[childsDt->node->lexical_head].word==g_word_both)){
    	 					 firstCond=false;
    	 				 }
    	 				 childsDt=childsDt->next;
    	 			 }
    	 			 
    	 			 if (firstCond){
    	 				 CStateNodeList* rightSisters=childsNp->next;
    	 				 if (rightSisters!=0){
    	 					 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_DT){
    	 						 secCond=false;
    	 					 }
    	 				 }
    	 				 if (secCond){
    	 					//!$++ CC $++ /^(?:N[MNXP]|CD|JJ|FW|ADJP|QP|RB|PRP(?![$])|PRN)/=det !$++ (/^PRP[$]|POS/ $++ =det !$++ (/''/ $++ =det))
    	 					while(rightSisters!=0){
    	 						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    	 							thirdCond=false;
    	 						}
    	 						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CD||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_FW||
    	 								rightSisters->node->constituent==PENN_CON_ADJP||
    	 								rightSisters->node->constituent==PENN_CON_QP||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADVERB||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP||
    	 								//(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR|| (the ! is the negation symbol, right?)
    	 								rightSisters->node->constituent==PENN_CON_PRN){
    	 							fourthCond=true;
    	 						}
    	 						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR||
    	 								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_POS){
    	 							CStateNodeList* rightSistR=rightSisters->next;
    	 							bool rFCond=false;
    	 							bool rSCond=true;
    	 							while(rightSistR!=0){
    	 								if ((*words)[rightSistR->node->lexical_head].tag.code()==PENN_TAG_CD||
    	 								    (*words)[rightSistR->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE||
    	 								    (*words)[rightSistR->node->lexical_head].tag.code()==PENN_TAG_FW||
    	 								    rightSistR->node->constituent==PENN_CON_ADJP||
    	 								    rightSistR->node->constituent==PENN_CON_QP||
    	 								    (*words)[rightSistR->node->lexical_head].tag.code()==PENN_TAG_ADVERB||
    	 								    (*words)[rightSistR->node->lexical_head].tag.code()==PENN_TAG_PRP||
    	 								    //(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR|| (the ! is the negation symbol, right?)
    	 								    rightSistR->node->constituent==PENN_CON_PRN){
    	 									rFCond=true;
    	 								}
    	 								//!$++ (/''/ $++ =det)
    	 								if ((*words)[rightSistR->node->lexical_head].word==g_word_squotes){
    	 									CStateNodeList* rightrightSis=rightSistR->next;
    	 									while(rightrightSis!=0){
    	 										if ((*words)[rightrightSis->node->lexical_head].tag.code()==PENN_TAG_CD||
    	 										    (*words)[rightrightSis->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE||
    	 										    (*words)[rightrightSis->node->lexical_head].tag.code()==PENN_TAG_FW||
    	 										    rightrightSis->node->constituent==PENN_CON_ADJP||
    	 										    rightrightSis->node->constituent==PENN_CON_QP||
    	 										    (*words)[rightrightSis->node->lexical_head].tag.code()==PENN_TAG_ADVERB||
    	 										    (*words)[rightrightSis->node->lexical_head].tag.code()==PENN_TAG_PRP||
    	 										    //(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR|| (the ! is the negation symbol, right?)
    	 										    rightrightSis->node->constituent==PENN_CON_PRN){
    	 										    
    	 											rSCond=true;
    	 										}
    	 										rightrightSis=rightrightSis->next;
    	 									}
    	 								}
    	 								rightSistR=rightSistR->next;
    	 							}
    	 							if (rFCond && rSCond){
    	 								fifthCond=false;
    	 							}
    	 						}
    	 					}
    	 				 }
    	 			 }
    	 			 
    	 			 if (firstCond && secCond && thirdCond && fourthCond && fifthCond){
    	 				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
    	 				if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
    	 					addLinked(&node,targ);
    	 					return true;
    	 				}
    	 			 }
    	 			 
    	 		 }
    	 	}
    	 }
    	 return false;
     }



//"NP|NP-TMP|NP-ADV < (DT=target < /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:NN|NX|NML)/ !$++ (NP < CC))",
bool det2(){
	 if (node.constituent==PENN_CON_NP){
		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
		 while(childsNp!=0){
			 const CStateNode* targ=childsNp->node;
			 if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT && !(isLinked(&node,targ))){
				 bool firstCond=false;
				 bool secCond=true;
				 bool thirdCond=true;
				 bool fourthCond=false;
				 bool fifthCond=true;
				 
				 CStateNodeList* rightSisters=childsNp->next;
				 
				 if (rightSisters!=0){
					 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_DT){
						 secCond=false;
					 }
				 }
				 if (secCond){
					 while(rightSisters!=0){
						 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
							 thirdCond=false;
						 }
						 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN || rightSisters->node->constituent==PENN_CON_NX){
							 fourthCond=true;
						 }
						 if (rightSisters->node->constituent==PENN_CON_NP){
							 CStateNodeList* childsNp=rightSisters->node->m_umbinarizedSubNodes;
							 while(childsNp!=0){
								 if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_CC){
									 fifthCond=false;
								 }
								 childsNp=childsNp->next;
							 }
						 }
						 rightSisters=rightSisters->next;
					 }
					 if (thirdCond && fourthCond && fifthCond){
						 CStateNodeList* childsDt=targ->m_umbinarizedSubNodes;
						 while(childsDt!=0){
							 if (((*words)[childsDt->node->lexical_head].word==g_word_either) 
									 ||((*words)[childsDt->node->lexical_head].word==g_word_neither)
									 ||((*words)[childsDt->node->lexical_head].word==g_word_both)){
								 firstCond=true;
							 }
							 childsDt=childsDt->next;
						 }
					 }
				 }
				 
				 
				 if (firstCond && secCond && thirdCond && fourthCond && fifthCond){
					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
					 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
						 addLinked(&node,targ);
					 	 return true;
					 }
				 }
			 }
			 childsNp=childsNp->next;
		 }
	 }
	 return false;
}



//"NP|NP-TMP|NP-ADV < (DT=target !< /^(?i:either|neither|both)$/ $++ CC $++ /^(?:NN|NX|NML)/)",
bool det3(){
	 if (node.constituent==PENN_CON_NP){
		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
		 while(childsNp!=0){
			 const CStateNode* dtTarg=childsNp->node;
			 if ((*words)[dtTarg->lexical_head].tag.code()==PENN_TAG_DT && !(isLinked(&node,dtTarg))){
				 bool wordsCond=true;
				 CStateNodeList* childsDt=dtTarg->m_umbinarizedSubNodes;
				 while(childsDt!=0){
					 if (((*words)[childsDt->node->lexical_head].word==g_word_either) 
							 ||((*words)[childsDt->node->lexical_head].word==g_word_neither)
							 ||((*words)[childsDt->node->lexical_head].word==g_word_both)){
						 wordsCond=false;
					 }
					 childsDt=childsDt->next;
				 }
				 CStateNodeList* rightSisters=childsDt->next;
				 bool ccCond=false;
				 while(rightSisters!=0){
					 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
						 ccCond=true;
					 }
					 rightSisters=rightSisters->next;
				 }
				 if (ccCond){
					 rightSisters=childsDt->next;
					 while(rightSisters!=0){
						 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN || rightSisters->node->constituent==PENN_CON_NX){
							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
							 if (buildStanfordLink(label, dtTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,dtTarg);
							 	 return true;
							 }
						 }
						 rightSisters=rightSisters->next;
					 }
				 }
				 
			 }
			 childsNp=childsNp->next;
		 }
		 
	 }
	 return false;
}

//"NP|NP-TMP|NP-ADV < (DT=target $++ (/^JJ/ !$+ /^NN/) !$++ CC !$+ DT)",
bool det4(){
	 if (node.constituent==PENN_CON_NP){
		CStateNodeList* childsDt=node.m_umbinarizedSubNodes;
		while(childsDt!=0){
			const CStateNode* dtTarg=childsDt->node;
			if ((*words)[dtTarg->lexical_head].tag.code()==PENN_TAG_DT && !(isLinked(&node,dtTarg))){
				bool firstCond=false;
				bool secCond=true;
				bool thirdCond=true;
				CStateNodeList* rightSisters=childsDt->next;
				if (rightSisters!=0){
					if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_DT){
						thirdCond=false;
					}
				}
				if (thirdCond){
					while(rightSisters!=0){
						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
							secCond=false;
						}
						//PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
						if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE ||
								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE ||
								(*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE){
							if (rightSisters->next!=0){
								//PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL
								if ((*words)[rightSisters->next->node->lexical_head].tag.code()==PENN_TAG_NOUN ||
									(*words)[rightSisters->next->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER ||
									(*words)[rightSisters->next->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL ||
								    (*words)[rightSisters->next->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL){
									firstCond=true;
								}
							}
						}
						rightSisters=rightSisters->next;
					}
				}
				
				
				if (firstCond && secCond && thirdCond){
					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
					if (buildStanfordLink(label, dtTarg->lexical_head, node.lexical_head)) {
						addLinked(&node,dtTarg);
						return true;
					}
				}
			}
			childsDt=childsDt->next;
		}
	 }
	 return false;
}





//"WHNP < (NP $-- (WHNP=target < WDT))",
 bool det6(){
	 if (node.constituent==PENN_CON_WHNP){
		 CStateNodeList* childsWhnp=node.m_umbinarizedSubNodes;
		 while(childsWhnp!=0){
			 const CStateNode* npChild=childsWhnp->node;
			 if (npChild->constituent==PENN_CON_NP){
				 CStateNodeList* leftSisters=childsWhnp->previous;
				 while(childsWhnp!=0){
					 const CStateNode* whnpTarg=childsWhnp->node;
					 if (whnpTarg->constituent==PENN_CON_WHNP && !(isLinked(&node,whnpTarg))){
						 CStateNodeList* whnpChilds=whnpTarg->m_umbinarizedSubNodes;
						 while(whnpChilds!=0){
							 if ((*words)[whnpChilds->node->lexical_head].tag.code()==PENN_TAG_WDT){
								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
								 if (buildStanfordLink(label, whnpTarg->lexical_head, node.lexical_head)) {
									 addLinked(&node,whnpTarg);
								     return true;
								 }
							 }
							 whnpChilds=whnpChilds->next;
						 }
					 }
					 childsWhnp=childsWhnp->previous;
				 }
			 }
			 childsWhnp=childsWhnp->next;
		 }
	 }
	 return false;
 }


 //"@WHNP|ADVP < (/^(?:NP|NN|CD|RBS)/ $-- DT|WDT|WP=target)"
   bool det7(){
  	 if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_ADVP){
  		 CStateNodeList* childsWHAD=node.m_umbinarizedSubNodes;
  		 while(childsWHAD!=0){
  			 if (childsWHAD->node->constituent==PENN_CON_NP||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_NOUN||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_CD||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE){
  				 
  				 CStateNodeList* leftSisters=childsWHAD->previous;
  				 while(leftSisters!=0){
  					 const CStateNode* targ=leftSisters->node;
  					 if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN||
  							 (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN||
  							 (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN) && !isLinked(&node,targ)){
  						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
  						 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
  							 addLinked(&node,targ);
  						 	 return true;
  						 }
  					 }
  					 leftSisters=leftSisters->previous;
  				 }
  				 
  			 }
  			 childsWHAD=childsWHAD->next;
  		 }
  	 }
  	 return false;
   }


