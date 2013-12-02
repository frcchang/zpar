
//"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < (NP|NML|NN|NNS|NNP|NNPS|FW|AFX=target $++ NN|NNS|NNP|NNPS|FW|CD !<<- POS !<<- (VBZ < /^\'s$/) !$- /^,$/ )",
//bool nn1(){

//}

//"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < JJ|JJR|JJS=sister < (NP|NML|NN|NNS|NNP|NNPS|FW=target !<<- POS !<<- (VBZ < /^\'s$/) $+ =sister) <# NN|NNS|NNP|NNPS !<<- POS !<<- (VBZ < /^\'s$/) ",
bool nn2(){

}

//"ADJP|ADVP < (FW [ $- FW=target | $- (IN=target < in|In) ] )",  // in vitro, in vivo, etc., in Genia
//Divided in 2...


//"ADJP|ADVP < (FW $- FW=target)",
bool nn3(){
	 if (node.constituent==PENN_CON_ADJP || node.constituent==PENN_CON_ADVP){
		 CStateNodeList* childsAd=node.m_umbinarizedSubNodes;
		 while(childsAd!=0){
			 if ((*words)[childsAd->node->lexical_head].tag.code()==PENN_TAG_FW){
				 if (childsAd->previous!=0){
					 const CStateNode* fwTarg=childsAd->previous->node;
					 if (((*words)[fwTarg->lexical_head].tag.code()==PENN_TAG_FW)&&!(isLinked(&node,fwTarg))){
						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
						 if (buildStanfordLink(label, fwTarg->lexical_head, node.lexical_head)) {
							 addLinked(&node,fwTarg);
						     return true;
						 }
					 }
				 }
			 }
			 childsAd=childsAd->next;
		 }
	 }
	 return false;
}


//"ADJP|ADVP < (FW $- (IN=target < in|In) )"
bool nn4(){
	 if (node.constituent==PENN_CON_ADJP || node.constituent==PENN_CON_ADVP){
		 CStateNodeList* childsAd=node.m_umbinarizedSubNodes;
	     while(childsAd!=0){
	    	 if ((*words)[childsAd->node->lexical_head].tag.code()==PENN_TAG_FW){
	    		 if (childsAd->previous!=0){
	    			 const CStateNode* inTarg=childsAd->previous->node;
	    			 	 if (((*words)[inTarg->lexical_head].tag.code()==PENN_TAG_IN)&&!(isLinked(&node,inTarg))){

	    			 		 CStateNodeList* childsIn=inTarg->m_umbinarizedSubNodes;
	    			 		 while(childsIn!=0){
	    			 			if ((*words)[childsIn->node->lexical_head].word==g_word_in||(*words)[childsIn->node->lexical_head].word==g_word_In){
	    			 				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
	    			 				if (buildStanfordLink(label, inTarg->lexical_head, node.lexical_head)) {
	    			 					addLinked(&node,inTarg);
	    			 				    return true;
	    			 				}
	    			 			}
	    			 			childsIn=childsIn->next;
	    			 		 }
	     				 }
	     			}
	     		}
	     		childsAd=childsAd->next;
	     	}
	    }
	 	return false;
}
