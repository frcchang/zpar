
//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (VP=target < VBG|VBN|VBD $-- NP)",
bool partmod1(){

}

//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (/^,$/ $+ (VP=target <, VBG|VBN))",
bool partmod2(){

}

//"S|SINV < (S=target < (VP <, VBG|VBN) [ $- (/^,$/ [ $- @NP | $- (@PP $ @NP) ] ) | $+ (/^,$/ $+ @NP) ] )",
bool partmod3(){

}

//"(VP < (@S=target < (VP <1 VBG|VBN )$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)])))",
bool partmod4(){

}
