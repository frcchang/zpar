

   //"__ < /^(?:\\.|:|,|''|``|\\*|-LRB-|-RRB-|HYPH)$/=target"
     inline const bool & buildPunct1(const unsigned long &cons) {
    	 CStateNodeList* childs=node.m_umbinarizedSubNodes;
    	 while(childs!=0){
    		 const CStateNode* child=childs->node;
    		 if ((((*words)[child->lexical_head].word==g_word_order)
    				 || ((*words)[child->lexical_head].word==g_word_two_dots)
    				 || ((*words)[child->lexical_head].word==g_word_dot)
    				 || ((*words)[child->lexical_head].tag.code()==PENN_TAG_R_QUOTE)
    				 || ((*words)[child->lexical_head].word==g_word_comma)
    				 || ((*words)[child->lexical_head].tag.code()==PENN_TAG_L_QUOTE)
    				 || ((*words)[child->lexical_head].word==g_word_asterisc)
    				 || ((*words)[child->lexical_head].word==g_word_hyphen)
    				 || ((*words)[child->lexical_head].word==g_word_hyphen2)
    				 || ((*words)[child->lexical_head].word==g_word_lowhyphen)
    				 || ((*words)[child->lexical_head].tag.code()==PENN_TAG_L_BRACKET)
    				 || ((*words)[child->lexical_head].tag.code()==PENN_TAG_R_BRACKET)
    				 || ((*words)[child->lexical_head].word==g_word_order))
    				 && (!isLinked(&node,child))){

//    			 CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_PUNCT);
    			 if (buildStanfordLink(STANFORD_DEP_PUNCT, child->lexical_head, node.lexical_head)) {
    				 addLinked(&node,child);
    			     return true;
    			 }

    		 }
    		 childs=childs->next;
    	 }

     }


//"__ < (NFP=target !< /^(?:[<>]?[:;=8][\\-o\\*']?(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])|(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])[\\-o\\*']?[:;=8][<>]?)$/"
//              + "!< /^(?:-LRB-)?[\\-\\^x=~<>'][_.]?[\\-\\^x=~<>'](?:-RRB-)?$/)"
     inline const bool & buildPunct2(const unsigned long &cons) {
	//I think we can safely ignore it, since we don't have NFP in our data set.
}
