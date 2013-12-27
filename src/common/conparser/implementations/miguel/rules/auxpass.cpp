     //"VP < (/^(?:VB|AUX|POS)/=target < " + passiveAuxWordRegex + " ) < (VP|ADJP [ < VBN|VBD | < (VP|ADJP < VBN|VBD) < CC ] )",
inline const bool &auxpass1(const unsigned long &cons){
    	 if (cons==PENN_CON_VP){
    		 bool secondCondition=false;

    		 CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		 while(childsVp!=0){
    			 const CStateNode* vpadjp=childsVp->node;
    			 if ((CConstituent::clearTmp(vpadjp->constituent.code())==PENN_CON_VP)||(CConstituent::clearTmp(vpadjp->constituent.code())==PENN_CON_ADJP)){
    				 CStateNodeList* childsVpadjp=vpadjp->m_umbinarizedSubNodes;
    				 while(childsVpadjp!=0){
    					 if ((*words)[childsVpadjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
                  				 (*words)[childsVpadjp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
    						 secondCondition=true;
    					 }
    					 if (CConstituent::clearTmp(childsVpadjp->node->constituent.code())==PENN_CON_VP || CConstituent::clearTmp(childsVpadjp->node->constituent.code())==PENN_CON_ADJP){
    						 CStateNodeList* childsInt=childsVpadjp->node->m_umbinarizedSubNodes;
    						 while(childsInt!=0){
    							 if ((*words)[childsInt->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
    							     (*words)[childsInt->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
    								 CStateNodeList* childsVpadjp2=vpadjp->m_umbinarizedSubNodes;
    								 while(childsVpadjp2!=0) {
    									 if ((*words)[childsVpadjp2->node->lexical_head].tag.code()==PENN_TAG_CC) {
    										 secondCondition=true;
    									 }
    									 childsVpadjp2=childsVpadjp2->next;
    								 }
    							 }
    							 childsInt=childsInt->next;
    						 }
    					 }
    					 childsVpadjp=childsVpadjp->next;
    				 }
    			 }
    			 childsVp=childsVp->next;
    		 }

    		 if (secondCondition){
    			 childsVp=node.m_umbinarizedSubNodes;
    			 //VP < (/^(?:VB|AUX|POS)/=target < " + passiveAuxWordRegex + " )
    			 while(childsVp!=0){
    				 const CStateNode* verbTarg=childsVp->node;
    				 //PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
    				 if (((*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PROG ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PRES ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE ||
    				     (*words)[verbTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)&&!(isLinked(&node,verbTarg))) {

    					 CStateNodeList* childsVerb=verbTarg->m_umbinarizedSubNodes;
    						 if ((compareWordToPassiveAuxWordRegex((*words)[verbTarg->lexical_head].word))) {
//    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
    							 if (buildStanfordLink(STANFORD_DEP_AUXPASS, verbTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,verbTarg);
    							     return true;
    							 }
    						 }
    				 }
    				 childsVp=childsVp->next;
    			 }
    		 }

    	 }
    	 return false;
     }

//bool auxpass1(){}

//"SQ|SINV < (/^(?:VB|AUX|POS)/=target < " + beAuxiliaryRegex + " $++ (VP < VBD|VBN))",
inline const bool &auxpass2(const unsigned long &cons){
    	 if (cons==PENN_CON_SQ ||cons==PENN_CON_SINV) {
    		 CStateNodeList* childsSqS=node.m_umbinarizedSubNodes;
    		 while(childsSqS!=0){
    			 const CStateNode* vbauxposTarg=childsSqS->node;
    			 //I don't know what AUX is...
    			 if (((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
    					 ((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_POS) && !(isLinked(&node,vbauxposTarg))) {
    				 
    				 bool secondCondition=false;
    				 CStateNodeList* rightSisters=childsSqS->next;
    				 while(rightSisters!=0){
    					 const CStateNode* vpSist=rightSisters->node;
    					 if (CConstituent::clearTmp(vpSist->constituent.code())==PENN_CON_VP){
    						 CStateNodeList* childsVp=vpSist->m_umbinarizedSubNodes;
    						 while(childsVp!=0){
    							 if (((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST)||((*words)[vbauxposTarg->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) {
    								 secondCondition=true;
    							 }
    							 childsVp=childsVp->next;
    						 }
    					 }
    					 rightSisters=rightSisters->next;
    				 }
    				 if (secondCondition){
    						 if (compareWordToBeAuxiliaryWordRegex((*words)[vbauxposTarg->lexical_head].word)) {
//    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
    							 if (buildStanfordLink(STANFORD_DEP_AUXPASS, vbauxposTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,vbauxposTarg);
    							     return true;
    							 }
    						 }
    				 }
    				 
    			 }
    			 childsSqS=childsSqS->next;
    		 }
    	 }
    	 return false;
     }     


     //"SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBD|VBN))",
inline const bool &auxpass3(const unsigned long &cons){
    	 if (cons==PENN_CON_SINV){
    		 CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
    		 while(childsSinv!=0){
    			 const CStateNode* vpTarg=childsSinv->node;
    			 if (CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP && !(isLinked(&node,vpTarg))) {
    				 CStateNodeList* childsVp=vpTarg->m_umbinarizedSubNodes;
    				 while(childsVp!=0){
    					 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB)||
    					 	 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
    					 	 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
    					 	 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
    					 	 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
    					 	 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_POS)) {

    						 //CStateNodeList* childsVerb=childsVp->node->m_umbinarizedSubNodes;
    						 //while(childsVerb!=0){
    							 if ((compareWordToBeAuxiliaryWordRegex((*words)[childsVp->node->lexical_head].word))) {
    								 if (childsSinv->previous!=0){
    									 CStateNodeList* leftSistersVp=childsSinv->previous;
    									 while(leftSistersVp!=0){
    										 const CStateNode* vpLeftSister=leftSistersVp->node;
    										 if (CConstituent::clearTmp(vpLeftSister->constituent.code())==PENN_CON_VP){
    											 CStateNodeList* childsVpInLeft=vpLeftSister->m_umbinarizedSubNodes;
    											 while (childsVpInLeft!=0){
    												 if ((*words)[childsVpInLeft->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
    												     (*words)[childsVpInLeft->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
//    													 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
    													 if (buildStanfordLink(STANFORD_DEP_AUXPASS, vpTarg->lexical_head, node.lexical_head)) {
    														 addLinked(&node,vpTarg);
    													     return true;
    													 }
    												 }
    												 childsVpInLeft=childsVpInLeft->next;
    											 }
    										 }
    										 leftSistersVp=leftSistersVp->previous;
    									 }
    								 }
    						      }
    						      //childsVerb=childsVerb->next;
    						  //}
    					 }
    					 childsVp=childsVp->next;
    				 }

    			 }
    			 childsSinv=childsSinv->next;
    		 }

    	 }
    	 return false;

     }



     //"SINV < (VP=target < (VP < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ")) $-- (VP < VBD|VBN))",
inline const bool &auxpass4(const unsigned long &cons){
       	 if (cons==PENN_CON_SINV){
       		 CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
       		 while(childsSinv!=0){
       			 const CStateNode* vpTarg=childsSinv->node;
       			 if (CConstituent::clearTmp(vpTarg->constituent.code())==PENN_CON_VP && !(isLinked(&node,vpTarg))) {
       				 CStateNodeList* childs1stVp=vpTarg->m_umbinarizedSubNodes;
       				 while(childs1stVp!=0){
       					 if (CConstituent::clearTmp(childs1stVp->node->constituent.code())==PENN_CON_VP){
       						 CStateNodeList* childsVp=childs1stVp->node->m_umbinarizedSubNodes;
       						 while(childsVp!=0){
       							 if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB)||
       									 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)||
       									 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)||
       									 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES)||
       									 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)||
       									 ((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_POS)) {

       								 //CStateNodeList* childsVerb=childsVp->node->m_umbinarizedSubNodes;
       								 //while(childsVerb!=0){
       									 if ((compareWordToBeAuxiliaryWordRegex((*words)[childsVp->node->lexical_head].word))) {
       										 if (childsSinv->previous!=0){
       											 CStateNodeList* leftSistersVp=childsSinv->previous;
       											 while(leftSistersVp!=0){
       												 const CStateNode* vpLeftSister=leftSistersVp->node;
       												 if (CConstituent::clearTmp(vpLeftSister->constituent.code())==PENN_CON_VP){
       													 CStateNodeList* childsVpInLeft=vpLeftSister->m_umbinarizedSubNodes;
       													 while (childsVpInLeft!=0){
       														 if ((*words)[childsVpInLeft->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST ||
       																 (*words)[childsVpInLeft->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE) {
//       															 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUXPASS);
       															 if (buildStanfordLink(STANFORD_DEP_AUXPASS, vpTarg->lexical_head, node.lexical_head)) {
       																 addLinked(&node,vpTarg);
       																 return true;
       															 }
       														 }
       														 childsVpInLeft=childsVpInLeft->next;
       													 }
       												 }
       												 leftSistersVp=leftSistersVp->previous;
       											 }
       										 }
       									 }
       									 //childsVerb=childsVerb->next;
       								 //}
       							 }
       							 childsVp=childsVp->next;
       						 }
       					 }
       					 childs1stVp=childs1stVp->next;
       				 }

       			 }
       			 childsSinv=childsSinv->next;
       		 }

       	 }
       	 return false;

        }

