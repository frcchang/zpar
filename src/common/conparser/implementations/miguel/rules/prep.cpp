//"/^(?:(?:WH)?(?:NP|ADJP|ADVP|NX|NML)(?:-TMP|-ADV)?|VP|NAC|SQ|FRAG|PRN|X)$/ < (WHPP|WHPP-TMP|PP|PP-TMP=target !$- (@CC|CONJP $- __))",
bool prep1(){

}

//"WHPP|WHPP-TMP|WHPP-ADV|PP|PP-TMP|PP-ADV < (WHPP|WHPP-TMP|WHPP-ADV|PP|PP-TMP|PP-ADV=target !$- IN|VBG|VBN|TO) !< @CC|CONJP",
bool prep2(){

}

//"S|SINV < (PP|PP-TMP=target !< SBAR) < VP|S",
bool prep3(){

}

//"SBAR|SBARQ < /^(?:WH)?PP/=target < S|SQ"
bool prep4(){

}

