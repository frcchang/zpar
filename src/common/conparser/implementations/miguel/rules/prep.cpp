 //"/^(?:(?:WH)?(?:NP|ADJP|ADVP|NX|NML)(?:-TMP|-ADV)?|VP|NAC|SQ|FRAG|PRN|X)$/ < (WHPP|WHPP-TMP|PP|PP-TMP=target !$- (@CC|CONJP $- __))",
inline const bool & prep1(const unsigned long &cons){
    	 //??NML????
    	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	 while(childs!=0){
    		 const CStateNode* head=childs->node;
    		 if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHNP||CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHADJP
    				 ||CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHADVP||
		 	 	 CConstituent::clearTmp(head->constituent.code())==PENN_CON_NX||CConstituent::clearTmp(head->constituent.code())==PENN_CON_VP||
		 	 	 CConstituent::clearTmp(head->constituent.code())==PENN_CON_NAC ||CConstituent::clearTmp(head->constituent.code())==PENN_CON_SQ||CConstituent::clearTmp(head->constituent.code())==PENN_CON_FRAG||
		 	 	 CConstituent::clearTmp(head->constituent.code())==PENN_CON_PRN||CConstituent::clearTmp(head->constituent.code())==PENN_CON_X) {
    		    
    			 
    			 bool sistCond=true;
    			 if (childs->previous!=0){
    				 if (CConstituent::clearTmp(childs->previous->node->constituent.code())==PENN_CON_CONJP || (*words)[childs->previous->node->lexical_head].tag.code()==PENN_TAG_CC){
    					if (childs->previous->previous!=0){
    						sistCond=false;
    					}
    				 }
    			 }
    			 if (sistCond!=0){
    				 CStateNodeList* childsHead=head->m_umbinarizedSubNodes;
    				 while(childsHead!=0){
    					 const CStateNode* whppTarg=childsHead->node;
    					 if ((CConstituent::clearTmp(whppTarg->constituent.code())==PENN_CON_WHPP || CConstituent::clearTmp(whppTarg->constituent.code())==PENN_CON_PP)&&!isLinked(&node,whppTarg)){
//    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
    						 if (buildStanfordLink(STANFORD_DEP_PREP, whppTarg->lexical_head, head->lexical_head)) {
    							 addLinked(&node,whppTarg);
    						     return true;
    						 }
    					 }
    					 childsHead=childsHead->next;
    				 }
    			 }
    			 
    		 }
    		 childs=childs->next;
    	 }
    	 return false;
     }

//"WHPP|WHPP-TMP|WHPP-ADV|PP|PP-TMP|PP-ADV < (WHPP|WHPP-TMP|WHPP-ADV|PP|PP-TMP|PP-ADV=target !$- IN|VBG|VBN|TO) !< @CC|CONJP",
inline const bool & prep2(const unsigned long &cons){
    	 if (cons==PENN_CON_WHPP||cons==PENN_CON_PP){
    		 
    		 CStateNodeList* childsWhPP=node.m_umbinarizedSubNodes;
    		 bool secCond=true;
    		 while(childsWhPP!=0){
    			 if (CConstituent::clearTmp(childsWhPP->node->constituent.code())==PENN_CON_CONJP || (*words)[childsWhPP->node->lexical_head].tag.code()==PENN_TAG_CC){
    				 secCond=false;
    			 }
    			 childsWhPP=childsWhPP->next;
    		 }
    		 childsWhPP=node.m_umbinarizedSubNodes;
    		 while(childsWhPP!=0){
    			 const CStateNode* whppTarg=childsWhPP->node;
    			 if ((CConstituent::clearTmp(whppTarg->constituent.code())==PENN_CON_WHPP||CConstituent::clearTmp(whppTarg->constituent.code())==PENN_CON_PP) && !isLinked(&node,whppTarg)){
    				 bool sisterCond=true;
    				 if (childsWhPP->previous!=0){
    					 //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
    					 if ((*words)[childsWhPP->previous->node->lexical_head].tag.code()==PENN_TAG_IN ||
    							 (*words)[childsWhPP->previous->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
    							 (*words)[childsWhPP->previous->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE ||
    							 (*words)[childsWhPP->previous->node->lexical_head].tag.code()==PENN_TAG_TO) {
    						 sisterCond=false;
    					 } 
    				 }
    				 if (sisterCond){
//    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
    					 if (buildStanfordLink(STANFORD_DEP_PREP, whppTarg->lexical_head, node.lexical_head)) {
    						 addLinked(&node,whppTarg);
    					     return true;
    					 }
    				 }
    			 }
    			 childsWhPP=childsWhPP->next;
    		 }
    	 }
    	 return false;

     }

//"S|SINV < (PP|PP-TMP=target !< SBAR) < VP|S",
inline const bool & prep3(const unsigned long &cons){
	 //std::cout<<"prep3 \n";
   	 if (cons==PENN_CON_S ||cons==PENN_CON_SINV){
   		 CStateNodeList* childsSSinv=node.m_umbinarizedSubNodes;
   		 bool secCondition=false;
   		 while(childsSSinv!=0){
   			 if (CConstituent::clearTmp(childsSSinv->node->constituent.code())==PENN_CON_VP||CConstituent::clearTmp(childsSSinv->node->constituent.code())==PENN_CON_S){
   				 secCondition=true;
   				 //std::cout<<"prep3 \n";
   				// std::cout<<((*words)[childsSSinv->node->lexical_head].word)<<"\n";
   			 }
   			 childsSSinv=childsSSinv->next;
   		 }
   		 if (secCondition) {
			 childsSSinv=node.m_umbinarizedSubNodes;
			 while(childsSSinv!=0){
				 const CStateNode* ppTarg=childsSSinv->node;
				 if (CConstituent::clearTmp(ppTarg->constituent.code())==PENN_CON_PP && !(isLinked(&node,ppTarg))){
					 bool inCond=true;
					 CStateNodeList* childsPP=ppTarg->m_umbinarizedSubNodes;
					 while(childsPP!=0){
						 if (CConstituent::clearTmp(childsPP->node->constituent.code())==PENN_CON_SBAR){
							 inCond=false;
						 }
						 childsPP=childsPP->next;
					 }
					 if (inCond){
	//   					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
						 if (buildStanfordLink(STANFORD_DEP_PREP, ppTarg->lexical_head, node.lexical_head)) {
							 addLinked(&node,ppTarg);
							 return true;
						 }
					 }
				 }
				 childsSSinv=childsSSinv->next;
			 }
   		 }
   	 }
    }
    
    
    
    //"SBAR|SBARQ < /^(?:WH)?PP/=target < S|SQ"
inline const bool & prep4(const unsigned long &cons){
        	 if (cons==PENN_CON_SBAR || cons==PENN_CON_SBARQ){
        		CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
        		bool secCondition=false;
        		while(childsSbar!=0){
        			if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_S ||CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_SQ){
        				secCondition=true;
        			}
        			childsSbar=childsSbar->next;
        		}
        		if (secCondition){
        			childsSbar=node.m_umbinarizedSubNodes;
        			while(childsSbar!=0){
        				const CStateNode* whTarg=childsSbar->node;
        				if ((CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_PP||CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHPP||
        						CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHNP||CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHADVP
        						||CConstituent::clearTmp(whTarg->constituent.code())==PENN_CON_WHADJP)
        						&& !isLinked(&node,whTarg)){
//        					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
        					if (buildStanfordLink(STANFORD_DEP_PREP, whTarg->lexical_head, node.lexical_head)) {
        						addLinked(&node,whTarg);
        					   	return true;
        					}
        					
        				}
        				childsSbar=childsSbar->next;
        			}
        		}
        	 }	
        	 return false;
         }




