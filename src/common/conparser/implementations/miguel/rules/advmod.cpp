
//"/^(?:VP|ADJP|JJP|WHADJP|SQ?|SBARQ?|SINV|XS|RRC|(?:WH)?NP(?:-TMP|-ADV)?)$/ < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
bool advmod1(){

}


//"ADVP|WHADVP < (RB|RBR|RBS|WRB|ADVP|WHADVP|JJ=target !< " + NOT_PAT + ") !< CC !< CONJP",
bool advmod2(){

}


//"SBAR < (WHNP=target < WRB)", "SBARQ <, WHADVP=target", "XS < JJ=target",
bool advmod3(){

}

//"/(?:WH)?PP(?:-TMP|-ADV)?$/ <# (__ $-- (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + "))",
bool advmod4(){

}


//"/(?:WH)?PP(?:-TMP|-ADV)?$/ < @NP|WHNP < (RB|RBR|RBS|WRB|ADVP|WHADVP=target !< " + NOT_PAT + ")",
bool advmod5(){

}

//"CONJP < (RB=target !< " + NOT_PAT + ")",
bool advmod6(){

}


