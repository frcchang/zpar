      
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
       * 
       * PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE
       */
      bool buildAux1(){
    	  if (node.constituent==PENN_CON_VP) {
    		  CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
    		  bool child1=false;
    	      bool child2=false;
    	      const CStateNode* targ=0;
    	      while(childsConjp!=0){
    	    	  if (childsConjp->node->constituent==PENN_CON_VP){
    	      		 child1=true;
    	      	  }
    	      	  if ((((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB) ||
    	      			((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST) ||
    	      			((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG) ||
    	      			((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) ||
    	      			((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES) ||
    	      			((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) ||
    	      			  ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_MD) ||
    	      			  ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_TO)) 
    	      			  && (!isLinked(&node, childsConjp->node))) { //V
    	      		  child2=true;
    	      		  targ=childsConjp->node;
    	      	  }
    	      	  childsConjp=childsConjp->next;
    	      	}
    	        if (child1 && child2) {
    	        	CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    	      		if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)){
    	      			addLinked(&node,targ);
    	      			//std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    	      			return true;
    	      		}
    	      	}
    	  }
    	  return false;
      }
      
      
      /*
      *"SQ|SINV < (/^(?:VB|MD|AUX)/=target $++ /^(?:VP|ADJP)/)",
      * 
      * 
      */
      bool buildAux2(){
    	  if (node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_SINV) {
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while (childs!=0) {
    		  const CStateNode* targ=childs->node;
    	      if ((((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB) ||
    	      		((*words)[targ->lexical_head].tag.code()==PENN_TAG_MD)) 
    	      			  && (!isLinked(&node, targ))) { 
    	      		  	
    	    	  		CStateNodeList* rightSisters=childs;
    	    	  		while(rightSisters!=0){
    	    	  			const CStateNode* rSis=rightSisters->node;
    	    	  			if ((rSis->constituent==PENN_CON_VP) || (rSis->constituent==PENN_CON_ADJP)) {
    	    	  				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    	    	  				if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)){
    	    	  					addLinked(&node,targ);
    	    	  				    //std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    	    	  				    return true;
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
      
      
      
      
      /*
       * "CONJP < TO=target < VB"
       * 
       */
      bool buildAux3() {
    	 if (node.constituent==PENN_CON_CONJP) {
    		 CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
    		 bool child1=false;
    		 bool child2=false;
    		 const CStateNode* tOTarg=0;
    		 while(childsConjp!=0){
    			 if ( ((*words)[childsConjp->node->lexical_head].tag.code()==PENN_TAG_TO) && (!isLinked(&node, childsConjp->node))){
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
    				 addLinked(&node,tOTarg);
    			    //std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    			    return true;
    			 }
    		 }
    	 }
         return false;
      }

      

      /*
       * SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBG))",
       * 
       */
      //SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBG))",
      bool buildAux4() {
        	 if (node.constituent==PENN_CON_SINV) {
           		 CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
           		 while (childsSinv!=0){
              		const CStateNode* vpTarg=childsSinv->node;
              		if (vpTarg->constituent==PENN_CON_VP && (isLinked(&node, vpTarg))) {
              		CStateNodeList* childsVp=vpTarg->m_umbinarizedSubNodes;
              		bool firstCondVp=false;
              		bool secondCondVp=false;
              		while(childsVp!=0) {
              			const CStateNode* childVp=childsVp->node;
              			if ((*words)[childVp->lexical_head].tag.code()==PENN_TAG_VERB ||
              				 (*words)[childVp->lexical_head].tag.code()==PENN_TAG_POS) { //VB or POS
              				 CStateNodeList* childsVbPos=childVp->m_umbinarizedSubNodes;
              				 while(childsVbPos!=0){
              					 if (compareWordToTimeWordRegex((*words)[childsVbPos->node->lexical_head].word)) {
              						 firstCondVp=true;
              					 }
              					 childsVbPos=childsVbPos->next;
              				 }
              			 }
              			 childsVp=childsVp->next;
              		 }	 
              		 //left sisters vp
              		 CStateNodeList* leftSistersVp=childsSinv->previous;
              		 while(leftSistersVp!=0){
              			 const CStateNode* vpChild=leftSistersVp->node;
              			 if (vpChild->constituent==PENN_CON_VP){
              			 //child PENN_TAG_VERB_PROG?
              				 CStateNodeList* childsSecVp=vpChild->m_umbinarizedSubNodes;
              				 while (childsSecVp!=0) {
              					 if ((*words)[childsSecVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG) { //VBG
              					 secondCondVp=true;
              					 }
              					 childsSecVp=childsSecVp->next;
              				 }
              						 
              			 }		 
              			 leftSistersVp=leftSistersVp->previous;
              		 }
              				 
              		 if (firstCondVp && secondCondVp) {
              			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
              			 if (buildStanfordLink(label, vpTarg->lexical_head, node.lexical_head)){
              				 addLinked(&node,vpTarg);
              				 //std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
              				 return true;
              			  }
              		  }
                  }
              	  childsSinv=childsSinv->next;
              }
           }
           return false;
        }
