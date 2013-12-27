 //"/^(?:PP(?:-TMP)?|(?:WH)?(?:PP|ADVP))$/ < (IN|VBG|TO|FW|RB|RBR $++ (/^(?:WH)?(?:NP|ADJP)(?:-TMP|-ADV)?$/=target !$- @NP))",
    inline const bool &buildPobj1(const unsigned long &cons) {
    	if (cons==PENN_CON_PP || cons==PENN_CON_WHPP || cons==PENN_CON_ADVP || cons==PENN_CON_WHADVP){
    		CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		while(childs!=0){
    			if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_IN
    			    || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG
    			    || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_TO
    			    || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_FW
    			    || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB
    			    || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE){

    				CStateNodeList* rightSisters=childs->next;
    				while(rightSisters!=0){
    					const CStateNode* targ=rightSisters->node;
    					if ((CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHNP ||
    							CConstituent::clearTmp(targ->constituent.code())==PENN_CON_WHADJP || CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADJP) && !isLinked(&node,targ)){
    						bool leftSisterCond=true;
    						if (rightSisters->previous!=0){
    							if (CConstituent::clearTmp(rightSisters->previous->node->constituent.code())==PENN_CON_NP){
    								leftSisterCond=false;
    							}
    						}
    						if (leftSisterCond){
//    							CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POBJ);
    							if (buildStanfordLink(STANFORD_DEP_POBJ, targ->lexical_head, node.lexical_head)) {
    								addLinked(&node,targ);
    							    return true;
    							}
    						}
    					}
    					rightSisters=rightSisters->next;
    				}
    			}
    			childs=childs->next;
    		}
    	}
    	return false;
    }

    //"/^PP(?:-TMP)?$/ < (/^(?:IN|VBG|TO)$/ $+ (ADVP=target [ < (RB < /^(?i:here|there)$/) | < (ADVP < /^NP(?:-TMP)?$/) ] ))",
          // second disjunct is weird ADVP, only matches 1 tree in 2-21
          // to deal with preposition stranding in questions (e.g., "Which city do you live in?") -- the preposition is sometimes treated as a particle by the parser (works well but doesn't preserve the tree structure!)

    inline const bool &buildPobj2(const unsigned long &cons) {
    	if (cons==PENN_CON_PP){
    		CStateNodeList* childsPP=node.m_umbinarizedSubNodes;
    		while(childsPP!=0){
    			if (((*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_IN 
    				|| (*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_TO || (*words)[childsPP->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)){
    			  	if (childsPP->next!=0){
    			  		const CStateNode* targ=childsPP->next->node;
    			  		if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_ADVP && !isLinked(&node,targ)){
    			  			bool cond1=false;
    			  			bool cond2=false;
    			  			CStateNodeList* childsAdvp=targ->m_umbinarizedSubNodes;
    			  			while(childsAdvp!=0){
    			  				if ((*words)[childsAdvp->node->lexical_head].tag.code()==PENN_TAG_ADVERB 
    			  						&& (((*words)[childsAdvp->node->lexical_head].word==g_word_here)||((*words)[childsAdvp->node->lexical_head].word==g_word_there))){
    			  					cond1=true;
    			  				}
    			  				childsAdvp=childsAdvp->next;
    			  			}
    			  			if (!cond1){
    			  				childsAdvp=targ->m_umbinarizedSubNodes;
    			  				while(childsAdvp!=0){
    			  					if (CConstituent::clearTmp(childsAdvp->node->constituent.code())==PENN_CON_ADVP){
    			  						CStateNodeList* childsAdvp2=childsAdvp->node->m_umbinarizedSubNodes;
    			  						while(childsAdvp2!=0){
    			  							if (CConstituent::clearTmp(childsAdvp2->node->constituent.code())==PENN_CON_NP){
    			  								cond2=true;
    			  							}
    			  							childsAdvp2=childsAdvp2->next;
    			  						}
    			  					}
    			  				    childsAdvp=childsAdvp->next;
    			  				}	
    			  			}
    			  			if (cond1 || cond2){
//    			  				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POBJ);
    			  				if (buildStanfordLink(STANFORD_DEP_POBJ, targ->lexical_head, node.lexical_head)) {
    			  					addLinked(&node,targ);
    			  				    return true;
    			  				}
    			  			}
    			  		}
    			  	}
    			}
    			childsPP=childsPP->next;
    		}
    	}
    	return false;
    }


//"PRT >- (VP !< (S < (VP < TO)) >+(SQ|SINV|S|VP) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/))",

inline const bool &buildPobj3(const unsigned long &cons) {}

//"(PP <: IN|TO) >- (VP !< (S < (VP < TO)) >+(SQ|SINV|S|VP) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/))",

inline const bool &buildPobj4(const unsigned long &cons) {}

//"(PP <: IN|TO) $- (NP $-- (VBZ|VBD) !$++ VP) >+(SQ) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/)",
      // to deal with preposition stranding in relative clause (works well but doesn't preserve the tree structure!)

inline const bool &buildPobj5(const unsigned long &cons) {}

//"(PP <- IN|TO) >+(@VP|S|SINV|SBAR) (SBAR !< (WHPP|WHNP) < (S < (NP $+ (VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) NP !< SBAR ))) $-- NP > NP=target)",

inline const bool & buildPobj6(const unsigned long &cons) {}

//"XS|ADVP < (IN < /^(?i:at)$/) < JJS|DT=target", // at least, at most, at best, at worst, at all

inline const bool &buildPobj7(const unsigned long &cons) {
  	 //what is XS????????? I believe we can safely remove it, since it is not included in the tagset. (YUE?)
  	 if (cons==PENN_CON_PP){
  		 bool firstCondition=false;
  		 CStateNodeList* childsPP=node.m_umbinarizedSubNodes;
  		 while(childsPP!=0){
  			//std::cerr<<"Entra al while 1 \n";
  			 const CStateNode* inChildPP=childsPP->node;
  			 if (((*words)[inChildPP->lexical_head].tag.code()==PENN_TAG_IN)) {
  				 //CStateNodeList* childsIn=inChildPP->m_umbinarizedSubNodes;
  				 //while(childsIn!=0){
  					 if (((*words)[inChildPP->lexical_head].word==g_word_at)||((*words)[inChildPP->lexical_head].word==g_word_in)) {
  						 firstCondition=true;
  					 }
  					// childsIn=childsIn->next;
  				 //}
  			 }
  			 childsPP=childsPP->next;
  		 }
  		 if (firstCondition){

  			CStateNodeList* childsPP2=node.m_umbinarizedSubNodes;
  			 while(childsPP2!=0){
  				//std::cerr<<"Entra al while 2 \n";
  				 const CStateNode* jjsdtTarg=childsPP2->node;
  				//std::cerr<<jjsdtTarg->constituent.code());
  				 if ((!isLinked(&node,jjsdtTarg))
  						 && (((*words)[jjsdtTarg->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)
  								 ||((*words)[jjsdtTarg->lexical_head].tag.code()==PENN_TAG_DT))) {
//  					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POBJ);
  					 if (buildStanfordLink(STANFORD_DEP_POBJ, jjsdtTarg->lexical_head, node.lexical_head)) {
  						 addLinked(&node,jjsdtTarg);
  						 return true;
  					 }
  				 }
  				 childsPP2=childsPP2->next;
  			 }
  		 }

  	 }
  	 return false;
   }


