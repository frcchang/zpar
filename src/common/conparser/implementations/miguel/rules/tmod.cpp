
//"VP|ADJP < NP-TMP=target",
bool tmod1(){
//we can safely ignore it...NP-TMP is not in our data set.
}

//"VP|ADJP < (NP=target <# (/^NN/ < " + timeWordRegex + ") !$+ (/^JJ/ < old))",
    bool tmod2(){
  	  if (node.constituent==PENN_CON_VP || node.constituent==PENN_CON_ADJP){
  		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
  		  while(childs!=0){
  			  const CStateNode* targ=childs->node;
  			  if (targ->constituent==PENN_CON_NP && !isLinked(&node,targ)){
  				  bool cond1=false;
  				  bool cond2=true;
  				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
  				  while(childsT!=0){
  					  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
  					  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
  							  || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL){
  						  CStateNodeList* childsNN=childsT->node->m_umbinarizedSubNodes;
  						  while(childsNN!=0){
  							  if (compareWordToTimeWordRegex((*words)[childsNN->node->lexical_head].word)) {
  								  cond1=true;
  							  }
  							  childsNN=childsNN->next;
  						  }
  					  }
  					  if (childsT->next!=0){
  						  //PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
  						  if ((*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE || (*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE
  						      || (*words)[childsT->next->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE){

  							  CStateNodeList* childsJJ=childsT->next->node->m_umbinarizedSubNodes;
  							  while(childsJJ!=0){
  								  if ((*words)[childsJJ->node->lexical_head].word==g_word_old){
  									  cond2=false;
  								  }
  								  childsJJ=childsJJ->next;
  							  }


  						  }
  					  }
  					  childsT=childsT->next;
  				  }

  				  if (cond1 && cond2){
  					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_TMOD);
  					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
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



//"@PP < (IN|TO|VBG|FW $++ (@NP $+ NP-TMP=target))",
bool tmod3(){
//we can safely ignore it, NP-TMP is not in our data set.
}

//"@PP < (IN|TO|VBG|FW $++ (@NP $+ (NP=target <# (/^NN/ < " + timeWordRegex + "))))",
bool tmod4(){

}


//"S < (NP-TMP=target $++ VP [ $++ NP | $-- NP] )",
bool tmod5(){
//we can safely ignore it....
}

//"S < (NP=target <# (/^NN/ < " + timeWordRegex + ") $++ (NP $++ VP))",
bool tmod6(){

}

//"SBAR < (@WHADVP < (WRB < when)) < (S < (NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) ))) !$-- CC $-- NP > NP=target",
bool tmod7(){

}

//"SBARQ < (@WHNP=target <# (/^NN/ < " + timeWordRegex + ")) < (SQ < @NP)",
bool tmod8(){

}

//"NP < NP-TMP=target"
bool tmod9(){
//we can safely ignore it...
}
