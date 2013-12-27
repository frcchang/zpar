
//"VP [ < (ADJP=target !$-- NP) |  < (/^VB/ $+ (@S=target < (@ADJP < /^JJ/ ! $-- @NP|S))) ]",

//this can be divided in 2 different rules

//"VP < (ADJP=target !$-- NP)
inline const bool &buildAComp1(const unsigned long &cons){
    	 if (cons==PENN_CON_VP){
    		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		 while(childsVp!=0){
    			 const CStateNode* adjpTarg=childsVp->node;
    			 if (CConstituent::clearTmp(adjpTarg->constituent.code())==PENN_CON_ADJP && !(isLinked(&node,adjpTarg))) {
    				 CStateNodeList* leftSistersAdjp=childsVp;
    				 if (leftSistersAdjp->previous!=0){
    					 leftSistersAdjp=leftSistersAdjp->previous;
    					 bool npCondition=true;
    					 while(leftSistersAdjp!=0){
    						 if (CConstituent::clearTmp(leftSistersAdjp->node->constituent.code())==PENN_CON_NP){
    							 npCondition=false;
    						 }
    						 leftSistersAdjp=leftSistersAdjp->previous;
    					 }
    					 if (npCondition){
//    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ACOMP);
    						 if (buildStanfordLink(STANFORD_DEP_ACOMP, adjpTarg->lexical_head, node.lexical_head)) {
    							 addLinked(&node,adjpTarg);
    						     return true;
    						 }   
    					 }
    				 }
    				 
    			 }
    			 childsVp=childsVp->next;
    		 }
    	 }
    	 return false;
     }


     //VP < (/^VB/ $+ (@S=target < (@ADJP < /^JJ/ ! $-- @NP|S)))
inline const bool &buildAComp2(const unsigned long &cons){
    	 if (cons==PENN_CON_VP){
    		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		 while(childsVp!=0){
    			 const CStateNode* vbChild=childsVp->node;
  
    			 if (((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB)||
    					 ((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||
    					 ((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    					 ((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
    					 ((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
    					 ((*words)[vbChild->lexical_head].tag.code()==PENN_TAG_VERB_PROG)) {
    				 if (childsVp->next!=0){ //right siter of vb
    					 const CStateNode* sTarg=childsVp->next->node;
    					 if ((CConstituent::clearTmp(sTarg->constituent.code())==PENN_CON_S)&& !(isLinked(&node,sTarg))){
    						 CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
    						 while(childsS!=0){
    							 const CStateNode* adjpChildS=childsS->node;
    							 //(@ADJP < /^JJ/ ! $-- @NP|S)))
    							 if (CConstituent::clearTmp(adjpChildS->constituent.code())==PENN_CON_ADJP){
    								 bool secondCondition=true;
    								 if (childsS->previous!=0){
    									 CStateNodeList* leftSisters=childsS->previous;
    									 while(leftSisters!=0){
    										 if (CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(leftSisters->node->constituent.code())==PENN_CON_S) {
    											 secondCondition=false;
    										 }
    										 leftSisters=leftSisters->previous;
    									 }
    									 if (secondCondition){
    										 CStateNodeList* childsAdjp=adjpChildS->m_umbinarizedSubNodes;
    										 while(childsAdjp!=0){
    											 //PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
    											 if (((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)||
    													 ((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)||
    													 ((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)||
    													 ((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)||
    													 ((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)) {
//    												 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ACOMP);
    												 if (buildStanfordLink(STANFORD_DEP_ACOMP, sTarg->lexical_head, node.lexical_head)) {
    													 addLinked(&node,sTarg);
    												     return true;
    												 }   
    											 }
    											 childsAdjp=childsAdjp->next;
    										 }
    									 }
    								 }
    							 }
    							 childsS=childsS->next;
    						 }
    					 } 
    				 }
    			 }
    			 childsVp=childsVp->next;
    		 }
    	 }
    	 return false;
     }
