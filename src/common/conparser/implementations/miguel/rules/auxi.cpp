      
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
inline const bool &buildAux1(const unsigned long &cons){
    	  if (cons==PENN_CON_VP) {
    		  CStateNodeList* childsConjp=node.m_umbinarizedSubNodes;
    		  bool child1=false;

    	      while(childsConjp!=0){
    	    	  if (CConstituent::clearTmp(childsConjp->node->constituent.code())==PENN_CON_VP){
    	      		 child1=true;
    	      	  }
    	    	  childsConjp=childsConjp->next;
    	      }
    	      if (child1){
    	    	  CStateNodeList* childsConjp2=node.m_umbinarizedSubNodes;
    	    	  while(childsConjp2!=0){
    	    		  if ((((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB) ||
    	      			((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST) ||
    	      			((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG) ||
    	      			((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) ||
    	      			((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES) ||
    	      			((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) ||
    	      			  ((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_MD) ||
    	      			  ((*words)[childsConjp2->node->lexical_head].tag.code()==PENN_TAG_TO))
    	      			  && (!isLinked(&node, childsConjp2->node))) { //
    	      		  const CStateNode* targ=childsConjp2->node;
//    	      		  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    	      		  if (buildStanfordLink(STANFORD_DEP_AUX, targ->lexical_head, node.lexical_head)){
    	      			  addLinked(&node,targ);
    	      			  //std::cout<<"aux1 \n";
    	      			  //std::cout<<"dependent:"<<(*words)[targ->lexical_head].word<<"\n";
    	      			  //std::cout<<"head:"<<(*words)[node.lexical_head].word<<"\n";
    	      			  return true;
    	      		  }
    	      	  }
    	      	  childsConjp2=childsConjp2->next;
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
inline const bool &buildAux2(const unsigned long &cons){
    	  if (cons==PENN_CON_SQ || cons==PENN_CON_SINV) {
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while (childs!=0) {
    		  const CStateNode* targ=childs->node;
    	      if ((((*words)[targ->lexical_head].tag.code()==PENN_TAG_VERB) ||
    	      		((*words)[targ->lexical_head].tag.code()==PENN_TAG_MD)) 
    	      			  && (!isLinked(&node, targ))) { 
    	      		  	
    	    	  		CStateNodeList* rightSisters=childs;
    	    	  		while(rightSisters!=0){
    	    	  			const CStateNode* rSis=rightSisters->node;
    	    	  			if ((CConstituent::clearTmp(rSis->constituent.code())==PENN_CON_VP) || (CConstituent::clearTmp(rSis->constituent.code())==PENN_CON_ADJP)) {
//    	    	  				CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    	    	  				if (buildStanfordLink(STANFORD_DEP_AUX, targ->lexical_head, node.lexical_head)){
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
inline const bool &buildAux3(const unsigned long &cons) {
    	 if (cons==PENN_CON_CONJP) {
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
//    			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    			 if (buildStanfordLink(STANFORD_DEP_AUX, tOTarg->lexical_head, node.lexical_head)){
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
inline const bool &buildAux4(const unsigned long &cons) {
        	 if (cons==PENN_CON_SINV) {
           		 CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
           		 while (childsSinv!=0){
              		const CStateNode* vpTarg=childsSinv->node;
              		if (CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP && (isLinked(&node, vpTarg))) {
              		CStateNodeList* childsVp=vpTarg->m_umbinarizedSubNodes;
              		bool firstCondVp=false;
              		bool secondCondVp=false;
              		while(childsVp!=0) {
              			const CStateNode* childVp=childsVp->node;
              			if ((*words)[childVp->lexical_head].tag.code()==PENN_TAG_VERB ||
              				 (*words)[childVp->lexical_head].tag.code()==PENN_TAG_POS) { //VB or POS
              					if (compareWordToBeAuxiliaryWordRegex((*words)[childVp->lexical_head].word)) {
              						firstCondVp=true;
              				    }
              			 }
              			 childsVp=childsVp->next;
              		 }	 
              		 //left sisters vp
              		 CStateNodeList* leftSistersVp=childsSinv->previous;
              		 while(leftSistersVp!=0){
              			 const CStateNode* vpChild=leftSistersVp->node;
              			 if (CConstituent::clearTmp(vpChild->constituent.code())==PENN_CON_VP){
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
//              			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
              			 if (buildStanfordLink(STANFORD_DEP_AUX, vpTarg->lexical_head, node.lexical_head)){
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
