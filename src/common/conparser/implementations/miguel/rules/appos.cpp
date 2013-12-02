 //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (NP=target !<: CD $- /^,$/ $-- /^(?:WH)?NP/ !$ CC|CONJP)",
      bool appos1(){
    	  if (node.constituent==PENN_CON_WHNP||node.constituent==PENN_CON_NP){
    		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
    		  while(childs!=0){
    			  const CStateNode* targ=childs->node;
    			  if (targ->constituent==PENN_CON_NP && !isLinked(&node,targ)){
    				  bool cdCond=true;
    				  bool firstSisCond=false;
    				  bool leftSisCond=false;
    				  bool leftRightCond=true;

    				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
    				  if(childsT!=0){
    					  if (((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_CD) && childsT->next==0){
    						  cdCond=false;
    					  }
    				  }
    				  if (cdCond){
    					  CStateNodeList* leftSisters=childs->previous;
    					  if (((*words)[leftSisters->node->lexical_head].word==g_word_comma)){
    						  firstSisCond=true;
    					  }
    					  while(leftSisters!=0){
    						  if (leftSisters->node->constituent==PENN_CON_WHNP || leftSisters->node->constituent==PENN_CON_NP){
    							  leftSisCond=true;
    						  }
    						  if (leftSisters->node->constituent==PENN_CON_CONJP || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    							leftRightCond=false;
    						  }
    						  leftSisters=leftSisters->previous;
    					  }
    					  if (leftSisCond && leftRightCond){
    						  CStateNodeList* rightSisters=childs->next;
    						  while(rightSisters!=0){
    							  if (rightSisters->node->constituent==PENN_CON_CONJP || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
    								  leftRightCond=false;
    							  }
    							  rightSisters=rightSisters->next;
    						  }
    					  }

    				  }

    				  if (cdCond && firstSisCond && leftSisCond && leftRightCond){
    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_APPOS);
    					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
    						  addLinked(&node,targ);
    					      return true;
    					 }
    				  }
    			  }
    			  childs=childs->next;
    		  }
    	  }
    	  return false;
      }

      //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (PRN=target < (NP < /^(?:NN|CD)/ $-- /^-LRB-$/ $+ /^-RRB-$/))",
            bool appos2(){
          	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP){
          		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
          		  while(childs!=0){
          			  const CStateNode* targ=childs->node;
          			  if (targ->constituent==PENN_CON_PRN && !isLinked(&node,targ)){
          				  CStateNodeList* childsPr=targ->m_umbinarizedSubNodes;
          				  while(childsPr!=0){
          					  if (childsPr->node->constituent==PENN_CON_NP){
          						  CStateNodeList* childsNp=childsPr->node->m_umbinarizedSubNodes;
          						  bool childCond=false;
          						  bool leftCond=false;
          						  bool rightCond=false;

          						  while(childsNp!=0){
          							  //PENN_TAG_NOUN, PENN_TAG_NOUN_PROPER, PENN_TAG_NOUN_PROPER_PLURAL, PENN_TAG_NOUN_PLURAL,
          							  if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN||
          									  (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER||
          									  (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL||
          									  (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL||
          									  (*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_CD){
          								  childCond=true;
          							  }
          							  childsNp=childsNp->next;
          						  }

          						  CStateNodeList* leftSisters=childsPr->previous;
          						  while(leftSisters!=0){
          							  if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
          								  leftCond=true;
          							  }
          							  leftSisters=leftSisters->previous;
          						  }

          						  if (childsPr->next!=0){
          							  if ((*words)[childsPr->node->lexical_head].tag.code()==PENN_TAG_R_BRACKET){
          								  rightCond=true;
          							  }
          						  }

          						  if (childCond && leftCond && rightCond){
          							  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_APPOS);
          							  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
          								  addLinked(&node,targ);
          							  	  return true;
          							  }
          						  }

          					  }
          					  childsPr=childsPr->next;
          				  }
          			  }
          			  childs=childs->next;
          		  }
          	  }
          	  return false;

            }


            //"@WHNP|NP < (@NP=target !<: CD <, /^-LRB-$/ <` /^-RRB-$/ $-- /^(?:WH)?NP/ !$ CC|CONJP)",
                  bool appos3(){
                	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP){
                		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
                		  while(childs!=0){
                			  const CStateNode* targ=childs->node;
                			  if (targ->constituent==PENN_CON_NP && !isLinked(&node,targ)){
                				  bool firstCond=true;
                				  bool secCond=false;
                				  bool thirdCond=false;
                				  bool fourthCond=false;
                				  bool fifthCond=true;

                				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
                				  if (childsT!=0){
                					  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_CD && childsT->next==0){
                						  firstCond=false;
                					  }
                					  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
                						  secCond=true;
                					  }
                				  }
                				  if (firstCond && secCond){
                					  while(childsT!=0){
                						  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_R_BRACKET && childsT->next==0){
                							  thirdCond=true;
                						  }
                						  childsT=childsT->next;
                					  }
                				  }
                				  CStateNodeList* leftSisters=childs->previous;
                				  while(leftSisters!=0){
                					  if (leftSisters->node->constituent==PENN_CON_WHNP || leftSisters->node->constituent==PENN_CON_NP){
                						  fourthCond=true;
                					  }
                					  else if (leftSisters->node->constituent==PENN_CON_CONJP || (*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
                						  fifthCond=false;
                					  }
                					  leftSisters=leftSisters->previous;
                				  }

                				  if (fifthCond){
                					  CStateNodeList* rightSisters=childs->next;
                					  while(rightSisters!=0){
                						  if (rightSisters->node->constituent==PENN_CON_CONJP || (*words)[rightSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
                							  fifthCond=false;
                						  }
                						  rightSisters=rightSisters->next;
                					  }
                				  }


                				  if (firstCond && secCond && thirdCond && fourthCond && fifthCond){
                					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_APPOS);
                					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
                						  addLinked(&node,targ);
                					      return true;
                					  }
                				  }
                			  }
                			  childs=childs->next;
                		  }
                	  }
                	  return false;
                  }





//"NP|NP-TMP|NP-ADV < (NNP $+ (/^,$/ $+ NNP=target)) !< CC|CONJP",
   bool appos4(){
 	  if (node.constituent==PENN_CON_NP){
 		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
 		  bool lastCond=true;
 		  while(childs!=0){
				  if (childs->node->constituent==PENN_CON_CONJP || (*words)[childs->node->lexical_head].tag.code()==PENN_TAG_CC){
					  lastCond=false;
				  }
 			  childs=childs->next;
 		  }

 		  if (lastCond){
 			  childs=node.m_umbinarizedSubNodes;
 			  while(childs!=0){
 				  if ((*words)[childs->node->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER){
 					  if (childs->next!=0){
 						  if (((*words)[childs->next->node->lexical_head].word==g_word_comma)){
 							  if (childs->next->next!=0){
 								  const CStateNode* targ=childs->next->next->node;
 			    				  if ((*words)[targ->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER && !isLinked(&node,targ)){
 			    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_APPOS);
 			    					  if (buildStanfordLink(label, targ->lexical_head, node.lexical_head)) {
 			    						  addLinked(&node,targ);
 			    					      return true;
 			    					  }
 			    				  }

 							  }
 						  }
 					  }

 				  }
 				  childs=childs->next;
 			  }
 		  }
 	  }
 	  return false;

   }


   //SIN TERMINAR!!!!!
   //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (PRN=target <, /^-LRB-$/ <- /^-RRB-$/ !<< /^(?:POS|(?:WP|PRP)\\$|[,$#]|CC|RB|CD)$/ <+(NP) (NNP|NN < /^(?:[A-Z]\\.?){2,}/) )"
         bool appos5(){
       	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP){
       		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
       		  while(childs!=0){
       			  const CStateNode* targ=childs->node;
       			  if (targ->constituent==PENN_CON_PRN && !isLinked(&node,targ)){
       				  bool firstCond=false; //<, /^-LRB-$/
       				  bool secondCond=false; // <- /^-RRB-$/
       				  bool thirdCond=true; //!<< /^(?:POS|(?:WP|PRP)\\$|[,$#]|CC|RB|CD)$/
       				  bool fourthCond=true; //<+(NP) (NNP|NN < /^(?:[A-Z]\\.?){2,}/)

       				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
       				  if (childsT!=0){
       					  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_L_BRACKET){
       						  firstCond=true;
       				      }
       				  }
       				  while(childsT!=0){
       					  if ((*words)[childsT->node->lexical_head].tag.code()==PENN_TAG_R_BRACKET && childsT->next==0){
       						  secondCond=true;
       					  }
       					  childsT=childsT->next;
       				  }

       				  //void listDescendants (CStateNodeList* childs, CStateNodeList*& candidates){
       				  if (firstCond && secondCond){
       					  CStateNodeList* descendants=new CStateNodeList();
       					  listDescendants(targ->m_umbinarizedSubNodes,descendants);
       					  while(descendants!=0){
       						  if (((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_POS)|| ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_PRP)
       								  || ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_CC) || ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_ADVERB)||
       								  ((*words)[descendants->node->lexical_head].tag.code()==PENN_TAG_CD)){ //??????

       						  }
       						  descendants=descendants->next;
       					  }
       				  }

       			  }
       			  childs=childs->next;
       		  }
       	  }
       	  return false;
         }


