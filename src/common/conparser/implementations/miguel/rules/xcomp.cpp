    //"VP < (S=target !$- (NN < order) < (VP < TO))",    // used to have !> (VP < (VB|AUX < be))
    bool buildXComp1() {
    	if (node.constituent==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* sTarg=childsVp->node;
    			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
    				//A $- B: A is the immediate right sister of B
    				bool sisterCondition=false;
    				if (childsVp->previous!=0){
    					const CStateNode* leftSisterS=childsVp->previous->node;
    					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
    						CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
    						while(childsNN!=0){
    							const CStateNode* orderChild=childsNN->node;
    							if ((*words)[orderChild->lexical_head].word==g_word_order){
    								sisterCondition=true;
    							}
    							childsNN=childsNN->next;
    						}
    					}
    				}
    				if (sisterCondition){
    					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
    					while(childsS!=0){
    						const CStateNode* vpChild=childsS->node;
    						if (vpChild->constituent==PENN_CON_VP){
    							CStateNodeList* childsVps=vpChild->m_umbinarizedSubNodes;
    							while(childsVps!=0){
    								if ((*words)[childsVps->node->lexical_head].tag.code()==PENN_TAG_TO) {
    									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
    									if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
    										addLinked(&node,sTarg);
    									    return true;
    									}
    								}
    								childsVps=childsVps->next;
    							}
    						}
    						childsS=childsS->next;
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;

    }

    //"ADJP < (S=target <, (VP <, TO))",
       bool buildXComp2() {
       	if (node.constituent==PENN_CON_ADJP){
       		CStateNodeList* childsADJP=node.m_umbinarizedSubNodes;
       		while(childsADJP!=0){
       			const CStateNode* sTarg=childsADJP->node;
       			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
       				//A <, B 	B is the first child of A
       				CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
       				if (childsS!=0){//only first child
       					const CStateNode* vpChildS=childsS->node;
       					if (vpChildS->constituent==PENN_CON_VP){
       						CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
       						if (childsVp!=0){//only first child
       							if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)){
       								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
       								if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
       									addLinked(&node,sTarg);
       									return true;
       								}
       							}
       						}
       					}
       				}

       			}
       			childsADJP=childsADJP->next;
       		}
       	}
       	return false;

       }

//"VP < (S=target !$- (NN < order) < (NP $+ NP|ADJP))",
bool buildXComp3() {
	if (node.constituent==PENN_CON_VP){
		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
		while(childsVp!=0){
			const CStateNode* sTarg=childsVp->node;
			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
				//A $- B: A is the immediate right sister of B
				bool sisterCondition=false;
				if (childsVp->previous!=0){
					const CStateNode* leftSisterS=childsVp->previous->node;
					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
						CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
						while(childsNN!=0){
							const CStateNode* orderChild=childsNN->node;
							if ((*words)[orderChild->lexical_head].word==g_word_order){
								sisterCondition=true;
							}
							childsNN=childsNN->next;
						}
					}
				}
				if (sisterCondition){
					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
					while(childsS!=0){
						const CStateNode* npChildS=childsS->node;
						if (npChildS->constituent==PENN_CON_NP){
							//A $+ B 	A is the immediate left sister of B
							if (childsS->next!=0){
								if (childsS->next->node->constituent==PENN_CON_NP ||childsS->next->node->constituent==PENN_CON_ADJP){
									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
									if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
										addLinked(&node,sTarg);
									    return true;
									}
								}
							}
						}
						childsS=childsS->next;
					}
				}
			}
			childsVp=childsVp->next;
		}
	}
	return false;

}

//"VP < (/^(?:VB|AUX)/ $+ (VP=target < VB < NP))",
   bool buildXComp4() {
   	if (node.constituent==PENN_CON_VP){
   		bool firstCondition=false;
   		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
   		while(childsVp!=0){
   			if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB)){
   				firstCondition=true;
   			}
   			childsVp=childsVp->next;
   		}
   		if (firstCondition){
   			//A $+ B 	A is the immediate left sister of B
   			if (childsVp->next!=0){
   				const CStateNode* vpTarg=childsVp->next->node;
   				if ((vpTarg->constituent==PENN_CON_VP) && (!isLinked(&node,vpTarg))){
   					CStateNodeList* childsVpTarg=vpTarg->m_umbinarizedSubNodes;
   					while(childsVpTarg!=0){
   						const CStateNode* vbChildVp=childsVpTarg->node;
   						if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)){
   							CStateNodeList* childsVb=vbChildVp->m_umbinarizedSubNodes;
   							while(childsVb!=0){
   								if (childsVb->node->constituent==PENN_CON_NP){
   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
   									if (buildStanfordLink(label, vpTarg->lexical_head, node.lexical_head)) {
   										addLinked(&node,vpTarg);
   										return true;
   									}
   								}
   								childsVb=childsVb->next;
   							}
   						}
   						childsVpTarg=childsVpTarg->next;
   					}
   				}

   			}
   		}
   	}
   	return false;

   }


//"VP < (SBAR=target < (S !$- (NN < order) < (VP < TO))) !> (VP < (VB|AUX < be)) ",
bool buildXComp5() {}

//"VP < (S=target !$- (NN < order) <: NP) > VP",
bool buildXComp6() {}

//"(VP < (S=target < (VP < VBG ) !< NP !$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)]) !$-- /^:$/))",
bool buildXComp7() {}
