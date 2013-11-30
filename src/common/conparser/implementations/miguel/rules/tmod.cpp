
//"VP|ADJP < NP-TMP=target",
bool tmod1(){

}

//"VP|ADJP < (NP=target <# (/^NN/ < " + timeWordRegex + ") !$+ (/^JJ/ < old))",
bool tmod2(){

}

//"@PP < (IN|TO|VBG|FW $++ (@NP $+ NP-TMP=target))",
bool tmod3(){
//we can safely ignore it, NP-TMP is not in our data set.
}

//"@PP < (IN|TO|VBG|FW $++ (@NP $+ (NP=target <# (/^NN/ < " + timeWordRegex + "))))",
bool tmod4(){

}


//"S < (NP-TMP=target $++ VP [ $++ NP | $-- NP] )",
bool tmod5(){

}

//"S < (NP=target <# (/^NN/ < " + timeWordRegex + ") $++ (NP $++ VP))",
bool tmod6(){

}

//"SBAR < (@WHADVP < (WRB < when)) < (S < (NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) ))) !$-- CC $-- NP > NP=target",
bool tmod7(){

}

//"SBARQ < (@WHNP=target <# (/^NN/ < " + timeWordRegex + ")) < (SQ < @NP)",
bool tmod8(){

}

//"NP < NP-TMP=target"
bool tmod9(){
//we can safely ignore it...
}
