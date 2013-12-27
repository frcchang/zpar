
//"__ < (NFP=target < /^(?:[<>]?[:;=8][\\-o\\*']?(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])|(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])[\\-o\\*']?[:;=8][<>]?)$/)",
inline const bool &discourse1(){
	//I think we can safely ignore it.
}

//"__ < (NFP=target < /^(?:-LRB-)?[\\-\\^x=~<>'][_.]?[\\-\\^x=~<>'](?:-RRB-)?$/)",
inline const bool &discourse2(){
	//I think we can safely ignore it.
}

//"__ [ < INTJ=target | < (PRN=target <1 /^(?:,|-LRB-)$/ <2 INTJ [ !<3 __ | <3 /^(?:,|-RRB-)$/ ] ) ]"
inline const bool & discourse3(const unsigned long &cons){
    	  bool cond1=false;
    	  bool cond2=false;
    	  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	  while(childs!=0){
    		  const CStateNode* targ=childs->node;
    		  if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_INTJ && !isLinked(&node,targ)){
    			  cond1=true;
    		  }
    		  else if (CConstituent::clearTmp(targ->constituent.code())==PENN_CON_PRN && !isLinked(&node,targ)){
    			  bool child1=false;
    			  bool child2=false;
    			  bool child3=false;
    			  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    			  if (childsT!=0){
    				  if ((*words)[childsT->node->lexical_head].word==g_word_comma || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    					  child1=true;
    				  }
    				  if (child1 && childsT->next!=0){
    					  if (CConstituent::clearTmp(childsT->next->node->constituent.code())==PENN_CON_INTJ){
    						  child2=true;
    					  }
    					  if (child2 && childsT->next->next!=0){
    						  if ((*words)[childsT->next->next->node->lexical_head].word==g_word_comma || (*words)[childsT->next->next->node->lexical_head].tag.code()==PENN_TAG_R_BRACKET){
    							  child3=true;
    						  }
    					  }
    				  }
    			  }
    			  cond2=child1 && child2 && child3;  
    		  }
    		  
    		  if (cond1||cond2){
//    			  CDependencyLabel* label=new CDependencyLabel(0);
    			  if (buildStanfordLink(0, targ->lexical_head, node.lexical_head)) {
    				  addLinked(&node,targ);
    			      return true;
    			  }
    		  }
    		  
    		  childs=childs->next;
    	  }
   
      }

