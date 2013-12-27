    
    //"S|SQ < (WHNP|NP=target !< EX) < (VP < (/^(?:VB|AUX)/ < " + passiveAuxWordRegex + ")  < (VP < VBN|VBD))",
    
 inline const bool &buildNsubjpass1(const unsigned long &cons) {
    	if (cons==PENN_CON_S || cons==PENN_CON_SQ) {
    		CStateNodeList* childsSSQ=node.m_umbinarizedSubNodes;
    		while(childsSSQ!=0){
    			const CStateNode* whnpnpTarg=childsSSQ->node;
    			if ((CConstituent::clearTmp(whnpnpTarg->constituent.code())==PENN_CON_WHNP || CConstituent::clearTmp(whnpnpTarg->constituent.code())==PENN_CON_NP) && (!isLinked(&node,whnpnpTarg))){
    				CStateNodeList* childsTarg=whnpnpTarg->m_umbinarizedSubNodes;
    				while(childsTarg!=0){
    					if (!((*words)[childsTarg->node->lexical_head].tag.code()==PENN_TAG_EX)) {
    						CStateNodeList* childsSSQSec=node.m_umbinarizedSubNodes; //we have to look the rest of childs of S|SQ (node)
    						bool firstCondition=false;
    						bool secondCondition=false;
    						 while(childsSSQSec!=0){
    							 const CStateNode* vp=childsSSQSec->node;
    							 if (CConstituent::clearTmp(vp->constituent.code())==PENN_CON_VP) {
    								 CStateNodeList* childsVp=vp->m_umbinarizedSubNodes;
    								 while(childsVp!=0){
    									 const CStateNode* vb=childsVp->node;
    									 if (((*words)[childsTarg->node->lexical_head].tag.code()==PENN_TAG_VERB)) {
    										 //CStateNodeList* childsVB=vb->m_umbinarizedSubNodes;
    										 //while(childsVB!=0){
    											 if ((compareWordToPassiveAuxWordRegex((*words)[vb->lexical_head].word))) {
    												 firstCondition=true;
    											 }
    											// childsVB=childsVB->next;
    										 //}
    									 }
    									 childsVp=childsVp->next;
    								 }
    							 }
    							 childsSSQSec=childsSSQSec->next;
    						 }
    						 childsSSQSec=node.m_umbinarizedSubNodes; //we have to look the rest of childs of S|SQ (node)
    						 while(childsSSQSec!=0){
    							 const CStateNode* vp=childsSSQSec->node;
    							 if (CConstituent::clearTmp(vp->constituent.code())==PENN_CON_VP) {
    								 CStateNodeList* childsVp=vp->m_umbinarizedSubNodes;
    								 while(childsVp!=0){
    									 const CStateNode* vbvbn=childsVp->node;
    									 if (((*words)[childsTarg->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST) 
    											 || ((*words)[childsTarg->node->lexical_head].tag.code()==PENN_TAG_VERB_PAST_PARTICIPATE)) { //VBN|VBD
    										 secondCondition=true;
    									 }
    									 childsVp=childsVp->next; //
    								 }
    							 }
    							 childsSSQSec=childsSSQSec->next;
    						 }
    						 
    						 
    						 if (firstCondition && secondCondition) {
//    							 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJPASS);
    							 if (buildStanfordLink(STANFORD_DEP_NSUBJPASS, whnpnpTarg->lexical_head, node.lexical_head)) {
    								 addLinked(&node,whnpnpTarg);
    							 	 return true;
    							 }   
    						 }
    						 
    					}
    					childsTarg=childsTarg->next;
    				}
    				
    				
    				/*CStateNodeList* childsSSQSec=node.m_umbinarizedSubNodes;
    				while(childsSSQSec!=0){
    					
    					childsSSQSec=childsSSQSec->next;
    				}*/
    			}
    			childsSSQ=childsSSQ->next;
    		}
    	}
    }

