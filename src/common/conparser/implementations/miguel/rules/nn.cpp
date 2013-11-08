
//"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < (NP|NML|NN|NNS|NNP|NNPS|FW|AFX=target $++ NN|NNS|NNP|NNPS|FW|CD !<<- POS !<<- (VBZ < /^\'s$/) !$- /^,$/ )",
bool nn1(){

}

//"/^(?:WH)?(?:NP|NX|NAC|NML)(?:-TMP|-ADV)?$/ < JJ|JJR|JJS=sister < (NP|NML|NN|NNS|NNP|NNPS|FW=target !<<- POS !<<- (VBZ < /^\'s$/) $+ =sister) <# NN|NNS|NNP|NNPS !<<- POS !<<- (VBZ < /^\'s$/) ",
bool nn2(){

}

//"ADJP|ADVP < (FW [ $- FW=target | $- (IN=target < in|In) ] )",  // in vitro, in vivo, etc., in Genia
bool nn3(){

}
