
//"VP [ < (ADJP=target !$-- NP) |  < (/^VB/ $+ (@S=target < (@ADJP < /^JJ/ ! $-- @NP|S))) ]",

//this can be divided in 2 different rules

//"VP < (ADJP=target !$-- NP)
     bool buildAComp1(){
    	 if (node.constituent==PENN_CON_VP){
    		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		 while(childsVp!=0){
    			 const CStateNode* adjpTarg=childsVp->node;
    			 if (adjpTarg->constituent==PENN_CON_ADJP && !(isLinked(&node,adjpTarg))) {
    				 CStateNodeList* leftSistersAdjp=childsVp;
    				 if (leftSistersAdjp->previous!=0){
    					 leftSistersAdjp=leftSistersAdjp->previous;
    					 bool npCondition=true;
    					 while(leftSistersAdjp!=0){
    						 if (leftSistersAdjp->node->constituent==PENN_CON_NP){
    							 npCondition=false;
    						 }
    						 leftSistersAdjp=leftSistersAdjp->previous;
    					 }
    					 if (npCondition){
    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ACOMP);
    						 if (buildStanfordLink(label, adjpTarg->lexical_head, node.lexical_head)) {
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
bool buildAComp2(){}
