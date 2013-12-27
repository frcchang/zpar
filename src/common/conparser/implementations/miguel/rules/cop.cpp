  //"VP < (/^(?:VB|AUX)/=target < " + copularWordRegex + " [ $++ (/^(?:ADJP|NP$|WHNP$)/ !< VBN|VBD) | $++ (S <: (ADJP < JJ)) ] )",
      bool cop1(const unsigned long &cons){
    	  if (cons==PENN_CON_VP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB && !isLinked(&node,targ)) {
    				  //CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    				  //while(childsT!=0){
    					  if (compareWordToCopularWordRegex((*words)[targ->lexical_head].word)) {
    						  CStateNodeList* rightSisters=childs->next;
    						  bool cond1=false;
    						  bool cond2=false;
    						  while(rightSisters!=0){
    							  if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_ADJP
    									  || CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP
		  	  	  	  	  	  	  	  	  || CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_WHNP){ //??
    								  bool intCond=true;
    								  CStateNodeList* childsANW=rightSisters->node->m_umbinarizedSubNodes;
    								  while(childsANW!=0){
    									  if ((*words)[childsANW->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST || (*words)[childsANW->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){
    										  intCond=false;
    									  }
    									  childsANW=childsANW->next;
    								  }
    								  if (intCond){
    									  cond1=true;
    								  }
    							  }
    							  else if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_S && !cond1){
    								  CStateNodeList* childsS=rightSisters->node->m_umbinarizedSubNodes;
    								  if (childsS!=0){
    									  if (childsS->next==0 && CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_ADJP){
    										  CStateNodeList* childsA=childsS->node->m_umbinarizedSubNodes;
    										  while(childsA!=0){
    											  if ((*words)[childsA->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST){
    												  cond2=true;
    											  }
    											  childsA=childsA->next;
    										  }
    									  }
    								  }
    							  }

    							  rightSisters=rightSisters->next;
    						  }
    						  if (cond1||cond2){
//    							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_COP);
    							  if (buildStanfordLink(STANFORD_DEP_COP, targ->lexical_head, node.lexical_head)) {
    								  addLinked(&node,targ);
    								  return true;
    							  }
    						  }
    					  }
    					  //childsT=childsT->next;
    				  //}

    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }


      //"SQ|SINV < (/^(?:VB|AUX)/=target < " + copularWordRegex + " [ $++ (ADJP !< VBN|VBD) | $++ (NP $++ NP) | $++ (S <: (ADJP < JJ)) ] )",
           bool cop2(const unsigned long &cons){
         	  if (cons==PENN_CON_SQ||cons==PENN_CON_SINV){
         	      		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
         	      		  while(childs!=0){
         	      			  const CStateNode* targ=childs->node;
         	      			  if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB && !isLinked(&node,targ)) {
         	      				  //CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
         	      				  //while(childsT!=0){
         	      					  if (compareWordToCopularWordRegex((*words)[targ->lexical_head].word)) {
         	      						  CStateNodeList* rightSisters=childs->next;
         	      						  bool cond1=false;
         	      						  bool cond2=false;
         	      						  bool cond3=false;
         	      						  while(rightSisters!=0){
         	      							  if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_ADJP ){
         	      								  bool intCond=true;
         	      								  CStateNodeList* childsANW=rightSisters->node->m_umbinarizedSubNodes;
         	      								  while(childsANW!=0){
         	      									  if ((*words)[childsANW->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST || (*words)[childsANW->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE){
         	      										  intCond=false;
         	      									  }
         	      									  childsANW=childsANW->next;
         	      								  }
         	      								  if (intCond){
         	      									  cond1=true;
         	      								  }
         	      							  }
         	      							  else if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_S && !cond1){
         	      								  CStateNodeList* childsS=rightSisters->node->m_umbinarizedSubNodes;
         	      								  if (childsS!=0){
         	      									  if (childsS->next==0 && CConstituent::clearTmp(childsS->node->constituent.code())==PENN_CON_ADJP){
         	      										  CStateNodeList* childsA=childsS->node->m_umbinarizedSubNodes;
         	      										  while(childsA!=0){
         	      											  if ((*words)[childsA->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST){
         	      												  cond3=true;
         	      											  }
         	      											  childsA=childsA->next;
         	      										  }
         	      									  }
         	      								  }
         	      							  }
         	      							  else if (CConstituent::clearTmp(rightSisters->node->constituent.code())==PENN_CON_NP && !cond1 && !cond3){
         	      								  if (rightSisters->next!=0){
         	      									  CStateNodeList* rightSisters2=rightSisters->next;
         	      									  while(rightSisters2!=0){
         	      										  if (CConstituent::clearTmp(rightSisters2->node->constituent.code())==PENN_CON_NP){
         	      											  cond2=true;
         	      										  }
         	      										  rightSisters2=rightSisters2->next;
         	      									  }
         	      								  }
         	      							  }

         	      							  rightSisters=rightSisters->next;
         	      						  }
         	      						  if (cond1||cond2||cond3){
//         	      							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_COP);
         	      							  if (buildStanfordLink(STANFORD_DEP_COP, targ->lexical_head, node.lexical_head)) {
         	      								  addLinked(&node,targ);
         	      								  return true;
         	      							  }
         	      						  }
         	      					  }
         	      					  //childsT=childsT->next;
         	      				  //}

         	      			  }
         	      			  childs=childs->next;
         	      		  }
         	      	  }
         	      	  return false;
           }
