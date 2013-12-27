
   //"NP|WHNP|NML $++ (SBAR=target <+(SBAR) WHPP|WHNP) !$-- NP|WHNP|NML > @NP|WHNP",
    inline const bool &rcmod1(const unsigned long &cons){
    	  if (cons==PENN_CON_NP || cons==PENN_CON_WHNP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* head=childs->node;
    			  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHNP){
    				  bool negCondition=true;
    				  CStateNodeList* leftSisters=childs->previous;
    				  while(leftSisters!=0){
    					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP
    							  || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHNP){
    						  negCondition=false;
    					  }
    					  leftSisters=leftSisters->previous;
    				  }
    				  if (negCondition){
    					  CStateNodeList* rightSisters=childs->next;
    					  while(rightSisters!=0){
    						  const CStateNode* sbarTarg=rightSisters->node;
    						  if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
    							  CStateNodeList* sbarChain=new CStateNodeList();
    							  //void findChain(CConstituent via_category, CConstituent target_category, const CStateNode* head, CStateNodeList*& candidates)
    							  findChain(PENN_CON_SBAR,PENN_CON_WHPP, sbarTarg, sbarChain);
    							  //void findChain(CConstituent via_category, CConstituent target_category, const CStateNode* head, CStateNodeList*& candidates)
    							  findChain(PENN_CON_SBAR,PENN_CON_WHNP, sbarTarg, sbarChain);
    							  if (sbarChain->node==0) {
    								  sbarChain->clear();
    								  sbarChain=0;
    							  }
    							  else {
//    								  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_RCMOD);
    								  if (buildStanfordLink(STANFORD_DEP_RCMOD, sbarTarg->lexical_head, head->lexical_head)) {
    									  addLinked(&node,sbarTarg);
    								      return true;
    								  }
    							  }

    						  }
    						  rightSisters=rightSisters->next;
    					  }
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }

      //"NP|WHNP|NML $++ (SBAR=target <: (S !<, (VP <, TO))) !$-- NP|WHNP|NLP > @NP|WHNP",
    inline const bool &rcmod2(const unsigned long &cons){
          	  if (cons==PENN_CON_NP || cons==PENN_CON_WHNP){
          		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
          		  while(childs!=0){
          			  const CStateNode* head=childs->node;
          			  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHNP){
          				  bool leftSistCond=true;
          				  CStateNodeList* leftSisters=childs->previous;
          				  while(leftSisters!=0){
          					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP
          							  ||CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHNP) { //¿? NLP
          						  leftSistCond=false;
          					  }
          					  leftSisters=leftSisters->previous;
          				  }

          				  if (leftSistCond){
          					  CStateNodeList* rightSisters=childs->next;
          					  while(rightSisters!=0){
          						  const CStateNode* sbarTarg=rightSisters->node;
          						  if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
          							  CStateNodeList* childsSbar=sbarTarg->m_umbinarizedSubNodes;
          							  if(childsSbar!=0){
          								  if (CConstituent::clearTmp(childsSbar->node->constituent.code())==PENN_CON_S && childsSbar->next==0){
          									  bool inCond=true;
          									  CStateNodeList* childsS=childsSbar->node->m_umbinarizedSubNodes;
          									  if(childsS!=0){
          										  const CStateNode* vp=childsS->node;
          										  if (CConstituent::clearTmp(vp->constituent.code())==PENN_CON_VP){
          											  CStateNodeList* childsVp=vp->m_umbinarizedSubNodes;
          											  if(childsVp!=0){
          												  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO){
          													  inCond=false;
          												  }
          											  }
          										  }
          									  }
          									  if (inCond){
//          										  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_RCMOD);
          										  if (buildStanfordLink(STANFORD_DEP_RCMOD, sbarTarg->lexical_head, head->lexical_head)) {
          											  addLinked(&node,sbarTarg);
          										      return true;
          										  }
          									  }
          								  }
          							  }
          						  }
          						  rightSisters=rightSisters->next;
          					  }
          				  }
          			  }
          			  childs=childs->next;
          		  }
          	  }
          	  return false;
            }

            //"NP|NML $++ (SBAR=target < (WHADVP < (WRB </^(?i:where|why|when)/))) !$-- NP|NML > @NP",
    inline const bool &rcmod3(const unsigned long &cons){
              	  if (cons==PENN_CON_NP){
              		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
              		  while(childs!=0){
              			  const CStateNode* head=childs->node;
              			  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_NP){
              				  bool leftCond=true;
              				  CStateNodeList* leftSisters=childs->previous;
              				  while(leftSisters!=0){
              					  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP){
              						  leftCond=false;
              					  }
              					  leftSisters=leftSisters->previous;
              				  }
              				  if (leftCond){
              					  CStateNodeList* rightSisters=childs->next;
              					  while(rightSisters!=0){
              						  const CStateNode* sbarTarg=rightSisters->node;
              						  if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
              							  CStateNodeList* childsSb=sbarTarg->m_umbinarizedSubNodes;
              							  while(childsSb!=0){
              								  if (CConstituent::clearTmp(childsSb->node->constituent.code())==PENN_CON_WHADVP){
              									  CStateNodeList* childsWh=childsSb->node->m_umbinarizedSubNodes;
              									  if ((*words)[childsWh->node->lexical_head].tag.code()==PENN_TAG_WRB){
              										  //CStateNodeList* childsWr=childsWh->node->m_umbinarizedSubNodes;
              										  //while(childsWr!=0){
              											  if (((*words)[childsWh->node->lexical_head].word==g_word_where) ||((*words)[childsWh->node->lexical_head].word==g_word_why)||((*words)[childsWh->node->lexical_head].word==g_word_when)){
//              												  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_RCMOD);
              												  if (buildStanfordLink(STANFORD_DEP_RCMOD, sbarTarg->lexical_head, head->lexical_head)) {
              													  addLinked(&node,sbarTarg);
              												      return true;
              												  }
              											  }
              											//  childsWr=childsWr->next;
              										  //}
              									  }
              								  }
              								  childsSb=childsSb->next;
              							  }
              						  }
              						  rightSisters=rightSisters->next;
              					  }
              				  }

              			  }
              			  childs=childs->next;
              		  }
              	  }
              	  return false;
                }




//"NP|WHNP|NML $++ RRC=target !$-- NP|WHNP|NML",
    inline const bool &rcmod4(const unsigned long &cons){
   	  CStateNodeList* childs=node.m_umbinarizedSubNodes;
   	  while(childs!=0){
   		  const CStateNode* head=childs->node;
   		  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHNP){
   			  CStateNodeList* rightSisters=childs->next;
   			  CStateNodeList* leftSisters=childs->previous;
   			  bool leftCond=true;
   			  while(leftSisters!=0){
   				  if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_WHNP){
   					  leftCond=false;
   				  }
   				  leftSisters=leftSisters->previous;
   			  }

   			  if (leftCond){
   				  while(rightSisters!=0){
   					  const CStateNode* targ=rightSisters->node;
   					  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_RRC && !isLinked(&node,targ)){
//   						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_RCMOD);
   						  if (buildStanfordLink(STANFORD_DEP_RCMOD, targ->lexical_head, head->lexical_head)) {
   							  addLinked(&node,targ);
   						      return true;
   						  }
   					  }
   					  rightSisters=rightSisters->next;
   				  }
   			  }
   		  }
   		  childs=childs->next;
   	  }
   	  return false;
     }


     //"@ADVP < (@ADVP < (RB < /where$/)) < @SBAR=target",
    inline const bool &rcmod5(const unsigned long &cons){
        	  if (cons==PENN_CON_ADVP){ //is this correct? or should I put whadvp?
        		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
        		  bool inCond=false;
        		  while(childs!=0){
        			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_ADVP || CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_WHADVP){
        				  CStateNodeList* childsAdvp=childs->node->m_umbinarizedSubNodes;
        				  while(childsAdvp!=0){
        					  if ((*words)[childsAdvp->node->lexical_head].tag.code()==PENN_TAG_ADVERB){
        						  //CStateNodeList* childsRb=childsAdvp->node->m_umbinarizedSubNodes;
        						  //while(childsRb!=0){
        							  if (((*words)[childsAdvp->node->lexical_head].word==g_word_where)){
        								  inCond=true;
        							  }
        							//  childsRb=childsRb->next;
        						  //}
        					  }
        					  childsAdvp=childsAdvp->next;
        				  }
        			  }
        			  childs=childs->next;
        		  }

        		  if (inCond){
        			  childs=node.m_umbinarizedSubNodes;
        			  while(childs!=0){
        				  const CStateNode* sbarTarg=childs->node;
        				  if (CConstituent::clearTmp(sbarTarg->constituent.code())==PENN_CON_SBAR && !isLinked(&node,sbarTarg)){
//        					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_RCMOD);
        					  if (buildStanfordLink(STANFORD_DEP_RCMOD, sbarTarg->lexical_head, node.lexical_head)) {
        						  addLinked(&node,sbarTarg);
        					      return true;
        					  }
        				  }
        				  childs=childs->next;
        			  }
        		  }
        	  }
        	  return false;
          }




