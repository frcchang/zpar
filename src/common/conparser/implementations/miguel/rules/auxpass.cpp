     //"VP < (/^(?:VB|AUX|POS)/=target < " + passiveAuxWordRegex + " ) < (VP|ADJP [ < VBN|VBD | < (VP|ADJP < VBN|VBD) < CC ] )",
     bool auxpass1(){
    	 if (node.constituent==PENN_CON_VP){
    		 bool secondCondition=false;

    		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		 while(childsVp!=0){
    			 const CStateNode* vpadjp=childsVp->node;
    			 if ((vpadjp->constituent==PENN_CON_VP)||(vpadjp->constituent==PENN_CON_ADJP)){
    				 CStateNodeList* childsVpadjp=vpadjp->m_umbinarizedSubNodes;
    				 while(childsVpadjp!=0){
    					 if ((*words)[childsVpadjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
                  				 (*words)[childsVpadjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
    						 secondCondition=true;
    					 }
    					 if (childsVpadjp->node->constituent==PENN_CON_VP || childsVpadjp->node->constituent==PENN_CON_ADJP){
    						 CStateNodeList* childsInt=childsVpadjp->node->m_umbinarizedSubNodes;
    						 while(childsInt!=0){
    							 if ((*words)[childsInt->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
    							     (*words)[childsInt->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
    								 CStateNodeList* childsVpadjp2=vpadjp->m_umbinarizedSubNodes;
    								 while(childsVpadjp2!=0) {
    									 if ((*words)[childsVpadjp2->node->lexical_head].tag.code()==PENN_TAG_CC) {
    										 secondCondition=true;
    									 }
    									 childsVpadjp2=childsVpadjp2->next;
    								 }
    							 }
    							 childsInt=childsInt->next;
    						 }
    					 }
    					 childsVpadjp=childsVpadjp->next;
    				 }
    			 }
    			 childsVp=childsVp->next;
    		 }

    		 if (secondCondition){
    			 childsVp=node.m_umbinarizedSubNodes;
    			 //VP < (/^(?:VB|AUX|POS)/=target < " + passiveAuxWordRegex + " )
    			 while(childsVp!=0){
    				 const CStateNode* verbTarg=childsVp->node;
    				 //PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
    				 if (((*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PRES ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)&&!(isLinked(&node,verbTarg))) {

    					 CStateNodeList* childsVerb=verbTarg->m_umbinarizedSubNodes;
    					 while(childsVerb!=0){
    						 if ((compareWordToPassiveAuxWordRegex((*words)[childsVerb->node->lexical_head].word))) {
    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
    							 if (buildStanfordLink(label, verbTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,verbTarg);
    							     return true;
    							 }
    						 }
    						 childsVerb=childsVerb->next;
    					 }

    				 }
    				 childsVp=childsVp->next;
    			 }
    		 }

    	 }
    	 return false;
     }

//bool auxpass1(){}

//"SQ|SINV < (/^(?:VB|AUX|POS)/=target < " + beAuxiliaryRegex + " $++ (VP < VBD|VBN))",
     bool auxpass2(){
    	 if (node.constituent==PENN_CON_SQ ||node.constituent==PENN_CON_SINV) {
    		 CStateNodeList* childsSqS=node.m_umbinarizedSubNodes;
    		 while(childsSqS!=0){
    			 const CStateNode* vbauxposTarg=childsSqS->node;
    			 //I don't know what AUX is...
    			 if (((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_POS) && !(isLinked(&node,vbauxposTarg))) {
    				 
    				 bool secondCondition=false;
    				 CStateNodeList* rightSisters=childsSqS->next;
    				 while(rightSisters!=0){
    					 const CStateNode* vpSist=rightSisters->node;
    					 if (vpSist->constituent==PENN_CON_VP){
    						 CStateNodeList* childsVp=vpSist->m_umbinarizedSubNodes;
    						 while(childsVp!=0){
    							 if (((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) {
    								 secondCondition=true;
    							 }
    							 childsVp=childsVp->next;
    						 }
    					 }
    					 rightSisters=rightSisters->next;
    				 }
    				 if (secondCondition){
    					 CStateNodeList* childsVb=vbauxposTarg->m_umbinarizedSubNodes;
    					 while(childsVb!=0){
    						 if (compareWordToBeAuxiliaryWordRegex((*words)[childsVb->node->lexical_head].word)) {
    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
    							 if (buildStanfordLink(label, vbauxposTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,vbauxposTarg);
    							     return true;
    							 }
    						 }
    						 childsVb=childsVb->next;
    					 }
    				 }
    				 
    			 }
    			 childsSqS=childsSqS->next;
    		 }
    	 }
    	 return false;
     }     

//"SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBD|VBN))",
bool auxpass3(){}


//"SINV < (VP=target < (VP < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ")) $-- (VP < VBD|VBN))",
bool auxpass4(){}

