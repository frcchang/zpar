void transfer(const long & hpos, const long & mpos,const long & label,const long & category){
	if(hpos=="﻿v" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="b" && mpos=="a" && label=="IC"){
		category="IP";
	}
	else if(hpos=="b" && mpos=="a" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="w" && label=="PUS"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="w" && label=="PUS"){
		category="ADJP";
	}
	else if(hpos=="p" && mpos=="p" && label=="COO"){
		category="PP";
	}
	else if(hpos=="a" && mpos=="a" && label=="COS"){
		category="ADJP";
	}
	else if(hpos=="r" && mpos=="r" && label=="RED"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="n" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="q" && mpos=="q" && label=="COO"){
		category="QP";
	}
	else if(hpos=="q" && mpos=="m" && label=="RAD"){
		category="QP";
	}
	else if(hpos=="u" && mpos=="u" && label=="COO"){
		category="CP";
	}
	else if(hpos=="n" && mpos=="w" && label=="PUS"){
		category="IP";
	}
	else if(hpos=="p" && mpos=="c" && label=="LAD"){
		category="PP";
	}
	else if(hpos=="m" && mpos=="q" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="s" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="q" && mpos=="c" && label=="LAD"){
		category="QP";
	}
	else if(hpos=="q" && mpos=="q" && label=="QUN"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="a" && label=="RED"){
		category="ADJP";
	}
	else if(hpos=="l" && mpos=="v" && label=="COO"){
		category="VP";
	}
	else if(hpos=="q" && mpos=="v" && label=="IS"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="ns" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="d" && mpos=="p" && label=="ADV"){
		category="ADVP";
	}
	else if(hpos=="m" && mpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="m" && label=="IS"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="n" && label=="IS"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="n" && label=="ISC"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="n" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="t" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="r" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="r" && mpos=="v" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="n" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="d" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="v" && label=="COS"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="p" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="q" && mpos=="m" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="v" && mpos=="v" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="p" && mpos=="n" && label=="POB"){
		category="PP";
	}
	else if(hpos=="u" && mpos=="v" && label=="DE"){
		category="CP";
	}
	else if(hpos=="v" && mpos=="v" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="n" && label=="COO"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="n" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="v" && mpos=="u" && label=="MT"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="a" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="q" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="q" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="c" && label=="LAD"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="r" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="r" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && mpos=="m" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="v" && label=="VV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="v" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="f" && mpos=="n" && label=="ATT"){
		category="LCP";
	}
	else if(hpos=="u" && mpos=="a" && label=="DE"){
		category="CP";
	}
	else if(hpos=="a" && mpos=="d" && label=="ADV"){
		category="ADJP";
	}
	else if(hpos=="v" && mpos=="t" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="a" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="p" && mpos=="f" && label=="POB"){
		category="PP";
	}
	else if(hpos=="v" && mpos=="nr" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && mpos=="b" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="v" && label=="COO"){
		category="VP";
	}
	else if(hpos=="r" && mpos=="r" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="f" && label=="ISC"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="t" && label=="APP"){
		category="NP";
	}
	else if(hpos=="r" && mpos=="q" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="o" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="z" && mpos=="d" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="v" && label=="ACT"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="f" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="v" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="v" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="p" && mpos=="v" && label=="POB"){
		category="PP";
	}
	else if(hpos=="nr" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="t" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="q" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="ns" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="u" && mpos=="r" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="f" && mpos=="v" && label=="ATT"){
		category="LCP";
	}
	else if(hpos=="n" && mpos=="nr" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="a" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="u" && mpos=="p" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="v" && mpos=="p" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="t" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="r" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="a" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="u" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="nz" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="p" && mpos=="r" && label=="POB"){
		category="PP";
	}
	else if(hpos=="p" && mpos=="ns" && label=="POB"){
		category="PP";
	}
	else if(hpos=="q" && mpos=="r" && label=="ATT"){
		category="DP";
	}
	else if(hpos=="v" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="r" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="nr" && mpos=="nr" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="nr" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="n" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="ns" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="f" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="m" && mpos=="m" && label=="RAD"){
		category="QP";
	}
	else if(hpos=="v" && mpos=="y" && label=="MT"){
		category="CP";
	}
	else if(hpos=="v" && mpos=="y" && label=="RAD"){
		category="CP";
	}
	else if(hpos=="nr" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="a" && mpos=="a" && label=="COO"){
		category="VCD";
	}
	else if(hpos=="n" && mpos=="nt" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="f" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="p" && mpos=="t" && label=="POB"){
		category="PP";
	}
	else if(hpos=="u" && mpos=="ns" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="v" && mpos=="q" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="u" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="m" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="q" && label=="QUC"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="a" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="a" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="n" && label=="IS"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="s" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="ns" && label=="COO"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="c" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="a" && mpos=="c" && label=="LAD"){
		category="VP";
	}
	else if(hpos=="f" && mpos=="q" && label=="QUN"){
		category="LCP";
	}
	else if(hpos=="ns" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="a" && label=="DI"){
		category="DVP";
	}
	else if(hpos=="v" && mpos=="nr" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="u" && mpos=="t" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="v" && mpos=="m" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="j" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="v" && label=="COO"){
		category="UCP";
	}
	else if(hpos=="q" && mpos=="w" && label=="PUN"){
		category="QP";
	}
	else if(hpos=="v" && mpos=="v" && label=="IS"){
		category="VP";
	}
	else if(hpos=="u" && mpos=="q" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="p" && mpos=="s" && label=="POB"){
		category="PP";
	}
	else if(hpos=="f" && mpos=="t" && label=="ATT"){
		category="LCP";
	}
	else if(hpos=="u" && mpos=="nr" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="a" && mpos=="p" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="f" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="m" && label=="IS"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="k" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="m" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="f" && mpos=="w" && label=="PUN"){
		category="LCP";
	}
	else if(hpos=="u" && mpos=="b" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="v" && mpos=="u" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="u" && label=="LAD"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="nt" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="r" && label=="ATT"){
		category="IP";
	}
	else if(hpos=="l" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="nrf" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="t" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="v" && label=="COO"){
		category="UCP";
	}
	else if(hpos=="m" && mpos=="r" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="v" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="m" && mpos=="w" && label=="PUN"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="a" && label=="IC"){
		category="IP";
	}
	else if(hpos=="p" && mpos=="nr" && label=="POB"){
		category="PP";
	}
	else if(hpos=="n" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="u" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="nr" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="q" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="p" && mpos=="w" && label=="PUN"){
		category="PP";
	}
	else if(hpos=="n" && mpos=="p" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="d" && mpos=="d" && label=="ADV"){
		category="ADVP";
	}
	else if(hpos=="q" && mpos=="u" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="q" && mpos=="a" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="v" && mpos=="s" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="nr" && label=="IS"){
		category="VP";
	}
	else if(hpos=="q" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="u" && mpos=="m" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="nz" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="m" && mpos=="u" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="n" && mpos=="v" && label=="APP"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="nz" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="u" && mpos=="v" && label=="DI"){
		category="DVP";
	}
	else if(hpos=="s" && mpos=="n" && label=="ATT"){
		category="LCP";
	}
	else if(hpos=="v" && mpos=="s" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="j" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="a" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="p" && mpos=="a" && label=="POB"){
		category="PP";
	}
	else if(hpos=="v" && mpos=="q" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="nr" && mpos=="u" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="m" && mpos=="d" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="v" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="p" && mpos=="d" && label=="ADV"){
		category="PP";
	}
	else if(hpos=="p" && mpos=="n" && label=="VOB"){
		category="PP";
	}
	else if(hpos=="n" && mpos=="d" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="n" && mpos=="z" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="d" && label=="DI"){
		category="DVP";
	}
	else if(hpos=="n" && mpos=="m" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="z" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="a" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="ns" && mpos=="u" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="p" && mpos=="m" && label=="POB"){
		category="PP";
	}
	else if(hpos=="ns" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="m" && mpos=="m" && label=="COO"){
		category="QP";
	}
	else if(hpos=="f" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="b" && mpos=="w" && label=="PUN"){
		category="ADJP";
	}
	else if(hpos=="a" && mpos=="y" && label=="MT"){
		category="VP";
	}
	else if(hpos=="m" && mpos=="m" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="v" && mpos=="u" && label=="RAD"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="l" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="d" && mpos=="w" && label=="PUN"){
		category="ADVP";
	}
	else if(hpos=="i" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="nz" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="nt" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="z" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="r" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="t" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && mpos=="r" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="f" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="b" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="z" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="n" && label=="IC"){
		category="IP";
	}
	else if(hpos=="q" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="j" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="p" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="l" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="u" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="q" && label=="IC"){
		category="IP";
	}
	else if(hpos=="n" && mpos=="n" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="l" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="l" && label=="IC"){
		category="IP";
	}
	else if(hpos=="m" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="m" && mpos=="c" && label=="LAD"){
		category="QP";
	}
	else if(hpos=="p" && mpos=="q" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="v" && mpos=="s" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="m" && mpos=="m" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="m" && mpos=="m" && label=="ISC"){
		category="QP";
	}
	else if(hpos=="m" && mpos=="m" && label=="IS"){
		category="QP";
	}
	else if(hpos=="a" && mpos=="u" && label=="RAD"){
		category="ADJP";
	}
	else if(hpos=="a" && mpos=="p" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="a" && label=="VV"){
		category="ADJP";
	}
	else if(hpos=="b" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="i" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="ns" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="b" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="l" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="a" && mpos=="v" && label=="VV"){
		category="VP";
	}
	else if(hpos=="i" && mpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="z" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="a" && mpos=="nr" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="z" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="i" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="i" && label=="IC"){
		category="IP";
	}
	else if(hpos=="b" && mpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="b" && label=="IC"){
		category="IP";
	}
	else if(hpos=="r" && mpos=="v" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="b" && mpos=="b" && label=="COO"){
		category="ADJP";
	}
	else if(hpos=="b" && mpos=="c" && label=="LAD"){
		category="ADJP";
	}
	else if(hpos=="f" && mpos=="f" && label=="COO"){
		category="LCP";
	}
	else if(hpos=="f" && mpos=="c" && label=="LAD"){
		category="LCP";
	}
	else if(hpos=="r" && mpos=="n" && label=="IS"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="n" && label=="IOB"){
		category="VP";
	}
	else if(hpos=="m" && mpos=="d" && label=="ADV"){
		category="QP";
	}
	else if(hpos=="d" && mpos=="d" && label=="COO"){
		category="ADVP";
	}
	else if(hpos=="d" && mpos=="c" && label=="LAD"){
		category="ADVP";
	}
	else if(hpos=="v" && mpos=="v" && label=="ISC"){
		category="VP";
	}
	else if(hpos=="m" && mpos=="n" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="m" && mpos=="q" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="r" && mpos=="d" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="q" && mpos=="d" && label=="ADV"){
		category="QP";
	}
	else if(hpos=="r" && mpos=="u" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="b" && mpos=="d" && label=="ADV"){
		category="ADJP";
	}
	else if(hpos=="z" && mpos=="z" && label=="COO"){
		category="VP";
	}
	else if(hpos=="ns" && mpos=="ns" && label=="IS"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="ns" && label=="IS"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="nr" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nx" && mpos=="nz" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="n" && label=="IS"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="nt" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nx" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="nz" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nrf" && mpos=="nr" && label=="IS"){
		category="NP";
	}
	else if(hpos=="r" && mpos=="r" && label=="APP"){
		category="NP";
	}
	else if(hpos=="s" && mpos=="s" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="t" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="v" && label=="IS"){
		category="NP";
	}
	else if(hpos=="nx" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nrf" && mpos=="nrf" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nx" && mpos=="nx" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="nt" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="m" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="l" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="m" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="nx" && mpos=="nx" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="ns" && label=="IS"){
		category="NP";
	}
	else if(hpos=="nr" && mpos=="n" && label=="IS"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="q" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="s" && mpos=="n" && label=="COO"){
		category="NP";
	}
	else if(hpos=="s" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="q" && label=="QUN"){
		category="VP";
	}
	else if(hpos=="l" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="l" && mpos=="p" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="r" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="z" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="nt" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="a" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="r" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="n" && label=="COO"){
		category="NP";
	}
	else if(hpos=="k" && mpos=="v" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nz" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="n" && label=="COO"){
		category="NP";
	}
	else if(hpos=="l" && mpos=="d" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="j" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="j" && label=="COO"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="c" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="nt" && label=="COO"){
		category="NP";
	}
	else if(hpos=="nt" && mpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="ns" && mpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="j" && mpos=="u" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="v" && label=="DEI"){
		category="VP";
	}
	else if(hpos=="nt" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="a" && mpos=="m" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="nx" && mpos=="w" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="t" && label=="COO"){
		category="NP";
	}
	else if(hpos=="u" && mpos=="a" && label=="DEI"){
		category="ADJP";
	}
	else if(hpos=="a" && mpos=="v" && label=="IS"){
		category="ADJP";
	}
	else if(hpos=="r" && mpos=="u" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="d" && mpos=="y" && label=="MT"){
		category="ADVP";
	}
	else if(hpos=="r" && mpos=="r" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="r" && mpos=="n" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="v" && mpos=="w" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="t" && mpos=="u" && label=="RAD"){
		category="ADVP";
	}
	else if(hpos=="r" && mpos=="y" && label=="MT"){
		category="IP";
	}
	else if(hpos=="t" && mpos=="y" && label=="RAD"){
		category="IP";
	}
	else if(hpos=="t" && mpos=="d" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="t" && mpos=="v" && label=="VV"){
		category="VP";
	}
	else if(hpos=="n" && mpos=="y" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="v" && mpos=="v" && label=="RED"){
		category="VP";
	}
	else if(hpos=="f" && mpos=="d" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="m" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="m" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="nr" && label=="ISC"){
		category="NP";
	}
	else if(hpos=="s" && label=="ATT"){
		category="LCP";
	}
	else if(hpos=="r" && label=="RED"){
		category="VP";
	}
	else if(hpos=="r" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="a" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="f" && label=="LAD"){
		category="LCP";
	}
	else if(hpos=="a" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="﻿v" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="n" && label=="ISC"){
		category="NP";
	}
	else if(hpos=="q" && label=="LAD"){
		category="QP";
	}
	else if(hpos=="v" && label=="TPC"){
		category="IP";
	}
	else if(hpos=="nt" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="v" && label=="IOB"){
		category="VP";
	}
	else if(hpos=="t" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="d" && label=="MT"){
		category="ADVP";
	}
	else if(hpos=="p" && label=="PUN"){
		category="PP";
	}
	else if(hpos=="a" && label=="MT"){
		category="VP";
	}
	else if(hpos=="nr" && label=="IS"){
		category="NP";
	}
	else if(hpos=="m" && label=="APP"){
		category="NP";
	}
	else if(hpos=="nr" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="n" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="q" && label=="COO"){
		category="QP";
	}
	else if(hpos=="v" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="nt" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="p" && label=="ADV"){
		category="PP";
	}
	else if(hpos=="ns" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="a" && label=="RED"){
		category="ADJP";
	}
	else if(hpos=="v" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="t" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="a" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="k" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="z" && label=="IC"){
		category="IP";
	}
	else if(hpos=="t" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="b" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="b" && label=="IC"){
		category="IP";
	}
	else if(hpos=="ns" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="v" && label=="LAD"){
		category="VP";
	}
	else if(hpos=="nr" && label=="APP"){
		category="NP";
	}
	else if(hpos=="r" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="a" && label=="COS"){
		category="ADJP";
	}
	else if(hpos=="m" && label=="ISC"){
		category="QP";
	}
	else if(hpos=="m" && label=="LAD"){
		category="QP";
	}
	else if(hpos=="nt" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="r" && label=="IS"){
		category="NP";
	}
	else if(hpos=="n" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="v" && label=="COO"){
		category="VP";
	}
	else if(hpos=="a" && label=="RAD"){
		category="ADJP";
	}
	else if(hpos=="n" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="z" && label=="COO"){
		category="VP";
	}
	else if(hpos=="d" && label=="PUN"){
		category="ADVP";
	}
	else if(hpos=="nx" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="a" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="t" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nrf" && label=="IS"){
		category="NP";
	}
	else if(hpos=="m" && label=="COO"){
		category="QP";
	}
	else if(hpos=="nr" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="r" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="i" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="q" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="v" && label=="RED"){
		category="VP";
	}
	else if(hpos=="q" && label=="RAD"){
		category="QP";
	}
	else if(hpos=="n" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="n" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="p" && label=="LAD"){
		category="PP";
	}
	else if(hpos=="ns" && label=="IS"){
		category="NP";
	}
	else if(hpos=="m" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="l" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="v" && label=="RAD"){
		category="VP";
	}
	else if(hpos=="u" && label=="COO"){
		category="CP";
	}
	else if(hpos=="a" && label=="LAD"){
		category="VP";
	}
	else if(hpos=="m" && label=="ADV"){
		category="QP";
	}
	else if(hpos=="ns" && label=="APP"){
		category="NP";
	}
	else if(hpos=="n" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="nx" && label=="COO"){
		category="NP";
	}
	else if(hpos=="m" && label=="PUN"){
		category="QP";
	}
	else if(hpos=="n" && label=="COO"){
		category="UCP";
	}
	else if(hpos=="t" && label=="COO"){
		category="NP";
	}
	else if(hpos=="v" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="a" && label=="CMP"){
		category="VP";
	}
	else if(hpos=="a" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && label=="APP"){
		category="NP";
	}
	else if(hpos=="a" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="r" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="v" && label=="QUN"){
		category="VP";
	}
	else if(hpos=="l" && label=="IC"){
		category="IP";
	}
	else if(hpos=="u" && label=="DI"){
		category="DVP";
	}
	else if(hpos=="p" && label=="POB"){
		category="PP";
	}
	else if(hpos=="r" && label=="COO"){
		category="NP";
	}
	else if(hpos=="n" && label=="IS"){
		category="NP";
	}
	else if(hpos=="i" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="ns" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="m" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="m" && label=="RAD"){
		category="QP";
	}
	else if(hpos=="v" && label=="ISC"){
		category="VP";
	}
	else if(hpos=="nt" && label=="COO"){
		category="NP";
	}
	else if(hpos=="r" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="s" && label=="COO"){
		category="NP";
	}
	else if(hpos=="f" && label=="ADV"){
		category="NP";
	}
	else if(hpos=="d" && label=="ADV"){
		category="ADVP";
	}
	else if(hpos=="f" && label=="PUN"){
		category="LCP";
	}
	else if(hpos=="a" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="r" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="r" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="v" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="nr" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="b" && label=="COO"){
		category="ADJP";
	}
	else if(hpos=="n" && label=="IC"){
		category="IP";
	}
	else if(hpos=="nz" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="r" && label=="MT"){
		category="IP";
	}
	else if(hpos=="nx" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="b" && label=="LAD"){
		category="ADJP";
	}
	else if(hpos=="t" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="nr" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="z" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="s" && label=="LAD"){
		category="NP";
	}
	else if(hpos=="l" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="q" && label=="IC"){
		category="IP";
	}
	else if(hpos=="q" && label=="ATT"){
		category="QP";
	}
	else if(hpos=="b" && label=="ADV"){
		category="ADJP";
	}
	else if(hpos=="r" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="m" && label=="IS"){
		category="QP";
	}
	else if(hpos=="ns" && label=="COO"){
		category="NP";
	}
	else if(hpos=="n" && label=="PUS"){
		category="IP";
	}
	else if(hpos=="l" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="a" && label=="COO"){
		category="UCP";
	}
	else if(hpos=="nrf" && label=="COO"){
		category="NP";
	}
	else if(hpos=="f" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="nz" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="z" && label=="ADV"){
		category="VP";
	}
	else if(hpos=="q" && label=="ADV"){
		category="QP";
	}
	else if(hpos=="t" && label=="VV"){
		category="VP";
	}
	else if(hpos=="ns" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="v" && label=="ATT"){
		category="IP";
	}
	else if(hpos=="v" && label=="IC"){
		category="IP";
	}
	else if(hpos=="v" && label=="VV"){
		category="VP";
	}
	else if(hpos=="q" && label=="QUN"){
		category="QP";
	}
	else if(hpos=="l" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="j" && label=="COO"){
		category="NP";
	}
	else if(hpos=="z" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="a" && label=="IS"){
		category="ADJP";
	}
	else if(hpos=="l" && label=="COO"){
		category="VP";
	}
	else if(hpos=="v" && label=="COS"){
		category="VP";
	}
	else if(hpos=="u" && label=="DE"){
		category="DNP";
	}
	else if(hpos=="nz" && label=="IS"){
		category="NP";
	}
	else if(hpos=="r" && label=="APP"){
		category="NP";
	}
	else if(hpos=="f" && label=="QUN"){
		category="LCP";
	}
	else if(hpos=="d" && label=="LAD"){
		category="ADVP";
	}
	else if(hpos=="ns" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="p" && label=="COO"){
		category="PP";
	}
	else if(hpos=="s" && label=="PUN"){
		category="NP";
	}
	else if(hpos=="b" && label=="PUN"){
		category="ADJP";
	}
	else if(hpos=="q" && label=="IS"){
		category="QP";
	}
	else if(hpos=="ns" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="t" && label=="APP"){
		category="NP";
	}
	else if(hpos=="v" && label=="PUS"){
		category="VP";
	}
	else if(hpos=="v" && label=="IS"){
		category="VP";
	}
	else if(hpos=="f" && label=="COO"){
		category="LCP";
	}
	else if(hpos=="nr" && label=="COO"){
		category="NP";
	}
	else if(hpos=="u" && label=="DEI"){
		category="ADJP";
	}
	else if(hpos=="j" && label=="ATT"){
		category="NP";
	}
	else if(hpos=="a" && label=="VV"){
		category="VP";
	}
	else if(hpos=="n" && label=="APP"){
		category="NP";
	}
	else if(hpos=="n" && label=="NUM"){
		category="NP";
	}
	else if(hpos=="t" && label=="RAD"){
		category="IP";
	}
	else if(hpos=="o" && label=="SBV"){
		category="IP";
	}
	else if(hpos=="d" && label=="COO"){
		category="ADVP";
	}
	else if(hpos=="n" && label=="QUC"){
		category="NP";
	}
	else if(hpos=="v" && label=="ACT"){
		category="VP";
	}
	else if(hpos=="n" && label=="RAD"){
		category="NP";
	}
	else if(hpos=="q" && label=="NUM"){
		category="QP";
	}
	else if(hpos=="i" && label=="PUN"){
		category="VP";
	}
	else if(hpos=="v" && label=="MT"){
		category="CP";
	}
	else if(hpos=="t" && label=="QUN"){
		category="NP";
	}
	else if(hpos=="nz" && label=="COO"){
		category="NP";
	}
	else if(hpos=="p" && label=="VOB"){
		category="VP";
	}
	else if(hpos=="q" && label=="PUN"){
		category="QP";
	}
	else if(hpos=="a" && label=="PUS"){
		category="ADJP";
	}
	else if(hpos=="nr" && label=="RAD"){
		category="NP";
	}
	else{
		category="None";
	}
