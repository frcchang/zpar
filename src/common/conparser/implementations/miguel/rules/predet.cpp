
//"/^(?:(?:WH)?NP(?:-TMP|-ADV)?|NX|NAC|NML)$/ < (PDT|DT=target $+ /^(?:DT|WP\\$|PRP\\$)$/ $++ /^N[NXM]/ !$++ CC)",
bool predet1(){

}

//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (PDT|DT=target $+ DT $++ (/^JJ/ !$+ /^NN/)) !$++ CC",
bool predet2(){

}

//"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < PDT=target <- DT"
bool predet3(){

}
