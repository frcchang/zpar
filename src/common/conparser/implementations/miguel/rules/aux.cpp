      
     //===============================================================================
      
      //aux rules: there are 4
      
      //"VP < VP < /^(?:TO|MD|VB.*|AUXG?|POS)$/=target",
      //"SQ|SINV < (/^(?:VB|MD|AUX)/=target $++ /^(?:VP|ADJP)/)",
      //"CONJP < TO=target < VB", // (CONJP not to mention)
                // add handling of tricky VP fronting cases...
      //"SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBG))",
      
      
      /*
       * 
       * VP < VP < /^(?:TO|MD|VB.*|AUXG?|POS)$/=target",
       * 
       */
      /*bool buildConj1(){
    	  if (node.constituent==PENN_CON_VP) {
    		  CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
    		  bool child1=false;
    	      bool child2=false;
    	      const CStateNode* targ=0;
    	      while(childsConjp!=0){
    	    	  if (childsConjp->node->constituent==PENN_CON_VP){
    	      		 child1=true;
    	      	  }
    	      	  if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB && (!isDangling(&node, childsConjp->node)) { //V
    	      		  child2=true;
    	      		  targ=childsConjp->node;
    	      	  }
    	      	  childsConjp=childsConjp->next;
    	      	}
    	        if (child1 && child2) {
    	        	CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    	      		if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)){
    	      			addDangling(&node,targ);
    	      			//std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    	      			return true;
    	      		}
    	      	}
    	  }
    	  return false;
      }*/
      
      
      
      /*
       * "CONJP < TO=target < VB"
       * 
       */
      bool buildConj3() {
    	 if (node.constituent==PENN_CON_CONJP) {
    		 CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
    		 bool child1=false;
    		 bool child2=false;
    		 const CStateNode* tOTarg=0;
    		 while(childsConjp!=0){
    			 if ( ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_TO) && (!isDangling(&node, childsConjp->node))){
    				 tOTarg=childsConjp->node;
    				 child1=true;
    			 }
    			 if ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB) { //VB
    			     child2=true;
    			 }
    			 childsConjp=childsConjp->next;
    		 }
    		 if (child1 && child2) {
    			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    			 if (buildStanfordLink(label, tOTarg->lexical_head, node.lexical_head)){
    				 addDangling(&node,tOTarg);
    			    //std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    			    return true;
    			 }
    		 }
    	 }
         return false;
      }
