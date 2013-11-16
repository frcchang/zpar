 //"/^(?:(?:WH)?(?:NP|ADJP|ADVP|NX|NML)(?:-TMP|-ADV)?|VP|NAC|SQ|FRAG|PRN|X)$/ < (WHPP|WHPP-TMP|PP|PP-TMP=target !$- (@CC|CONJP $- __))",
     bool prep1(){
    	 //??NML????
    	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	 while(childs!=0){
    		 const CStateNode* head=childs->node;
    		 if (head->constituent==PENN_CON_WHNP||head->constituent==PENN_CON_WHADJP||head->constituent==PENN_CON_WHADVP||
    		     head->constituent==PENN_CON_NX||head->constituent==PENN_CON_VP||
    		     head->constituent==PENN_CON_NAC ||head->constituent==PENN_CON_SQ||head->constituent==PENN_CON_FRAG||
    		     head->constituent==PENN_CON_PRN||head->constituent==PENN_CON_X) {
    		    
    			 
    			 bool sistCond=true;
    			 if (childs->previous!=0){
    				 if (childs->previous->node->constituent==PENN_CON_CONJP || (*words)[childs->previous->node->lexical_head].tag.code()==PENN_TAG_CC){
    					if (childs->previous->previous!=0){
    						sistCond=false;
    					}
    				 }
    			 }
    			 if (sistCond!=0){
    				 CStateNodeList* childsHead=head->m_umbinarizedSubNodes;
    				 while(childsHead!=0){
    					 const CStateNode* whppTarg=childsHead->node;
    					 if ((whppTarg->constituent==PENN_CON_WHPP || whppTarg->constituent==PENN_CON_PP)&&!isLinked(&node,whppTarg)){
    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
    						 if (buildStanfordLink(label, whppTarg->lexical_head, head->lexical_head)) {
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
     bool prep2(){
    	 if (node.constituent==PENN_CON_WHPP||node.constituent==PENN_CON_PP){
    		 
    		 CStateNodeList* childsWhPP=node.m_umbinarizedSubNodes;
    		 bool secCond=true;
    		 while(childsWhPP!=0){
    			 if (childsWhPP->node->constituent==PENN_CON_CONJP || (*words)[childsWhPP->node->lexical_head].tag.code()==PENN_TAG_CC){
    				 secCond=false;
    			 }
    			 childsWhPP=childsWhPP->next;
    		 }
    		 childsWhPP=node.m_umbinarizedSubNodes;
    		 while(childsWhPP!=0){
    			 const CStateNode* whppTarg=childsWhPP->node;
    			 if ((whppTarg->constituent==PENN_CON_WHPP||whppTarg->constituent==PENN_CON_PP) && !isLinked(&node,whppTarg)){
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
    					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
    					 if (buildStanfordLink(label, whppTarg->lexical_head, node.lexical_head)) {
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
    bool prep3(){
   	 if (node.constituent==PENN_CON_S ||node.constituent==PENN_CON_SINV){
   		 CStateNodeList* childsSSinv=node.m_umbinarizedSubNodes;
   		 bool secCondition=false;
   		 while(childsSSinv!=0){
   			 if (childsSSinv->node->constituent==PENN_CON_VP||childsSSinv->node->constituent==PENN_CON_S){
   				 secCondition=true;
   			 }
   			 childsSSinv=childsSSinv->next;
   		 }
   		 childsSSinv=node.m_umbinarizedSubNodes;
   		 while(childsSSinv!=0){
   			 const CStateNode* ppTarg=childsSSinv->node;
   			 if (ppTarg->constituent==PENN_CON_PP && !(isLinked(&node,ppTarg))){
   				 bool inCond=true;
   				 CStateNodeList* childsPP=ppTarg->m_umbinarizedSubNodes;
   				 while(childsPP!=0){
   					 if (childsPP->node->constituent==PENN_CON_SBAR){
   						 inCond=false;
   					 }
   					 childsPP=childsPP->next;
   				 }
   				 if (inCond){
   					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
   					 if (buildStanfordLink(label, ppTarg->lexical_head, node.lexical_head)) {
   						 addLinked(&node,ppTarg);
   					     return true;
   					 }
   				 }
   			 }
   			 childsSSinv=childsSSinv->next;
   		 }
   	 }
    }
    
    
    
    //"SBAR|SBARQ < /^(?:WH)?PP/=target < S|SQ"
         bool prep4(){
        	 if (node.constituent==PENN_CON_SBAR || node.constituent==PENN_CON_SBARQ){
        		CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
        		bool secCondition=false;
        		while(childsSbar!=0){
        			if (childsSbar->node->constituent==PENN_CON_S ||childsSbar->node->constituent==PENN_CON_SQ){
        				secCondition=true;
        			}
        			childsSbar=childsSbar->next;
        		}
        		if (secCondition){
        			childsSbar=node.m_umbinarizedSubNodes;
        			while(childsSbar!=0){
        				const CStateNode* whTarg=childsSbar->node;
        				if ((whTarg->constituent==PENN_CON_PP||whTarg->constituent==PENN_CON_WHPP||
        						whTarg->constituent==PENN_CON_WHNP||whTarg->constituent==PENN_CON_WHADVP||whTarg->constituent==PENN_CON_WHADJP)
        						&& !isLinked(&node,whTarg)){
        					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREP);
        					if (buildStanfordLink(label, whTarg->lexical_head, node.lexical_head)) {
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




