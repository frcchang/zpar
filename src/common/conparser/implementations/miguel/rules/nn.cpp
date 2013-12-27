 //"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < (NP|NML|NN|NNS|NNP|NNPS|FW|AFX=target $++ NN|NNS|NNP|NNPS|FW|CD !<<- POS !<<- (VBZ < /^\'s$/) !$- /^,$/ )",
inline const bool &nn1(const unsigned long &cons){
    	  //std::cout<<"nn1 \n";
    	  //std::cout<<(*words)[node.lexical_head].word<<"\n";
    	  //std::cout<<cons<<"\n";
    	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP ||cons==PENN_CON_NAC || cons==PENN_CON_NX){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  //std::cout<<"After the constittuent test"<<(*words)[node.lexical_head].word<<"\n";

			  //std::cout<<(*words)[targ->lexical_head].word<<"\n";
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
    			  if ((CConstituent::clearTmp(targ->constituent.code())==PENN_CON_NP
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL
    					  || (*words)[targ->lexical_head].tag.code()==PENN_TAG_FW ) && !isLinked(&node,targ)){

    				  //std::cout<<"TARGET"<<(*words)[targ->lexical_head].word<<"\n";
    				  bool rightsistCond=false;
    				  bool descCond1=true;
    				  bool descCond2=true;
    				  bool leftSistCond=true;
    				  //(*words)[targ->lexical_head].tag.code()==PENN_TAG_CD
    				  CStateNodeList* rightSisters=childs->next;
    				  while(rightSisters!=0){
    					  //std::cout<<"RIGHTSIS"<<(*words)[rightSisters->node->lexical_head].word<<"\n";
    					  if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN
    							|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CD
    					      	|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
    					      	|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL
    					      	|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL
    					      	|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_FW ){
    						  rightsistCond=true;
    					  }
    					  rightSisters=rightSisters->next;
    				  }
    				  if (rightsistCond){
    					  CStateNodeList* leftSisters=childs->previous;
    					  if (leftSisters!=0){
    						  if ((*words)[leftSisters->node->lexical_head].word==g_word_comma){
    							  leftSistCond=false;
    						  }
    					  }
    					  if (leftSistCond){
    						  CStateNodeList* descendants=new CStateNodeList();
    						  listRightMostDescendants(targ->m_umbinarizedSubNodes,descendants);
    						  if (descendants->node==0) {
    							  descendants->clear();
    						      descendants=0;
    						  }
    						  while(descendants!=0){
    							  if ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_POS){
    								  descCond1=false;
    							  }
    							  else if ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
    								  CStateNodeList* childsVbz=descendants->node->m_umbinarizedSubNodes;
    								  //while(childsVbz!=0){
    									  if ((*words)[descendants->node->lexical_head].word==g_word_aps){
    										  descCond2=false;
    									  }
    									  //childsVbz=childsVbz->next;
    								  //}
    							  }
    							  descendants=descendants->next;
    						  }

    					  }
    					  if (rightsistCond && descCond1 && descCond2 && leftSistCond){

//    						  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
    					      if (buildStanfordLink(STANFORD_DEP_NN, targ->lexical_head, node.lexical_head)) {
    					      	  addLinked(&node,targ);
    					      	  return true;
    					      }
    					   }
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
}

      //"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < JJ|JJR|JJS=sister < (NP|NML|NN|NNS|NNP|NNPS|FW=target !<<- POS !<<- (VBZ < /^\'s$/) $+ =sister) <# NN|NNS|NNP|NNPS !<<- POS !<<- (VBZ < /^\'s$/) ",
inline const bool &nn2(const unsigned long &cons){
         	  if (cons==PENN_CON_WHNP || cons==PENN_CON_NP || cons==PENN_CON_NAC || cons==PENN_CON_NX){
          		  bool firstCond=false; //< JJ|JJR|JJS=sister
          		  bool thirdCond=false; //<# NN|NNS|NNP|NNPS
          		  bool fourthCond=true;//!<<- POS
          		  bool fifthCond=true; //!<<- (VBZ < /^\'s$/)

          		  const CStateNode* sister=0;
          		  const CStateNodeList* childs=node.m_umbinarizedSubNodes;
          		  while(childs!=0){
          			  if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE){
          				  firstCond=true;
          				  sister=childs->node;
          			  }
          			  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
          			  else if (((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
          					  || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL) && childs->node->lexical_head==node.lexical_head){
          				  thirdCond=true;
          			  }
          			  childs=childs->next;
          		  }

          		  if (firstCond && thirdCond){
          			  CStateNodeList* descendants=new CStateNodeList();
          			  listRightMostDescendants(node.m_umbinarizedSubNodes,descendants);
          			  if (descendants->node==0) {
          				descendants->clear();
          			   	descendants=0;
          			  }
          			  while(descendants!=0){
          				  if ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_POS){
          					  fourthCond=false;
          				  }
          				  else if ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
          					  //CStateNodeList* childsVbz=descendants->node->m_umbinarizedSubNodes;
          				      //while(childsVbz!=0){
          				    	  if ((*words)[descendants->node->lexical_head].word==g_word_aps){
          				    		  fifthCond=false;
          				    	  }
          				      	//  childsVbz=childsVbz->next;
          				      //}
          				  }
          				  descendants=descendants->next;
          			  }
          		  }

          		  if (firstCond && thirdCond && fourthCond && fifthCond){
          			  //< (NP|NML|NN|NNS|NNP|NNPS|FW=target !<<- POS !<<- (VBZ < /^\'s$/) $+ =sister)
          			  childs=node.m_umbinarizedSubNodes;
          			  while(childs!=0){
          				  const CStateNode* targ=childs->node;
          				  if (((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER
          						  || CConstituent::clearTmp(childs->node->constituent.code())==PENN_CON_NP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL
          						  || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_FW || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL) && !isLinked(&node,targ)){

          					  CStateNodeList* descendants2=new CStateNodeList();
          					  listRightMostDescendants(targ->m_umbinarizedSubNodes,descendants2);
          					  if (descendants2->node==0) {
          						  descendants2->clear();
          					      descendants2=0;
          					  }
          					  while(descendants2!=0){
          						  if ((*words)[descendants2->node->lexical_head].tag.code()==PENN_TAG_POS){
          							  fourthCond=false;
          					      }
          					      else if ((*words)[descendants2->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
          					    	  CStateNodeList* childsVbz=descendants2->node->m_umbinarizedSubNodes;
          					      	  //while(childsVbz!=0){
          					      		  if ((*words)[descendants2->node->lexical_head].word==g_word_aps){
          					      			CStateNodeList* rightSisters=childs->next;
          					      			if (rightSisters!=0 && sister!=0){
          					      				if ((*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE
          					      						|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_COMPARATIVE
          					      						|| (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_ADJECTIVE_SUPERLATIVE){
//          					      					CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
          					      					if (buildStanfordLink(STANFORD_DEP_NN, targ->lexical_head, node.lexical_head)) {
          					      						addLinked(&node,targ);
          					      					    return true;
          					      					}
          					      				}
          					      			}
          					      		  }
          					      		//  childsVbz=childsVbz->next;
          					      	  //}
          					       }
          					      descendants2=descendants2->next;
          					    }

          				  }
          				  childs=childs->next;
          			  }
          		  }
          	  }
          	  return false;
}


//"ADJP|ADVP < (FW [ $- FW=target | $- (IN=target < in|In) ] )",  // in vitro, in vivo, etc., in Genia
//Divided in 2...


//"ADJP|ADVP < (FW $- FW=target)",
inline const bool &nn3(const unsigned long &cons){
	 if (cons==PENN_CON_ADJP || cons==PENN_CON_ADVP){
		 CStateNodeList* childsAd=node.m_umbinarizedSubNodes;
		 while(childsAd!=0){
			 if ((*words)[childsAd->node->lexical_head].tag.code()==PENN_TAG_FW){
				 if (childsAd->previous!=0){
					 const CStateNode* fwTarg=childsAd->previous->node;
					 if (((*words)[fwTarg->lexical_head].tag.code()==PENN_TAG_FW)&&!(isLinked(&node,fwTarg))){
//						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
						 if (buildStanfordLink(STANFORD_DEP_NN, fwTarg->lexical_head, node.lexical_head)) {
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
inline const bool &nn4(const unsigned long &cons){
	 if (cons==PENN_CON_ADJP || cons==PENN_CON_ADVP){
		 CStateNodeList* childsAd=node.m_umbinarizedSubNodes;
	     while(childsAd!=0){
	    	 if ((*words)[childsAd->node->lexical_head].tag.code()==PENN_TAG_FW){
	    		 if (childsAd->previous!=0){
	    			 const CStateNode* inTarg=childsAd->previous->node;
	    			 	 if (((*words)[inTarg->lexical_head].tag.code()==PENN_TAG_IN)&&!(isLinked(&node,inTarg))){

	    			 		 CStateNodeList* childsIn=inTarg->m_umbinarizedSubNodes;
	    			 		 //while(childsIn!=0){
	    			 			if ((*words)[inTarg->lexical_head].word==g_word_in||(*words)[inTarg->lexical_head].word==g_word_In){
//	    			 				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NN);
	    			 				if (buildStanfordLink(STANFORD_DEP_NN, inTarg->lexical_head, node.lexical_head)) {
	    			 					addLinked(&node,inTarg);
	    			 				    return true;
	    			 				}
	    			 			}
	    			 			//childsIn=childsIn->next;
	    			 		 //}
	     				 }
	     			}
	     		}
	     		childsAd=childsAd->next;
	     	}
	    }
	 	return false;
}
