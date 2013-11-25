
//"/^(?:VP|ADJP|JJP|WHADJP|SQ?|SBARQ?|SINV|XS|RRC|(?:WH)?NP(?:-TMP|-ADV)?)$/ < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
bool advmod1(){

}


//"ADVP|WHADVP < (RB|RBR|RBS|WRB|ADVP|WHADVP|JJ=target !< " + NOT_PAT + ") !< CC !< CONJP",
bool advmod2(){

}


//"SBAR < (WHNP=target < WRB)",
bool advmod3(){

}

//"SBARQ <, WHADVP=target"
bool advmod31(){

}

//"XS < JJ=target",
bool advmod32(){

}

//"/(?:WH)?PP(?:-TMP|-ADV)?$/ <# (__ $-- (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + "))",
bool advmod4(){

}


//"/(?:WH)?PP(?:-TMP|-ADV)?$/ < @NP|WHNP < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
bool advmod5(){

}

//"CONJP < (RB=target !< " + NOT_PAT + ")",
    bool advmod6(){
   	 if (node.constituent==PENN_CON_CONJP){
   		 CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
   		 while(childsConjp!=0){
   			 const CStateNode* rbTarg=childsConjp->node;
   			 if ((*words)[rbTarg->lexical_head].tag.code()==PENN_TAG_ADVERB && !isLinked(&node, rbTarg)){
   				 bool notCond=true;
   				 CStateNodeList* childsRb=rbTarg->m_umbinarizedSubNodes;
   				 while(childsRb!=0){
   					 if (((*words)[childsRb->node->lexical_head].word==g_word_not) ||((*words)[childsRb->node->lexical_head].word==g_word_nt)){
   						 notCond=false;
   					 }
   					 childsRb=childsRb->next;
   				 }
   				 if (notCond){
   					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_ADVMOD);
   					 if (buildStanfordLink(label, rbTarg->lexical_head, node.lexical_head)) {
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


