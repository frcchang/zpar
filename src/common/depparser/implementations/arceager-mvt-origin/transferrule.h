inline void transfer(const long & hpos, const long & mpos,const long & label, bool head_left, unsigned long & category){

	if(hpos==PENN_TAG_V && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_A && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_A && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_W && label==PENN_DEP_PUS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_W && label==PENN_DEP_PUS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_P && label==PENN_DEP_COO){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_A && label==PENN_DEP_COS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_R && label==PENN_DEP_RED){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_N && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_Q && label==PENN_DEP_COO){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_M && label==PENN_DEP_RAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_U && label==PENN_DEP_COO){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_W && label==PENN_DEP_PUS){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_S && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_A && label==PENN_DEP_RED){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_V && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_V && label==PENN_DEP_IS){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_NS && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_P && label==PENN_DEP_ADV){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_M && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_ISC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_V && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_COS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_P && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_N && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_V && label==PENN_DEP_DE){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_N && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_MT){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_A && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_R && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_R && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_A && label==PENN_DEP_DE){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_T && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_A && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_F && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NR && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_B && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_R && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_F && label==PENN_DEP_ISC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_T && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_O && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_ACT){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_F && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_V && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_V && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_T && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NS && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_R && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_V && label==PENN_DEP_ATT){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_NR && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_A && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_P && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_P && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_T && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_R && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_A && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_NZ && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_R && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_NS && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_R && label==PENN_DEP_ATT){

		category=PENN_CON_DP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_R && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_NR && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_NR && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NS && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_F && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_RAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Y && label==PENN_DEP_MT){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Y && label==PENN_DEP_RAD){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_A && label==PENN_DEP_COO){

		category=PENN_CON_VCD;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_NT && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_F && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_T && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_NS && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_M && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_Q && label==PENN_DEP_QUC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_A && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_S && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_NS && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_C && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_A && label==PENN_DEP_DI){

		category=PENN_CON_DVP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NR && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_T && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_M && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_J && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_V && label==PENN_DEP_COO){

		category=PENN_CON_UCP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_IS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_Q && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_S && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_T && label==PENN_DEP_ATT){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_NR && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_P && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_F && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_M && label==PENN_DEP_IS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_K && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_M && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_B && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_LAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NT && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_R && label==PENN_DEP_ATT){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_NRF && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_T && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_COO){

		category=PENN_CON_UCP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_R && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_A && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_NR && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_P && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_A && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_S && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NR && label==PENN_DEP_IS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_M && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_V && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NZ && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_V && label==PENN_DEP_DI){

		category=PENN_CON_DVP;

	}

	else if(hpos==PENN_TAG_S && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_S && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_J && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_A && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_D && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_N && label==PENN_DEP_VOB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_Z && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_D && label==PENN_DEP_DI){

		category=PENN_CON_DVP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_M && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Z && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_A && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_M && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_COO){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_Y && label==PENN_DEP_MT){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_L && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_I && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NZ && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_NT && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_R && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_T && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_R && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_F && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_B && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_J && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_P && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_U && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_N && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_L && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_P && mpos==PENN_TAG_Q && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_S && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_ISC){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_M && label==PENN_DEP_IS){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_P && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_A && label==PENN_DEP_VV){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_I && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_NS && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_B && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_L && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_I && mpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Z && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_NR && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_I && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_I && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_B && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_V && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_B && label==PENN_DEP_COO){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_F && label==PENN_DEP_COO){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_N && label==PENN_DEP_IOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_D && label==PENN_DEP_COO){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_ISC){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_M && mpos==PENN_TAG_Q && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_Z && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_NS && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_NS && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_NR && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_NZ && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_NT && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_NZ && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NRF && mpos==PENN_TAG_NR && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_R && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_S && mpos==PENN_TAG_S && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_T && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_V && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NRF && mpos==PENN_TAG_NRF && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_NX && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_NT && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_NX && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_NS && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && mpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_S && mpos==PENN_TAG_N && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_S && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_P && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Z && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_A && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_N && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_K && mpos==PENN_TAG_V && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_N && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_J && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_C && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_NT && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && mpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_J && mpos==PENN_TAG_U && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_V && label==PENN_DEP_DEI){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NT && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_NX && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_T && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_U && mpos==PENN_TAG_A && label==PENN_DEP_DEI){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_A && mpos==PENN_TAG_V && label==PENN_DEP_IS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_U && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_D && mpos==PENN_TAG_Y && label==PENN_DEP_MT){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_R && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_N && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_W && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_U && label==PENN_DEP_RAD){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_R && mpos==PENN_TAG_Y && label==PENN_DEP_MT){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_Y && label==PENN_DEP_RAD){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && mpos==PENN_TAG_V && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && mpos==PENN_TAG_Y && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && mpos==PENN_TAG_V && label==PENN_DEP_RED){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_F && mpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_LAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_COO){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_LAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_QUN){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_ADV){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_POB){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_LAD){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_I && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_IS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_ACT){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_S && label==PENN_DEP_ATT){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_ADV){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_O && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_COO){

		category=PENN_CON_UCP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_RAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_D && label==PENN_DEP_PUN){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_LAD){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_NT && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NRF && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_RAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_Z && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_ISC){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_QUC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_IS){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_IOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_RAD){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_MT){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && label==PENN_DEP_COO){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NZ && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_IS){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_PUS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_RAD){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_J && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_COO){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_PUN){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_S && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_MT){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_RED){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_ISC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NRF && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_QUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_PUS){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NZ && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_Z && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NX && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_COO){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_I && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_RAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_D && label==PENN_DEP_MT){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_LAD){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_PUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_COS){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_ADV){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_COO){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_COO){

		category=PENN_CON_UCP;

	}

	else if(hpos==PENN_TAG_NT && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_K && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_S && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_PUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_COS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_ISC){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_P && label==PENN_DEP_PUN){

		category=PENN_CON_PP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_ADV){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_RED){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_LAD){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_J && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_M && label==PENN_DEP_ATT){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && label==PENN_DEP_DE){

		category=PENN_CON_DNP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_NUM){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_LAD){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_ADV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_I && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && label==PENN_DEP_DI){

		category=PENN_CON_DVP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Z && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_VOB){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_D && label==PENN_DEP_LAD){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_L && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_COO){

		category=PENN_CON_LCP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_PUS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_U && label==PENN_DEP_DEI){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_VV){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NX && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_PUN){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NZ && label==PENN_DEP_COO){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NZ && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_Q && label==PENN_DEP_QUN){

		category=PENN_CON_QP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_IS){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_QUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_CMP){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_ISC){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_RED){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_Z && label==PENN_DEP_IC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_B && label==PENN_DEP_PUN){

		category=PENN_CON_ADJP;

	}

	else if(hpos==PENN_TAG_F && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_D && label==PENN_DEP_ADV){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_D && label==PENN_DEP_COO){

		category=PENN_CON_ADVP;

	}

	else if(hpos==PENN_TAG_Z && label==PENN_DEP_COO){

		category=PENN_CON_VP;

	}

	else if(hpos==PENN_TAG_S && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NT && label==PENN_DEP_ATT){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_ATT){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_A && label==PENN_DEP_TPC){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_NR && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_IS){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_ADV){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_LAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_T && label==PENN_DEP_PUN){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_APP){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_V && label==PENN_DEP_MT){

		category=PENN_CON_CP;

	}

	else if(hpos==PENN_TAG_R && label==PENN_DEP_SBV){

		category=PENN_CON_IP;

	}

	else if(hpos==PENN_TAG_N && label==PENN_DEP_NUM){

		category=PENN_CON_NP;

	}

	else if(hpos==PENN_TAG_NS && label==PENN_DEP_RAD){

		category=PENN_CON_NP;

	}

	else{

		category=PENN_CON_NONE;

	}

        if (head_left)
             category |= (1<<(std::max(PENN_TAG_COUNT_BITS, PENN_CON_COUNT_BITS)+1)); // head right.

}
