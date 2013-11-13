
//"/^(?:NP(?:-TMP|-ADV)?|NX|NML|NAC|WHNP)$/ < (ADJP|WHADJP|JJ|JJR|JJS|JJP|VBN|VBG|VBD|IN=target !< QP !$- CC)",
bool amod1(){

}


//"ADJP !< CC|CONJP < (JJ|NNP $ JJ|NNP=target)",
bool amod2(){
	 if (node.constituent==PENN_CON_ADJP){
		 bool firstCondition=true;
		 CStateNodeList* childsADJP=node.m_umbinarizedSubNodes;
		 while(childsADJP!=0){
			 if (childsADJP->node->constituent==PENN_CON_CONJP || (*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_CC){
				 firstCondition=false;
			 }
			 childsADJP=childsADJP->next;
		 }

		 if (firstCondition){
			 childsADJP=node.m_umbinarizedSubNodes;
			 while(childsADJP!=0){
				 if ((*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE ||(*words)[childsADJP->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER){
					 CStateNodeList* rightSisters=childsADJP->next;
					 CStateNodeList* leftSisters=childsADJP->previous;
					 while(rightSisters!=0){
						 const CStateNode* jjnnpTarg=rightSisters->node;
						 if (((*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_ADJECTIVE
								 ||(*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER) && !(isLinked(&node,jjnnpTarg))){
							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
							 if (buildStanfordLink(label, jjnnpTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,jjnnpTarg);
							     return true;
							 }
						 }
						 rightSisters=rightSisters->next;
					 }
					 while(leftSisters!=0){
						 const CStateNode* jjnnpTarg=leftSisters->node;
						 if (((*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_ADJECTIVE
								 ||(*words)[jjnnpTarg->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER) && !(isLinked(&node,jjnnpTarg))){
						     CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AMOD);
						     if (buildStanfordLink(label, jjnnpTarg->lexical_head, node.lexical_head)) {
						    	 addLinked(&node,jjnnpTarg);
						     	 return true;
						     }
						  }
						 leftSisters=leftSisters->previous;
					 }
				 }
				 childsADJP=childsADJP->next;
			 }
		 }
	 }
	 return false;
}


//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (NP=target <: CD $- /^,$/ $-- /^(?:WH)?NP/ !$ CC|CONJP)",
bool amod3(){

}
