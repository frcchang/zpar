
//"__ < (NFP=target < /^(?:[<>]?[:;=8][\\-o\\*']?(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])|(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])[\\-o\\*']?[:;=8][<>]?)$/)",
bool discourse1(){

}

//"__ < (NFP=target < /^(?:-LRB-)?[\\-\\^x=~<>'][_.]?[\\-\\^x=~<>'](?:-RRB-)?$/)",
bool discourse2(){

}

//"__ [ < INTJ=target | < (PRN=target <1 /^(?:,|-LRB-)$/ <2 INTJ [ !<3 __ | <3 /^(?:,|-RRB-)$/ ] ) ]"
      bool discourse3(){
    	  bool cond1=false;
    	  bool cond2=false;
    	  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	  while(childs!=0){
    		  const CStateNode* targ=childs->node;
    		  if (targ->constituent==PENN_CON_INTJ && !isLinked(&node,targ)){
    			  cond1=true;
    		  }
    		  else if (targ->constituent==PENN_CON_PRN && !isLinked(&node,targ)){
    			  bool child1=false;
    			  bool child2=false;
    			  bool child3=false;
    			  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    			  if (childsT!=0){
    				  if ((*words)[childsT->node->lexical_head].word==g_word_comma || (*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
    					  child1=true;
    				  }
    				  if (child1 && childsT->next!=0){
    					  if (childsT->next->node->constituent==PENN_CON_INTJ){
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
    			  CDependencyLabel* label=new CDependencyLabel(0);
    			  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
    				  addLinked(&node,targ);
    			      return true;
    			  }
    		  }
    		  
    		  childs=childs->next;
    	  }
    	  
    	  /*if (cond1||cond2){
    		  
    	  }*/
      }

