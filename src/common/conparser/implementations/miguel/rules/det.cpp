//"/^(?:NP(?:-TMP|-ADV)?|NAC|NML|NX|X)$/ < (DT=target !< /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:N[MNXP]|CD|JJ|FW|ADJP|QP|RB|PRP(?![$])|PRN)/=det !$++ (/^PRP[$]|POS/ $++ =det !$++ (/''/ $++ =det)))",
bool det1(){

}

//"NP|NP-TMP|NP-ADV < (DT=target < /^(?i:either|neither|both)$/ !$+ DT !$++ CC $++ /^(?:NN|NX|NML)/ !$++ (NP < CC))",
bool det2(){

}

//"NP|NP-TMP|NP-ADV < (DT=target !< /^(?i:either|neither|both)$/ $++ CC $++ /^(?:NN|NX|NML)/)",
bool det3(){

}

//"NP|NP-TMP|NP-ADV < (DT=target $++ (/^JJ/ !$+ /^NN/) !$++CC !$+ DT)",
bool det4(){

}

//"NP|NP-TMP|NP-ADV <<, PRP <- (NP|DT|RB=target <<- all|both|each)", // we all, them all; various structures
bool det5(){

}

//"WHNP < (NP $-- (WHNP=target < WDT))",
bool det6(){

}

//"@WHNP|ADVP < (/^(?:NP|NN|CD|RBS)/ $-- DT|WDT|WP=target)"
bool det7(){

}
