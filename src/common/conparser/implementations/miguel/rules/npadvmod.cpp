

//"@ADJP < (NN=target $++ /^JJ/) !< CC|CONJP",
bool npadvmod2(){
	 if (node.constituent==PENN_CON_ADJP){
		 CStateNodeList* childsAdjp=node.m_umbinarizedSubNodes;
		 bool firstCondition=true;
		 
		 while(childsAdjp!=0){
			 if (((*words)[childsAdjp->node->lexical_head].tag.code()==PENN_TAG_CC) || childsAdjp->node->constituent==PENN_CON_CONJP) {
				 firstCondition=false;
			 }
			 childsAdjp=childsAdjp->next;
		 }
		 if (firstCondition){
			 childsAdjp=node.m_umbinarizedSubNodes;
			 while(childsAdjp!=0){
				 const CStateNode* nnTarg=childsAdjp->node;
				 if (((*words)[nnTarg->lexical_head].tag.code()==PENN_TAG_NOUN)&& !(isLinked(&node,nnTarg))) {
					 CStateNodeList* rightSistersNN=childsAdjp;
					 while(rightSistersNN!=0){
						 //PENN_TAG_ADJECTIVE, PENN_TAG_ADJECTIVE_COMPARATIVE, PENN_TAG_ADJECTIVE_SUPERLATIVE
						 if (((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)||
								 ((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE)||
								 ((*words)[rightSistersNN->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)) {
							 CDependencyLabel* label=new CDependencyLabel(0);
							 if (buildStanfordLink(label, nnTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,nnTarg);
							 	return true;
							 }
							 
						 }
						 rightSistersNN=rightSistersNN->next;
					 }
				 }
				 childsAdjp=childsAdjp->next;
			 }
		 }
		 
	 }
	 return false;
}


//"@NP|WHNP < /^NP-ADV/=target",
bool npadvmod3(){
	  //I think we can safely ignore it...
	//......

		////
}


//"@NP <1 (@NP <<# /^%$/) <2 (@NP=target <<# days|month|months) !<3 __",
     bool npadvmod4(){
   	  if (node.constituent==PENN_CON_NP){

   		  CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
   		  if (childsNp!=0){
   			  bool firstCond=false;
   			  if (childsNp->node->constituent==PENN_CON_NP){
   				  //
   				  CStateNodeList* desc=new CStateNodeList();
   				  listDescendants (childsNp->node->m_umbinarizedSubNodes, desc);
   				  while(desc!=0){
   					  if (((*words)[desc->node->lexical_head].word==g_word_perc) && (childsNp->node->lexical_head==desc->node->lexical_head)){
   						  firstCond=true;
   					  }
   					  desc=desc->next;
   				  }
   			  }
   			  childsNp=childsNp->next;
   			  if (childsNp!=0 && firstCond){
   				  bool secCond=false;
   				  const CStateNode* targ=childsNp->node;
   				  if (targ->constituent==PENN_CON_NP && !isLinked(&node,targ)){
   					  CStateNodeList* desc2=new CStateNodeList();
   					  listDescendants (targ->m_umbinarizedSubNodes, desc2);
   					  while(desc2!=0){
   						  if (((*words)[desc2->node->lexical_head].word==g_word_days || (*words)[desc2->node->lexical_head].word==g_word_month || (*words)[desc2->node->lexical_head].word==g_word_months)
   								  && (targ->lexical_head==desc2->node->lexical_head)){
   							  secCond=true;
   					      }
   						  desc2=desc2->next;
   					  }
   				  }

   				  childsNp=childsNp->next;
   				  if (childsNp==0 && secCond){ //there is no 3rd child
   					  CDependencyLabel* label=new CDependencyLabel(0);
   					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
   						  addLinked(&node,targ);
   						  return true;
   					  }
   				  }
   			  }
   		  }
   	  }
   	  return false;
     }


//"@VP < /^NP-ADV/=target",
bool npadvmod5(){
	 //I think we can safely ignore it...

	//......

	////

}
