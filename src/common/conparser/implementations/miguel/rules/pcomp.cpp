//"@PP|WHPP < (IN|VBG|VBN|TO $+ @SBAR|S|PP|ADVP=target)", // no intervening NP; VBN is for "compared with"
bool buildPComp1() {
	if (node.constituent==PENN_CON_WHPP || node.constituent==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
		 CStateNodeList* childsPPWhpp=node.m_umbinarizedSubNodes;
		 while(childsPPWhpp!=0){
			 const CStateNode* rbChild=childsPPWhpp->node;
			 if (((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_IN)||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_VERB_PROG)
					 ||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_TO)) {
				 if (childsPPWhpp->previous!=0){
					 const CStateNode* sbarSTarg=childsPPWhpp->previous->node;
					 if ((sbarSTarg->constituent==PENN_CON_S||sbarSTarg->constituent==PENN_CON_SBAR
							 ||sbarSTarg->constituent==PENN_CON_PP||sbarSTarg->constituent==PENN_CON_ADVP)
							 && !(isLinked(&node, sbarSTarg))){
						 //again, what is the basic category of SBAR????, what is @SBAR?
						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
		    			if (buildStanfordLink(label, sbarSTarg->lexical_head, node.lexical_head)) {
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
    bool buildPComp2() {
   	 if (node.constituent==PENN_CON_WHPP || node.constituent==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
   		 CStateNodeList* childsPPWhpp=node.m_umbinarizedSubNodes;
   		 while(childsPPWhpp!=0){
   			 const CStateNode* rbChild=childsPPWhpp->node;
   			 if (((*words)[rbChild->lexical_head].tag.code()==PENN_TAG_ADVERB)) {
   				 if (childsPPWhpp->previous!=0){
   					 const CStateNode* sbarSTarg=childsPPWhpp->previous->node;
   					 if ((sbarSTarg->constituent==PENN_CON_S||sbarSTarg->constituent==PENN_CON_SBAR)&& !(isLinked(&node, sbarSTarg))){
   						 //again, what is the basic category of SBAR????, what is @SBAR?
   						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
			    			if (buildStanfordLink(label, sbarSTarg->lexical_head, node.lexical_head)) {
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
bool buildPComp3() {
	if (node.constituent==PENN_CON_WHPP || node.constituent==PENN_CON_PP) { //what is the basic category of PP, or in diff words, what is @PP?
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
				if ((sbarTarg->constituent==PENN_CON_SBAR)&& !(isLinked(&node, sbarTarg))){
					CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
					if (childsSbar!=0){ //A <, B B is the first child of A
						const CStateNode* firstChildIN=childsSbar->node;
						//A $+ B (A is a inmediately left sister of B)
						if (childsSbar->next!=0){
							const CStateNode* sSister=childsSbar->next->node;
							if (sSister->constituent==PENN_CON_S){
								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PCOMP);
								if (buildStanfordLink(label, sbarTarg->lexical_head, node.lexical_head)) {
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


