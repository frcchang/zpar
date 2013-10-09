//"@PP|WHPP < (IN|VBG|VBN|TO $+ @SBAR|S|PP|ADVP=target)", // no intervening NP; VBN is for "compared with"
bool buildPComp1() {}

//"@PP|WHPP < (RB $+ @SBAR|S=target)", // RB is for weird tagging like "after/RB adjusting for inflation"
bool buildPComp2() {}

//"@PP|WHPP !< IN|TO < (SBAR=target <, (IN $+ S))",
bool buildPComp3() {}


