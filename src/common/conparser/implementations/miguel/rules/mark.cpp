
//"SBAR|SBAR-TMP < (IN|DT=target $++ S|FRAG)",
     bool mark1(){
    	 if (node.constituent==PENN_CON_SBAR){
    		 CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
    		 while(childsSbar!=0){
    			 const CStateNode* indtTarg=childsSbar->node;
    			 if (((*words)[indtTarg->lexical_head].tag.code()==PENN_TAG_IN ||
    			     (*words)[indtTarg->lexical_head].tag.code()==PENN_TAG_DT) && !isLinked(&node,indtTarg)){
    				 CStateNodeList* rightSisters=childsSbar;
    				 while(rightSisters!=0){
    					 if (rightSisters->node->constituent==PENN_CON_S ||
    							 rightSisters->node->constituent==PENN_CON_FRAG) {

    						 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_MARK);
    						 if (buildStanfordLink(label, indtTarg->lexical_head, node.lexical_head)) {
    							 addLinked(&node,indtTarg);
    						     return true;
    						 }
    					 }
    					 rightSisters=rightSisters->next;
    				 }
    			 }
    			 childsSbar=childsSbar->next;
    		 }
    	 }
    	 return false;
     }

//"SBAR < (IN|DT=target < that|whether) [ $-- /^(?:VB|AUX)/ | $- NP|NN|NNS | > ADJP|PP | > (@NP|UCP|SBAR < CC|CONJP $-- /^(?:VB|AUX)/) ]",
bool mark2(){

}
