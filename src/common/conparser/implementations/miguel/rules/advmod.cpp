
 //"/^(?:VP|ADJP|JJP|WHADJP|SQ?|SBARQ?|SINV|XS|RRC|(?:WH)?NP(?:-TMP|-ADV)?)$/ < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
inline const bool &advmod1(const unsigned long &cons){
    	  if (cons==PENN_CON_VP || cons==PENN_CON_ADJP || cons==PENN_CON_S || //S (SBAR) is there because because SQ? (SBARQ?) matches SQ and S, being Q optional
    			  cons==PENN_CON_SQ || cons==PENN_CON_SBAR || cons==PENN_CON_SBARQ ||
    			  cons==PENN_CON_WHNP || cons==PENN_CON_SBARQ ||
    			  cons==PENN_CON_SINV || cons==PENN_CON_RRC || cons==PENN_CON_SBARQ){
    		  CStateNodeList* childs = node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE 
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE || (*words)[targ->lexical_head].tag.code()==PENN_TAG_WRB
    					  || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADVP) && !isLinked(&node,targ)){
    				  
    				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    				  bool notCond=true;
    				  if (((*words)[targ->lexical_head].word==g_word_not) ||((*words)[targ->lexical_head].word==g_word_nt)){
    					  notCond=false;
    				  }
    				  
    				  if (notCond){
//    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
    					  if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
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
            


//"ADVP|WHADVP < (RB|RBR|RBS|WRB|ADVP|WHADVP|JJ=target !< " + NOT_PAT + ") !< CC !< CONJP",
inline const bool &advmod2(const unsigned long &cons){
  	  if (cons==PENN_CON_ADVP||cons==PENN_CON_WHADVP){
  		  bool ccCond=true;
  		  bool conjpCond=true;
  		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
  		  while(childs!=0){
  			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_CONJP){
  				  conjpCond=false;
  			  }
  			  else if (((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC)){
  				  ccCond=false;
  			  }
  			  childs=childs->next;
  		  }
  		  if (ccCond && conjpCond){
  			  childs=node.m_umbinarizedSubNodes;
  			  while(childs!=0){
  				  const CStateNode* targ=childs->node;
  				  if ((CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADVP
  						  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB ||
  						  (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE ||
  						CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADVP || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADJECTIVE ||
  						  (*words)[targ->lexical_head].tag.code()==PENN_TAG_WRB) && !isLinked(&node,targ)){
  					  bool notCond=true;
  					  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
  					  if (((*words)[targ->lexical_head].word==g_word_not) ||((*words)[targ->lexical_head].word==g_word_nt)){
  						  notCond=false;
  					  }

  					  if (notCond){
//  						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
  						  if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
  							  addLinked(&node,targ);
  						      return true;
  						  }
  					  }
  				  }
  				  childs=childs->next;
  			  }
  		  }
  	  }
  	  return false;
    }

    //"SBAR < (WHNP=target < WRB)",
inline const bool &advmod3(const unsigned long &cons){
  	  if (cons==PENN_CON_SBAR){
  		  CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
  		  while(childsSbar!=0){
  			  const CStateNode* targ=childsSbar->node;
  			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHNP && !isLinked(&node,targ)){
  				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
  				  while(childsT!=0){
  					  if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_WRB)){
//  						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
  						  if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
  							  addLinked(&node,targ);
  						      return true;
  						  }
  					  }
  					  childsT=childsT->next;
  				  }
  			  }
  			  childsSbar=childsSbar->next;
  		  }
  	  }
  	  return false;
    }



    //"SBARQ <, WHADVP=target"
inline const bool &advmod31(const unsigned long &cons){
    	  if (cons==PENN_CON_SBARQ){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  if (childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADVP && !isLinked(&node,targ)){
//    				  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
    				  if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
    					  addLinked(&node,targ);
    				      return true;
    				 }
    			  }
    		  }
    	  }
    	  return false;
      }
      

//"XS < JJ=target",
/*bool advmod32(){
//We can safely remove it, there is no XS in our data set.
}*/

      //"/(?:WH)?PP(?:-TMP|-ADV)?$/ <# (__ $-- (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + "))",
inline const bool &advmod4(const unsigned long &cons){
        	  if (cons==PENN_CON_PP || cons==PENN_CON_WHPP){
        		  CStateNodeList* childs=node.m_umbinarizedSubNodes;

        		  while(childs!=0){
        			  if (childs->node->lexical_head==node.lexical_head){//<#
        				  CStateNodeList* leftSisters=childs->previous;
        				  while(leftSisters!=0){
        					  const CStateNode* targ=childs->node;
        					  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE ||   					          						  (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE || (*words)[targ->lexical_head].tag.code()==PENN_TAG_WRB ||
        							  CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADVP
        							  || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADVP) && !isLinked(&node,targ)){
        						  bool notCond=true;
        						  if (((*words)[targ->lexical_head].word==g_word_not) ||((*words)[targ->lexical_head].word==g_word_nt)){
        							  notCond=false;
        						  }
        						  if (notCond){
//        							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
        						      if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
        						    	  addLinked(&node,targ);
        						          return true;
        						          }
        						      }
        					  }
        					  leftSisters=leftSisters->previous;
        				  }
        			  }
        			  childs=childs->next;
        		  }
        	  }
          }

          //"/(?:WH)?PP(?:-TMP|-ADV)?$/ < @NP|WHNP < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
inline const bool &advmod5(const unsigned long &cons){
            	  if (cons==PENN_CON_PP || cons==PENN_CON_WHPP){
            		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
            		  bool npwhCond=false;
            		  while(childs!=0){
            			  if (CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_WHNP){
            				  npwhCond=true;
            			  }
            			  childs=childs->next;
            		  }
            		  if (npwhCond){
            			  childs=node.m_umbinarizedSubNodes;
            			  while(childs!=0){
            				  const CStateNode* targ=childs->node;
            				  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB || (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE ||
            						  (*words)[targ->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE || (*words)[targ->lexical_head].tag.code()==PENN_TAG_WRB ||
            						  CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADVP || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADVP) && !isLinked(&node,targ)){


            					  bool notCond=true;
            					  if (((*words)[targ->lexical_head].word==g_word_not) ||((*words)[targ->lexical_head].word==g_word_nt)){
            						  notCond=false;
            					  }
            					  if (notCond){
//            						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
            						  if (buildStanfordLink(STANFORD_DEP_ADVMOD, targ->lexical_head, node.lexical_head)) {
            							  addLinked(&node,targ);
            						      return true;
            						  }
            					  }

            				  }
            				  childs=childs->next;
            			  }
            		  }
            	  }
            	  return false;
              }



//"CONJP < (RB=target !< " + NOT_PAT + ")",
inline const bool &advmod6(const unsigned long &cons){
   	 if (cons==PENN_CON_CONJP){
   		 CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
   		 while(childsConjp!=0){
   			 const CStateNode* rbTarg=childsConjp->node;
   			 if ((*words)[rbTarg->lexical_head].tag.code()==PENN_TAG_ADVERB && !isLinked(&node, rbTarg)){
   				 bool notCond=true;
   				 if (((*words)[rbTarg->lexical_head].word==g_word_not) ||((*words)[rbTarg->lexical_head].word==g_word_nt)){
   					 notCond=false;
   				 }
   				 if (notCond){
//   					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
   					 if (buildStanfordLink(STANFORD_DEP_ADVMOD, rbTarg->lexical_head, node.lexical_head)) {
   						 addLinked(&node,rbTarg);
   					     return true;
   					 }
   				 }
   			 }
   			 childsConjp=childsConjp->next;

   		 }
   	 }
   	 return false;
    }


