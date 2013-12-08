
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

       	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
       	 while(childs!=0){
       		 const CStateNode* headSbar=childs->node;
       		 	 if (headSbar->constituent==PENN_CON_SBAR){
       		 		 bool secCondition=false;
       		 		 bool thirdCondition=false;
       		 		 bool fourthCondition=false;
       		 		 bool fifthCondition=false;

       		 		 CStateNodeList* leftSisters=childs->previous;
       		 		 while(leftSisters!=0){
       		 			//PENN_TAG_VERB, PENN_TAG_VERB_PAST, PENN_TAG_VERB_PROG, PENN_TAG_VERB_PAST_PARTICIPATE, PENN_TAG_VERB_PRES, PENN_TAG_VERB_THIRD_SINGLE,
       		 			if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB||
       		 					(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST||
       		 					(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG||
       		 					(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE||
       		 					(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES||
       		 					(*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
       		 				secCondition=true;
       		 			}
       		 			 leftSisters=leftSisters->previous;
       		 		 }
       		 		 if (!secCondition){
       		 			 if (childs->previous!=0){
       		 				 if ((childs->previous->node->constituent==PENN_CON_NP)||((*words)[childs->previous->node->lexical_head].tag.code()==PENN_TAG_NOUN)||
       	    		 					((*words)[childs->previous->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL)){

       		 					 thirdCondition=true;
       		 				 }
       		 			 }
       		 		 }
       		 		 if (!secCondition&&!thirdCondition){
       		 			 if (node.constituent==PENN_CON_ADJP || node.constituent==PENN_CON_PP){
       		 				 fourthCondition=true;
       		 			 }
       		 		 }
       		 		 if (!secCondition && !thirdCondition && !fourthCondition){
       		 			 if (node.constituent==PENN_CON_UCP||node.constituent==PENN_CON_NP||node.constituent==PENN_CON_SBAR){
       		 				 CStateNodeList* childsUNS=node.m_umbinarizedSubNodes;
       		 				 while(childsUNS!=0){
       		 					 if ((childsUNS->node->constituent==PENN_CON_CONJP)||(*words)[childsUNS->node->lexical_head].tag.code()==PENN_TAG_CC){
       		 						 CStateNodeList* leftSistersUNS=childsUNS;
       		 						 while(leftSistersUNS!=0){
       		 							if ((*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB||
       		 							    (*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST||
       		 							    (*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG||
       		 							    (*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE||
       		 							    (*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB_PRES||
       		 							    (*words)[leftSistersUNS->node->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE){
       		 								fifthCondition=true;
       		 							}
       		 							 leftSistersUNS=leftSistersUNS->previous;
       		 						 }

       		 					 }
       		 					 childsUNS=childsUNS->next;
       		 				 }
       		 			 }
       		 		 }

       		 		 if (secCondition || thirdCondition || fourthCondition || fifthCondition) {
       		 			 CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
       		 			 while(childsSbar!=0){

       		 				 const CStateNode* indtTarg=childsSbar->node;
       		 				 if (((*words)[indtTarg->lexical_head].tag.code()==PENN_TAG_IN||
       							 (*words)[indtTarg->lexical_head].tag.code()==PENN_TAG_DT)&&!(isLinked(&node,indtTarg))){

       		 					 CStateNodeList* childsInDt=indtTarg->m_umbinarizedSubNodes;
       		 					 while(childsInDt!=0){
       		 						 if ((*words)[childsInDt->next->node->lexical_head].word==g_word_that &&
       		 								 (*words)[childsInDt->next->node->lexical_head].word==g_word_whether){

       		 							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_MARK);
       		 							 if (buildStanfordLink(label, indtTarg->lexical_head, headSbar->lexical_head)) {
       		 								 addLinked(&node,indtTarg);
       		 								 return true;
       		 							 }
       		 						 }
       		 						 childsInDt=childsInDt->next;
       		 					 }
       		 				 }
       		 				 childsSbar=childsSbar->next;
       		 			 }
       		 		 }

       		 	 }
       		 	 childs=childs->next;
       	 	 }
       	 	 return false;
        	 }

