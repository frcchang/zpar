//"/^(?:WH)?(?:NP|NX|NML)(?:-TMP|-ADV)?$/ < (CD|QP=target !$- CC)",
      bool num1(){
     	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP || node.constituent==PENN_CON_NX){
     		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
     		  while(childs!=0){
     			  const CStateNode* targ=childs->node;
     			  if ((targ->constituent==PENN_CON_QP || (*words)[targ->lexical_head].tag.code()==PENN_TAG_CD) && !isLinked(&node,targ)){
     				  bool ccCond=true;
     				  CStateNodeList* leftSisters=childs->previous;
     				  if (leftSisters!=0){
     					  if ((*words)[leftSisters->node->lexical_head].tag.code()==PENN_TAG_CC){
     						  ccCond=true;
     					  }
     				  }
     				  if (ccCond){
     					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NUM);
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

//"/^(?:WH)?(?:NP|NX|NML)(?:-TMP|-ADV)?$/ < (ADJP=target <: QP)"
     bool num2(){
   	  if (node.constituent==PENN_CON_WHNP || node.constituent==PENN_CON_NP || node.constituent==PENN_CON_NX){
   		  CStateNodeList* childs=node.m_umbinarizedSubNodes;
   		  while(childs!=0){
   			  const CStateNode* targ=childs->node;
   			  if (targ->constituent==PENN_CON_ADJP && !isLinked(&node,targ)){
   				  bool qpCond=false;
   				  CStateNodeList* childsT=targ->m_umbinarizedSubNodes;
   				  if (childsT->node->constituent==PENN_CON_QP && childsT->next==0){
   					  qpCond=true;
   				  }
   				  if (qpCond){
   					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NUM);
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
