//"/^(?:PP(?:-TMP)?|(?:WH)?(?:PP|ADVP))$/ < (IN|VBG|TO|FW|RB|RBR $++ (/^(?:WH)?(?:NP|ADJP)(?:-TMP|-ADV)?$/=target !$- @NP))",
      // We allow ADVP with NP objects for cases like (ADVP earlier this year)

inline const bool &buildPobj1(const unsigned long &cons) {}

//"/^PP(?:-TMP)?$/ < (/^(?:IN|VBG|TO)$/ $+ (ADVP=target [ < (RB < /^(?i:here|there)$/) | < (ADVP < /^NP(?:-TMP)?$/) ] ))",
      // second disjunct is weird ADVP, only matches 1 tree in 2-21
      // to deal with preposition stranding in questions (e.g., "Which city do you live in?") -- the preposition is sometimes treated as a particle by the parser (works well but doesn't preserve the tree structure!)

inline const bool &buildPobj2(const unsigned long &cons) {}

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
  				 CStateNodeList* childsIn=inChildPP->m_umbinarizedSubNodes;
  				 while(childsIn!=0){
  					 if (((*words)[childsIn->node->lexical_head].word==g_word_at)||((*words)[childsIn->node->lexical_head].word==g_word_in)) {
  						 firstCondition=true;
  					 }
  					 childsIn=childsIn->next;
  				 }
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
  					 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_POBJ);
  					 if (buildStanfordLink(label, jjsdtTarg->lexical_head, node.lexical_head)) {
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


