//"VP < (/^(?:VB|AUX|POS)/=target < " + passiveAuxWordRegex + " ) < (VP|ADJP [ < VBN|VBD | < (VP|ADJP < VBN|VBD) < CC ] )",
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

