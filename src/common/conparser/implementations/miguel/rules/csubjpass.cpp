
      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP < (VP < VBN|VBD) < (/^(?:VB|AUXG?)/ < " + passiveAuxWordRegex + ") !$-- NP))",
      bool csubjpass1(){
    	  if (node.constituent==PENN_CON_S){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (targ->constituent==PENN_CON_SBAR || targ->constituent==PENN_CON_S){
    				  bool commaCond=true;
    				  CStateNodeList* rightSisters=childs->next;
    				  if (rightSisters!=0){
    					  if ((*words)[rightSisters->node->lexical_head].word==g_word_comma){
    						  commaCond=false;
    					  }
    				  }
    				  if (commaCond){
    					  while(rightSisters!=0){
    						  if (rightSisters->node->constituent==PENN_CON_VP){
    							  bool leftSisCond=true;
    							  CStateNodeList* leftSisters=rightSisters->previous;
    							  while(leftSisters!=0){
    								  if (leftSisters->node->constituent==PENN_CON_NP){
    									  leftSisCond=false;
    								  }
    								  leftSisters=leftSisters->previous;
    							  }
    							  if (leftSisCond){
    								  CStateNodeList* childsVp=rightSisters->node->m_umbinarizedSubNodes;
    								  bool firstCond=false;
    								  bool secCond=false;
    								  //< (VP < VBN|VBD) < (/^(?:VB|AUXG?)/ < " + passiveAuxWordRegex + ")
    								  while(childsVp!=0){
    									  if (childsVp->node->constituent==PENN_CON_VP){
    										  CStateNodeList* childsVpIn=childsVp->node->m_umbinarizedSubNodes;
    										  while(childsVpIn!=0){
    											  if ((*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST
    													  || (*words)[childsVpIn->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){
    												  firstCond=true;
    											  }
    											  childsVpIn=childsVpIn->next;
    										  }
    									  }
    									  if ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB){
    										  CStateNodeList* childsVb=childsVp->node->m_umbinarizedSubNodes;
    										  while(childsVb!=0){
    											  if (compareWordToPassiveAuxWordRegex((*words)[childsVb->node->lexical_head].word)) {
    												  secCond=true;
    											  }
    											  childsVb=childsVb->next;
    										  }
    									  }
    									  childsVp=childsVp->next;
    								  }
    								  
    								  if (firstCond && secCond){
    									  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
    									  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
    										  addLinked(&node,targ);
    									  	  return true;
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
      
//"S < (SBAR|S=target !$+ /^,$/ $++ (VP <+(VP) (VP < VBN|VBD > (VP < (/^(?:VB|AUX)/ < " + passiveAuxWordRegex + "))) !$-- NP))"
bool csubjpass2(){

}
