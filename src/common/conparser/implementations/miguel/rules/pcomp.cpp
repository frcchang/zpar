//"@PP|WHPP < (IN|VBG|VBN|TO $+ @SBAR|S|PP|ADVP=target)", // no intervening NP; VBN is for "compared with"
inline const bool & buildPComp1(const unsigned long &cons) {
	if (cons==PENN_CON_WHPP || cons==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
		 CStateNodeList* childsPPWhpp=node.m_umbinarizedSubNodes;
		 while(childsPPWhpp!=0){
			 const CStateNode* rbChild=childsPPWhpp->node;
			 if (((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_IN)||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
					 ||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_TO)) {
				 if (childsPPWhpp->previous!=0){
					 const CStateNode* sbarSTarg=childsPPWhpp->previous->node;
					 if ((CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_S||CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_SBAR
							 ||CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_PP||CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_ADVP)
							 && !(isLinked(&node, sbarSTarg))){
						 //again, what is the basic category of SBAR????, what is @SBAR?
//						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
		    			if (buildStanfordLink(STANFORD_DEP_PCOMP, sbarSTarg->lexical_head, node.lexical_head)) {
		    				addLinked(&node,sbarSTarg);
		    				return true;
		    			}
						 
					 }
				 }
			 }
			 childsPPWhpp=childsPPWhpp->next;
		 }
	 }
	 return false;
	
}

//"@PP|WHPP < (RB $+ @SBAR|S=target)", // RB is for weird tagging like "after/RB adjusting for inflation"
inline const bool &buildPComp2(const unsigned long &cons) {
   	 if (cons==PENN_CON_WHPP || cons==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
   		 CStateNodeList* childsPPWhpp=node.m_umbinarizedSubNodes;
   		 while(childsPPWhpp!=0){
   			 const CStateNode* rbChild=childsPPWhpp->node;
   			 if (((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_ADVERB)) {
   				 if (childsPPWhpp->previous!=0){
   					 const CStateNode* sbarSTarg=childsPPWhpp->previous->node;
   					 if ((CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_S||CConstituent::clearTmp(sbarSTarg->constituent.code())==PENN_CON_SBAR)&& !(isLinked(&node, sbarSTarg))){
   						 //again, what is the basic category of SBAR????, what is @SBAR?
//   						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
			    			if (buildStanfordLink(STANFORD_DEP_PCOMP, sbarSTarg->lexical_head, node.lexical_head)) {
			    				addLinked(&node,sbarSTarg);
			    				return true;
			    			}
   					 }
   				 }
   			 }
   			 childsPPWhpp=childsPPWhpp->next;
   		 }
   	 }
   	 return false;
    }

//"@PP|WHPP !< IN|TO < (SBAR=target <, (IN $+ S))",
inline const bool &buildPComp3(const unsigned long &cons) {
	if (cons==PENN_CON_WHPP || cons==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
		CStateNodeList* childsPPWhpp=node.m_umbinarizedSubNodes;
		bool firstCond=true;
		while(childsPPWhpp!=0){
			//firstcond
			const CStateNode* childInto=childsPPWhpp->node;
			if (((*words)[childInto->lexical_head].tag.code()==PENN_TAG_IN)||((*words)[childInto->lexical_head].tag.code()==PENN_TAG_TO)) {
				firstCond=false;
			}
			childsPPWhpp=childsPPWhpp->next;
		}
		if (firstCond){
			childsPPWhpp=node.m_umbinarizedSubNodes;
			while(childsPPWhpp!=0){
				const CStateNode* sbarTarg=childsPPWhpp->node;
				if ((CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR)&& !(isLinked(&node, sbarTarg))){
					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
					if (childsSbar!=0){ //A <, B B is the first child of A
						const CStateNode* firstChildIN=childsSbar->node;
						//A $+ B (A is a inmediately left sister of B)
						if (childsSbar->next!=0){
							const CStateNode* sSister=childsSbar->next->node;
							if (CConstituent::clearTmp(sSister->constituent.code())==PENN_CON_S){
//								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
								if (buildStanfordLink(STANFORD_DEP_PCOMP, sbarTarg->lexical_head, node.lexical_head)) {
									addLinked(&node,sbarTarg);
									return true;
								}
							}
						}
					}
				}
				childsPPWhpp=childsPPWhpp->next;
			}
		}
	}
	return false;
	
}


