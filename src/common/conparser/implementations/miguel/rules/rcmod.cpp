//"NP|WHNP|NML $++ (SBAR=target <+(SBAR) WHPP|WHNP) !$-- NP|WHNP|NML > @NP|WHNP",
bool rcmod1(){

}

//"NP|WHNP|NML $++ (SBAR=target <: (S !<, (VP <, TO))) !$-- NP|WHNP|NLP > @NP|WHNP",
bool rcmod2(){

}


//"NP|NML $++ (SBAR=target < (WHADVP < (WRB </^(?i:where|why|when)/))) !$-- NP|NML > @NP",
bool rcmod3(){

}

//"NP|WHNP|NML $++ RRC=target !$-- NP|WHNP|NML",
     bool rcmod4(){
   	  CStateNodeList* childs=node.m_umbinarizedSubNodes;
   	  while(childs!=0){
   		  const CStateNode* head=childs->node;
   		  if (head->constituent==PENN_CON_NP || head->constituent==PENN_CON_WHNP){
   			  CStateNodeList* rightSisters=childs->next;
   			  CStateNodeList* leftSisters=childs->previous;
   			  bool leftCond=true;
   			  while(leftSisters!=0){
   				  if (leftSisters->node->constituent==PENN_CON_NP || leftSisters->node->constituent==PENN_CON_WHNP){
   					  leftCond=false;
   				  }
   				  leftSisters=leftSisters->previous;
   			  }
   			  
   			  if (leftCond){
   				  while(rightSisters!=0){
   					  const CStateNode* targ=rightSisters->node;
   					  if (targ->constituent==PENN_CON_RRC && !isLinked(&node,targ)){
   						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVCL);
   						  if (buildStanfordLink(label, targ->lexical_head, head->lexical_head)) {
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
bool rcmod5(){

}
