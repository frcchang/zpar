     //"VP < (PRN=target < S|SINV|SBAR)",
     bool parataxis1(){
     	if (node.constituent==PENN_CON_VP){
     		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
     		while(childsVp!=0){
     			const CStateNode* prnTarg=childsVp->node;
     			if (prnTarg->constituent==PENN_CON_PRN && !(isLinked(&node, prnTarg))){
     				CStateNodeList* childsPrn=prnTarg->m_umbinarizedSubNodes;
     				while(childsPrn!=0){
     					if (childsPrn->node->constituent==PENN_CON_S ||childsPrn->node->constituent==PENN_CON_SINV|| childsPrn->node->constituent==PENN_CON_SBAR) {
     						CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PARATAXIS);
     						if (buildStanfordLink(label, prnTarg->lexical_head, node.lexical_head)) {
     							addLinked(&node,prnTarg);
     							return true;
     						}
     					}
     					childsPrn=childsPrn->next;
     				}
     			}
     			childsVp=childsVp->next;
     		}
     	}
     	return false;
     }

//"VP $ (PRN=target [ < S|SINV|SBAR | < VP < @NP ] )"
bool parataxis2(){

}


//"S|VP < (/^:$/ $+ /^S/=target) !<, (__ $++ CC|CONJP)",
bool parataxis3(){

}


//"@S < (@S $.. @S=target) !< @CC|CONJP",
bool parataxis4(){

}
