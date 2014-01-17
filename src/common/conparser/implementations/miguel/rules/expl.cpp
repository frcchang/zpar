 //"S|SQ|SINV < (NP=target <+(NP) EX)"
     inline const bool &buildExpl1(const unsigned long &cons) {
        if (cons==PENN_CON_S || cons==PENN_CON_SQ || cons==PENN_CON_SINV) {
     	   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
     	   while (childsS!=0) {
     		   const CStateNode* npTarg=childsS->node;
     		   if (CConstituent::clearTmp(npTarg->constituent.code())==PENN_CON_NP && (!isLinked(&node, npTarg))){
     		   CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;

				while(childsNp!=0) {
					const CStateNode* npChild=childsNp->node;//zero or more REMEMBER, this is why we can have this case.
					if ((*words)[npChild->lexical_head].tag.code()==PENN_TAG_EX) {
//						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_EXPL);
						if (buildStanfordLink(STANFORD_DEP_EXPL, npTarg->lexical_head, node.lexical_head)){
    		      				addLinked(&node,npTarg);
    		      				//std::cout<<"expl"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    		      				return true;
    		      			}
					}
					else if (CConstituent::clearTmp(npChild->constituent.code())==PENN_CON_NP) { //zero or more REMEMBER, this is why we have the thing shown above.
						CStateNodeList* npsChain=new CStateNodeList();
      						  //std::cout<<"findingchain\n";
      					findChainTargetPos(PENN_CON_NP,PENN_TAG_EX,npChild,npsChain);
      					if (npsChain->node==0) {
      						npsChain->clear();
      					    npsChain=0;
      					}
						while(npsChain!=0) {
							const CStateNode* npCh=npsChain->node;
							if ((*words)[npChild->lexical_head].tag.code()==PENN_TAG_EX) {
//								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_EXPL);
								if (buildStanfordLink(STANFORD_DEP_EXPL, npTarg->lexical_head, node.lexical_head)){
    		      						addLinked(&node,npTarg);
    		      						//std::cout<<"expl"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
    		      						return true;
								}
							}
							npsChain=npsChain->next;
						}
					}
					childsNp=childsNp->next;
     			}

     		   }
     		   childsS=childsS->next;
     	   }
        }
     return false;
     }
