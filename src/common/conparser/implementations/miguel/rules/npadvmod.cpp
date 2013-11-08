
//"@ADVP|ADJP|WHADJP|WHADVP|PP|WHPP <# (JJ|JJR|IN|RB|RBR !< notwithstanding $- (@NP=target !< NNP|NNPS))",
bool npadvmod1(){

}

//"@ADJP < (NN=target $++ /^JJ/) !< CC|CONJP",
bool npadvmod2(){

}


//"@NP|WHNP < /^NP-ADV/=target",
bool npadvmod3(){

}


//"@NP <1 (@NP <<# /^%$/) <2 (@NP=target <<# days|month|months) !<3 __",
bool npadvmod4(){

}

//"@VP < /^NP-ADV/=target",
bool npadvmod5(){

}
