/* 
    	"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
          // match "give it next week"
          
          "VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
          
         // basic direct object cases
          "VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )",
          // cdm Jul 2010: No longer require WHNP as first child of SBARQ below: often not because of adverbials, quotes, etc., and removing restriction does no harm
          // this next pattern used to assume no empty NPs. Corrected.  If you adjust this pattern, also adjust the corresponding one for attr!
         
         "SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
         
          // matches direct object in relative clauses "I saw the book that you bought"
          "SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",

          // matches direct object in relative clauses "I saw the book that you said you bought"
          "SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",

          // matches direct object for long dependencies in relative clause without explicit relative pronouns
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
          
          "SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"
*/



    /*
       *"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")"
       *
       */
       bool buildDobj1() {
    	   if (node.constituent==PENN_CON_VP) {
    		   CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		   bool secondCondition=true;
    		   while(childsVp!=0){
    			   const CStateNode* vbChildVp=childsVp->node;
    			   if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)) {
    				   CStateNodeList* childsVB=vbChildVp->m_umbinarizedSubNodes;
    			       while(childsVB!=0){
    			    	   if ((compareWordToCopularWordRegex((*words)[childsVB->node->lexical_head].word))) {
    			    		   secondCondition=false;
    			    	   }
    			       	   childsVB=childsVB->next;
    			       	}
    			   }
    			   childsVp=childsVp->next;
    		   }

    		   if (secondCondition) { //VP does not a have child that looks like (/^(?:VB|AUX)/ < " + copularWordRegex + ")
    			   childsVp=node.m_umbinarizedSubNodes;
    			   while(childsVp!=0){
    				   const CStateNode* npChildVp=childsVp->node;

    				   if (npChildVp->constituent==PENN_CON_NP) {
    					   	  //now the immediate left sister must look like (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))
    					   	   CStateNodeList* previous=childsVp->previous;
    					   	   if (previous!=0) {
    					   		   const CStateNode* leftSisterNpTarg=previous->node;

    					   		   if ((leftSisterNpTarg->constituent==PENN_CON_NP || leftSisterNpTarg->constituent==PENN_CON_WHNP) && !(isLinked(&node, leftSisterNpTarg))) {
    					   			   bool noChildNNTime=true;
    					   			   CStateNodeList* childsOfTarg=leftSisterNpTarg->m_umbinarizedSubNodes;
    					   			   while(childsOfTarg!=0){
    					   				   const CStateNode* nnChildNp=childsOfTarg->node;
    					   				   if ((nnChildNp->type==CStateNode::LEAF)
    					   					   && (nnChildNp->lexical_head==leftSisterNpTarg->lexical_head) //<#
    					   					   && (compareWordToTimeLotWordRegex((*words)[nnChildNp->lexical_head].word))
    					   					   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
    					   					   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
    					   					   noChildNNTime=false;
    					   					}
    					   				   childsOfTarg=childsOfTarg->next;
    					   			   }


    					   			   if (noChildNNTime) {
    					   				   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DOBJ);
    					   				   if (buildStanfordLink(label, leftSisterNpTarg->lexical_head, node.lexical_head)) {
    					   					   addLinked(&node,leftSisterNpTarg);
    					   					   return true;
    					   				   }
    					   			   }
    					   	   	   }
    					   	   }

    				   }
    				   childsVp=childsVp->next;
    			   }
    		   }
    	   }
    	   return false;

       }

 bool buildDobj2() {

 }

 bool buildDobj3() {

 }

 bool buildDobj4() {

 }

 bool buildDobj5() {

 }

 bool buildDobj6() {

 }

 bool buildDobj7() {

 }

 bool buildDobj8() {

 }
