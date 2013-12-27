      
      //"/^(?:(?:WH)?NP(?:-TMP|-ADV)?|NX|NAC|NML)$/ < (PDT|DT=target $+ /^(?:DT|WP\\$|PRP\\$)$/ $++ /^N[NXM]/ !$++ CC)",
    inline const bool &predet1(const unsigned long &cons){
    	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP || cons==PENN_CON_NAC || cons==PENN_CON_NX){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if ((((*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT)
    					  ||((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT))
    					&& !isLinked(&node,targ)){
    				  
    				  bool firstCond=false;
    				  bool secCond=false;
    				  bool thirdCond=true;
    				  CStateNodeList* rightSisters=childs->next;
    				  if (rightSisters!=0){
    					  if (((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PDT)
    						  ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_WP)
    						  ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_WP_DOLLAR)
    						  ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP_DOLLAR)
    					      ||((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_PRP)) {
    						  firstCond=true;
    					  }
    				  }
    				  while(rightSisters!=0){
    					  if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN || CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NX){
    						  secCond=true;
    					  }
    					  if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    						  thirdCond=false;
    					  }
    					  rightSisters=rightSisters->next;
    				  }
    				  
    				  
    				  if (firstCond && secCond && thirdCond){
//    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREDET);
    					  if (buildStanfordLink(STANFORD_DEP_PREDET, targ->lexical_head, node.lexical_head)) {
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



//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (PDT|DT=target $+ DT $++ (/^JJ/ !$+ /^NN/)) !$++ CC",
    inline const bool &predet2(const unsigned long &cons){
  	  CStateNodeList* childsN=node.m_umbinarizedSubNodes;
  	  while(childsN!=0){
  		  const CStateNode* head=childsN->node;
  		  if (CConstituent::clearTmp(head->constituent.code())==PENN_CON_WHNP || CConstituent::clearTmp(head->constituent.code())==PENN_CON_NP){
  			  bool rightSisCond=true;
  			  CStateNodeList* rightSisters=childsN->next;
  			  while(rightSisters!=0){
  				  if (((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC)){
  					  rightSisCond=false;
  				  }
  				  rightSisters=rightSisters->next;
  			  }
  			  if (rightSisCond){
  				  CStateNodeList* childs=head->m_umbinarizedSubNodes;
  				  while(childs!=0){
  					  const CStateNode* targ=childs->node;
  					  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_DT)
  							  ||((*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT) && !isLinked(&node,targ)){
  						  CStateNodeList* rightSis=childs->next;
  						  bool firstCond=false;
  						  bool secCond=false;
  						  if (rightSis!=0){
  							  if (((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE)
  								  ||((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE)
  							      ||((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE)){
  							  firstCond=true;
  						  }
  						  if (firstCond){
  							  while(rightSis!=0){
  								  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
  								  if (((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_NOUN)
  								      ||((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER)
  								      ||((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)
  								      ||((*words)[rightSis->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)){
  									  secCond=true;
  								  }
  								  rightSis=rightSis->next;
  							  }
  							  if (firstCond && secCond){
//  								  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREDET);
  								  if (buildStanfordLink(STANFORD_DEP_PREDET, targ->lexical_head, head->lexical_head)) {
  									  addLinked(&node,targ);
  								      return true;
  								  }
  							  }
  						  }
  					  }
  						  }
  					  childs=childs->next;
  				  }
  			  }
  		  }
  		  childsN=childsN->next;
  	  }
  	  return false;
    }


//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < PDT=target <- DT"
    inline const bool &predet3(const unsigned long &cons){
   	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP){
   		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
   		  bool lastCond=false;
   		  while(childs!=0){
   			  if (((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_DT) && childs->next==0){
   				  lastCond=true;
   			  }
   			  childs=childs->next;
   		  }
   		  if (lastCond){
   			  childs=node.m_umbinarizedSubNodes;
   			  while(childs!=0){
   				  const CStateNode* targ=childs->node;
   				  if (((*words)[targ->lexical_head].tag.code()==PENN_TAG_PDT) && !isLinked(&node,targ)){
//   					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PREDET);
   					  if (buildStanfordLink(STANFORD_DEP_PREDET, targ->lexical_head, node.lexical_head)) {
   						  addLinked(&node,targ);
   					      return true;
   					  }
   				  }
   				  childs=childs->next;
   			  }
   		  }
   	  }	
   	  return false;
     }

