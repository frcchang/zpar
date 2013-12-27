  //"/^(?:NP(?:-TMP|-ADV)?|NX|NML|NAC|WHNP)$/ < (ADJP|WHADJP|JJ|JJR|JJS|JJP|VBN|VBG|VBD|IN=target !< QP !$- CC)",
inline const bool &amod1(const unsigned long &cons){
    	 if (cons==PENN_CON_NP || cons==PENN_CON_NX || cons==PENN_CON_NAC ||cons==PENN_CON_WHNP){
    		 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		 while(childs!=0){
    			 const CStateNode* targ=childs->node; //JJP????
    			 //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
    			 if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADJP || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADJP||(*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE||
    					 (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE||(*words)[targ->lexical_head].tag.code()==PENN_TAG_IN||
    					 (*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB_PROG || (*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB_PRES || (*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){

    				 bool secCond=true;
    				 bool thirdCond=true;

    				 if (childs->previous!=0){
    					 if ((*words)[childs->previous->node->lexical_head].tag.code()==PENN_TAG_CC){
    						 thirdCond=false;
    					 }
    				 }
    				 if (thirdCond){
    					 CStateNodeList* childsAd=targ->m_umbinarizedSubNodes;
    					 while(childsAd!=0){
    						 if (CConstituent::clearTmp(childsAd->node->constituent.code())==PENN_CON_QP){
    							 secCond=false;
    						 }
    						 childsAd=childsAd->next;
    					 }
    				 }
    				 if (secCond && thirdCond){
//    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
    					 if (buildStanfordLink(STANFORD_DEP_AMOD, targ->lexical_head, node.lexical_head)) {
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



//"ADJP !< CC|CONJP < (JJ|NNP $ JJ|NNP=target)",
inline const bool &amod2(const unsigned long &cons){
	 if (cons==PENN_CON_ADJP){
		 bool firstCondition=true;
		 CStateNodeList* childsADJP=node.m_umbinarizedSubNodes;
		 while(childsADJP!=0){
			 if (CConstituent::clearTmp(childsADJP->node->constituent.code())==PENN_CON_CONJP || (*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_CC){
				 firstCondition=false;
			 }
			 childsADJP=childsADJP->next;
		 }

		 if (firstCondition){
			 childsADJP=node.m_umbinarizedSubNodes;
			 while(childsADJP!=0){
				 if ((*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE ||(*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER){
					 CStateNodeList* rightSisters=childsADJP->next;
					 CStateNodeList* leftSisters=childsADJP->previous;
					 while(rightSisters!=0){
						 const CStateNode* jjnnpTarg=rightSisters->node;
						 if (((*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_ADJECTIVE
								 ||(*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER) && !(isLinked(&node,jjnnpTarg))){
//							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
							 if (buildStanfordLink(STANFORD_DEP_AMOD, jjnnpTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,jjnnpTarg);
							     return true;
							 }
						 }
						 rightSisters=rightSisters->next;
					 }
					 while(leftSisters!=0){
						 const CStateNode* jjnnpTarg=leftSisters->node;
						 if (((*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_ADJECTIVE
								 ||(*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER) && !(isLinked(&node,jjnnpTarg))){
//						     CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
						     if (buildStanfordLink(STANFORD_DEP_AMOD, jjnnpTarg->lexical_head, node.lexical_head)) {
						    	 addLinked(&node,jjnnpTarg);
						     	 return true;
						     }
						  }
						 leftSisters=leftSisters->previous;
					 }
				 }
				 childsADJP=childsADJP->next;
			 }
		 }
	 }
	 return false;
}


//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (NP=target <: CD $- /^,$/ $-- /^(?:WH)?NP/ !$ CC|CONJP)",
inline const bool &amod3(const unsigned long &cons){
   	 if (cons==PENN_CON_WHNP||cons==PENN_CON_NP){
   		 CStateNodeList* childsWhNp=node.m_umbinarizedSubNodes;
   		 while(childsWhNp!=0){
   			 const CStateNode* npTarg=childsWhNp->node;
   			 if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && !isLinked(&node,npTarg)){
   				 bool firstCond=false;
   				 bool secCond=false;
   				 bool thirdCond=false;
   				 bool fourthCond=true;
   				 if (npTarg->m_umbinarizedSubNodes!=0){
   					 if (npTarg->m_umbinarizedSubNodes->next==0 && (*words)[npTarg->m_umbinarizedSubNodes->node->lexical_head].tag.code()==PENN_TAG_CD){
   						 firstCond=true;
   					 }
   				 }
   				 if (firstCond){
   					 CStateNodeList* leftSisters=childsWhNp->previous;
   					 if (leftSisters!=0){
   						 if ((*words)[leftSisters->node->lexical_head].word==g_word_comma){
   							 secCond=true;
   						 }
   					 }
   					 if (secCond){
   						 while(leftSisters!=0){
   							 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHNP
   									 || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHADJP
   									 || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHADVP
   									 || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHPP
   									 || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHNP) {
   								 thirdCond=true;
   							 }
   							 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_CONJP
   									 || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
   								 fourthCond=false;
   							 }
   							 leftSisters=leftSisters->previous;
   						 }
   						 if (thirdCond && fourthCond){
   							 CStateNodeList* rightSisters=childsWhNp->next;
   							 while(rightSisters!=0){
   								 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_CONJP
   										 || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
   									 fourthCond=false;
   								 }
   								 rightSisters=rightSisters->next;
   							 }
   						 }
   					 }
   				 }

   				 if (firstCond && secCond && thirdCond && fourthCond){
//   					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
   					 if (buildStanfordLink(STANFORD_DEP_AMOD, npTarg->lexical_head, node.lexical_head)) {
   						 addLinked(&node,npTarg);
   					     return true;
   					 }
   				 }
   			 }
   			 childsWhNp=childsWhNp->next;
   		 }
   	 }
   	 return false;
    }



