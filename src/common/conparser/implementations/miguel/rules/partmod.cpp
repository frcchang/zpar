
     //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (VP=target < VBG|VBN|VBD $-- NP)",
     inline const bool &partmod1(const unsigned long &cons){
    	 if (cons==PENN_CON_WHNP ||cons==PENN_CON_NP){
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 const CStateNode* vpTarg=childsNp->node;
    			 if (CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP && !(isLinked(&node,vpTarg))){
    				 CStateNodeList* childsVp=vpTarg->m_umbinarizedSubNodes;
    				 while(childsVp!=0){
    					 //PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
    					 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    					     ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||
    					     ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)){
    						 
    						 CStateNodeList* leftSisters=childsVp->previous;
    						 while(leftSisters!=0){
    							 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
//    								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARTMOD);
    								 if (buildStanfordLink(STANFORD_DEP_PARTMOD, vpTarg->lexical_head, node.lexical_head)) {
    									 addLinked(&node,vpTarg);
    								     return true;
    								 }
    							 }
    							 leftSisters=leftSisters->previous;
    						 }
    					 }
    					 childsVp=childsVp->next;
    				 }
    			 }
    			 childsNp=childsNp->next;
    		 }
    	 }
    	 return false;
     }



     //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (/^,$/ $+ (VP=target <, VBG|VBN))",
     inline const bool &partmod2(const unsigned long &cons){
    	 if (cons==PENN_CON_WHNP ||cons==PENN_CON_NP){
    		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
    		 while(childsNp!=0){
    			 if ((*words)[childsNp->node->lexical_head].word==g_word_comma){
    				 if (childsNp->next!=0){
    					 const CStateNode* vpTarg=childsNp->next->node;
    					 if (CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP && !(isLinked(&node,vpTarg))){
    						 CStateNodeList* childsVpt=vpTarg->m_umbinarizedSubNodes;
    						 if (childsVpt!=0){
    							 //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
    	    						if (((*words)[childsVpt->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
    	    								((*words)[childsVpt->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)){
//    	    							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARTMOD);
    	    							if (buildStanfordLink(STANFORD_DEP_PARTMOD, vpTarg->lexical_head, node.lexical_head)) {
    	    								addLinked(&node,vpTarg);
    	    							    return true;
    	    							}
    	    						}
    						 }
    					 }
    				 }
    			 }
    			 childsNp=childsNp->next;
    		 }
    	 }
    	 return false;
     }


     //"S|SINV < (S=target < (VP <, VBG|VBN) [ $- (/^,$/ [ $- @NP | $- (@PP $ @NP) ] ) | $+ (/^,$/ $+ @NP) ] )",
     inline const bool &partmod3(const unsigned long &cons){
        	 if (cons==PENN_CON_S || cons==PENN_CON_SINV){
        		 CStateNodeList* childsS=node.m_umbinarizedSubNodes;
        		 while(childsS!=0){
        			 const CStateNode* sTarg=childsS->node;
        			 if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S && !(isLinked(&node,sTarg))){
        				 bool secondCondition=false; //[ $- (/^,$/ [ $- @NP | $- (@PP $ @NP) ] )
        				 bool thirdCondition=false; //| $+ (/^,$/ $+ @NP) ]
        				 
        				 if (childsS->previous!=0){
        					 if ((*words)[childsS->previous->node->lexical_head].word==g_word_comma){
        						 if (childsS->previous->previous!=0){
        							 if (CConstituent::clearTmp(childsS->previous->previous->node->constituent.code())==PENN_CON_NP){
        						     	secondCondition=true;
        						     }
        							 else if (CConstituent::clearTmp(childsS->previous->previous->node->constituent.code())==PENN_CON_PP){
        								 CStateNodeList* rightSisters=childsS->previous;
        								 while(rightSisters!=0){
        									 if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP){
        										 secondCondition=true;
        									 }
        									 rightSisters=rightSisters->next;
        								 }
        								 if (childsS->previous->previous->previous!=0){
        									 CStateNodeList* leftSisters=childsS->previous->previous->previous;
        									 while(leftSisters!=0){
        										 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
        											 secondCondition=true;
        										 }
        										 leftSisters=leftSisters->previous;
        									 }
        								 }
        							 }
        						 }
        					 }
        				 }
        				 
        				 if (childsS->next!=0){
        					 if ((*words)[childsS->next->node->lexical_head].word==g_word_comma){
        						 if (childsS->next->next!=0){
        							 if (CConstituent::clearTmp(childsS->next->next->node->constituent.code())==PENN_CON_NP){
        								 thirdCondition=true;
        							 }
        						 }
        						 
        					 }
        				 }
        				 
        				 if (secondCondition || thirdCondition){
        					 CStateNodeList* childsStarg=sTarg->m_umbinarizedSubNodes;
        					 while(childsStarg!=0){
        						 const CStateNode* vpChild=childsStarg->node;
        						 if (CConstituent::clearTmp(vpChild->constituent.code())==PENN_CON_VP){
        							 CStateNodeList* childsVp=vpChild->m_umbinarizedSubNodes;
        							 if (childsVp!=0){
        								 //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
        								 if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
        										 (*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){
//        									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARTMOD);
        									 if (buildStanfordLink(STANFORD_DEP_PARTMOD, sTarg->lexical_head, node.lexical_head)) {
        										 addLinked(&node,sTarg);
        									     return true;
        									 }
        								 }
        							 }
        						 }
        						 childsStarg=childsStarg->next;
        					 }
        				 }
        			 }
        			 childsS=childsS->next;
        		 }
        	 }
        	 return false;
         }

         
         //"(VP < (@S=target < (VP <1 VBG|VBN )$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)])))",
     inline const bool &partmod4(const unsigned long &cons){
        	 if (cons==PENN_CON_VP){
        		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
        		 while(childsVp!=0){
        			 const CStateNode* sTarg=childsVp->node;
        			 if (CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_NP && !(isLinked(&node,sTarg))){
        				 bool secondCondition=false;
        				 if (childsVp->previous!=0){
        					 const CStateNode* comma=childsVp->previous->node;
        					 if ((*words)[comma->lexical_head].word==g_word_comma){
        						 bool firstInCond=false;
        						 bool secondInCond=false;
        						 bool thirdInCond=false;
        						 
        						 if (childsVp->previous->previous!=0){
        							 if (CConstituent::clearTmp(childsVp->previous->previous->node->constituent.code())==PENN_CON_NP){
        								 firstInCond=true;
        							 }
        							 if (!firstInCond){
        								 if (CConstituent::clearTmp(childsVp->previous->previous->node->constituent.code())==PENN_CON_PP){
        									 CStateNodeList* leftSisters=childsVp->previous->previous->previous;
        									 while(leftSisters!=0){
        										 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
        											 secondInCond=true;
        										 }
        										 leftSisters=leftSisters->previous;
        									 }
        								 }
        								 if (!secondInCond){
        									 if (CConstituent::clearTmp(childsVp->previous->previous->node->constituent.code())==PENN_CON_ADVP){
        										 CStateNodeList* leftSisters=childsVp->previous->previous->previous;
        									     while(leftSisters!=0){
        									    	 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
        									    		 thirdInCond=true;
        									     	 }
        									     	 leftSisters=leftSisters->previous;
        									     }
        									 } 
        								 }
        							 }
        						 }
        						 
        						 if (firstInCond||secondInCond||thirdInCond){
        							 secondCondition=true;
        						 }
        					 }
        				 }
        				 if (secondCondition){
        					 CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
        					 while(childsS!=0){
        						 const CStateNode* vpChild=childsS->node;
        						 if (CConstituent::clearTmp(vpChild->constituent.code())==PENN_CON_VP){
        							 CStateNodeList* childsVpIn=vpChild->m_umbinarizedSubNodes;
        							 if (childsVpIn!=0){
        								 //PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE
        								 if (((*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
        										((*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)){
//        									 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARTMOD);
        									 if (buildStanfordLink(STANFORD_DEP_PARTMOD, sTarg->lexical_head, node.lexical_head)) {
        										 addLinked(&node,sTarg);
        									     return true;
        									 }
        								 }
        							 }
        						 }
        						 childsS=childsS->next;
        					 }
        				 }
        			 }
        			 childsVp=childsVp->next;
        		 }
        	 }
        	 return false;
         }



