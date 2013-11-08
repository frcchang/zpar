//"NP|WHNP|NML $++ (SBAR=target <+(SBAR) WHPP|WHNP) !$-- NP|WHNP|NML > @NP|WHNP",
bool rcmod1(){

}

//"NP|WHNP|NML $++ (SBAR=target <: (S !<, (VP <, TO))) !$-- NP|WHNP|NLP > @NP|WHNP",
bool rcmod2(){

}


//"NP|NML $++ (SBAR=target < (WHADVP < (WRB </^(?i:where|why|when)/))) !$-- NP|NML > @NP",
bool rcmod3(){

}

//"NP|WHNP|NML $++ RRC=target !$-- NP|WHNP|NML",
bool rcmod4(){

}

//"@ADVP < (@ADVP < (RB < /where$/)) < @SBAR=target",
bool rcmod5(){

}
