//conj rules. There are
      //(stf comment) this is more ugly, but the first 3 patterns are now duplicated and for clausal things, that daughter to the left of the CC/CONJP can't be a PP or RB or ADVP either
      //(stf comment) non-parenthetical or comma in suitable phrase with conjunction to left

//"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
bool buildConj1(){}

// non-parenthetical or comma in suitable phrase with conj then adverb to left
//"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
bool buildConj2(){}
      // content phrase to the right of a comma or a parenthetical
      // The test at the end is to make sure that a conjunction or
      // comma etc actually show up between the target of the conj

      // dependency and the head of the phrase.  Otherwise, a
      // different relationship is probably more appropriate.
//"VP|S|SBAR|SBARQ|SINV|SQ=root < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ (/^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target $-- (/^CC|CONJP|:|,$/ $-- (__ ># =root))) )",
bool buildConj3(){}

	// non-parenthetical or comma in suitable phrase with conjunction to left
//"/^(?:ADJP|JJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
bool buildConj4(){}

//"/^(?:VP|S|SBAR|SBARQ|SINV|ADJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC $++ (CC|CONJP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
bool buildConj5(){}

//"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
      // content phrase to the right of a comma or a parenthetical
bool buildConj6(){}

//"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ /^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target)",
                // content phrase to the left of a comma for at least NX
bool buildConj7(){}

//"NX|NML < (CC|CONJP $- __) < (/^,$/ $- /^(?:A|N|V|PP|PRP|J|W|R|S)/=target)",
                // to take the conjunct in a preconjunct structure "either X or Y"
bool buildConj8(){}


