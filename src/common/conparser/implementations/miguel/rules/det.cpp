//"/^(?:NP(?:-TMP|-ADV)?|NAC|NML|NX|X)$/ < (DT=target !< /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:N[MNXP]|CD|JJ|FW|ADJP|QP|RB|PRP(?![$])|PRN)/=det !$++ (/^PRP[$]|POS/ $++ =det !$++ (/''/ $++ =det)))",
bool det1(){

}

//"NP|NP-TMP|NP-ADV < (DT=target < /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:NN|NX|NML)/ !$++ (NP < CC))",
bool det2(){

}


//"NP|NP-TMP|NP-ADV < (DT=target !< /^(?i:either|neither|both)$/ $++ CC $++ /^(?:NN|NX|NML)/)",
bool det3(){
	 if (node.constituent==PENN_CON_NP){
		 CStateNodeList* childsNp=node.m_umbinarizedSubNodes;
		 while(childsNp!=0){
			 const CStateNode* dtTarg=childsNp->node;
			 if ((*words)[dtTarg->lexical_head].tag.code()==PENN_TAG_DT && !(isLinked(&node,dtTarg))){
				 bool wordsCond=true;
				 CStateNodeList* childsDt=dtTarg->m_umbinarizedSubNodes;
				 while(childsDt!=0){
					 if (((*words)[childsDt->node->lexical_head].word==g_word_either) 
							 ||((*words)[childsDt->node->lexical_head].word==g_word_neither)
							 ||((*words)[childsDt->node->lexical_head].word==g_word_both)){
						 wordsCond=false;
					 }
					 childsDt=childsDt->next;
				 }
				 CStateNodeList* rightSisters=childsDt->next;
				 bool ccCond=false;
				 while(rightSisters!=0){
					 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
						 ccCond=true;
					 }
					 rightSisters=rightSisters->next;
				 }
				 if (ccCond){
					 rightSisters=childsDt->next;
					 while(rightSisters!=0){
						 if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN || rightSisters->node->constituent==PENN_CON_NX){
							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
							 if (buildStanfordLink(label, dtTarg->lexical_head, node.lexical_head)) {
								 addLinked(&node,dtTarg);
							 	 return true;
							 }
						 }
						 rightSisters=rightSisters->next;
					 }
				 }
				 
			 }
			 childsNp=childsNp->next;
		 }
		 
	 }
	 return false;
}



//"NP|NP-TMP|NP-ADV < (DT=target $++ (/^JJ/ !$+ /^NN/) !$++ CC !$+ DT)",
bool det4(){

}

//"NP|NP-TMP|NP-ADV <<, PRP <- (NP|DT|RB=target <<- all|both|each)", // we all, them all; various structures
bool det5(){

}

//"WHNP < (NP $-- (WHNP=target < WDT))",
 bool det6(){
	 if (node.constituent==PENN_CON_WHNP){
		 CStateNodeList* childsWhnp=node.m_umbinarizedSubNodes;
		 while(childsWhnp!=0){
			 const CStateNode* npChild=childsWhnp->node;
			 if (npChild->constituent==PENN_CON_NP){
				 CStateNodeList* leftSisters=childsWhnp->previous;
				 while(childsWhnp!=0){
					 const CStateNode* whnpTarg=childsWhnp->node;
					 if (whnpTarg->constituent==PENN_CON_WHNP && !(isLinked(&node,whnpTarg))){
						 CStateNodeList* whnpChilds=whnpTarg->m_umbinarizedSubNodes;
						 while(whnpChilds!=0){
							 if ((*words)[whnpChilds->node->lexical_head].tag.code()==PENN_TAG_WDT){
								 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
								 if (buildStanfordLink(label, whnpTarg->lexical_head, node.lexical_head)) {
									 addLinked(&node,whnpTarg);
								     return true;
								 }
							 }
							 whnpChilds=whnpChilds->next;
						 }
					 }
					 childsWhnp=childsWhnp->previous;
				 }
			 }
			 childsWhnp=childsWhnp->next;
		 }
	 }
	 return false;
 }


 //"@WHNP|ADVP < (/^(?:NP|NN|CD|RBS)/ $-- DT|WDT|WP=target)"
   bool det7(){
  	 if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_ADVP){
  		 CStateNodeList* childsWHAD=node.m_umbinarizedSubNodes;
  		 while(childsWHAD!=0){
  			 if (childsWHAD->node->constituent==PENN_CON_NP||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_NOUN||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_CD||
  					 (*words)[childsWHAD->node->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE){
  				 
  				 CStateNodeList* leftSisters=childsWHAD->previous;
  				 while(leftSisters!=0){
  					 const CStateNode* targ=leftSisters->node;
  					 if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN||
  							 (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN||
  							 (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN) && !isLinked(&node,targ)){
  						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_DET);
  						 if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
  							 addLinked(&node,targ);
  						 	 return true;
  						 }
  					 }
  					 leftSisters=leftSisters->previous;
  				 }
  				 
  			 }
  			 childsWHAD=childsWHAD->next;
  		 }
  	 }
  	 return false;
   }


