// Copyright (C) University of Oxford 2010

   /*----------------------word bigram----------------------*/
   head_word_tag_dep_word_tag.refer( &head_word_tag, &dep_word_tag ) ;
   head_word_tag_dep_word.refer( &head_word_tag, &dep_word_nil ) ;
   head_word_dep_word_tag.refer( &head_word_nil, &dep_word_tag ) ;
   head_word_tag_dep_tag.refer( &head_word_tag, &dep_nil_tag ) ;
   head_tag_dep_word_tag.refer( &head_nil_tag, &dep_word_tag ) ;
   head_word_dep_word.refer( &head_word, &dep_word ) ;

#define getUnigramScoreTemplate(x) \
   retval += cast_weights->m_mapHeadWordTag.find( std::make_pair(head_word_tag, x) , g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapDepWordTag.find( std::make_pair(dep_word_tag, x) , g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadWord.find( std::make_pair(head_word, x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapDepWord.find( std::make_pair(dep_word, x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadTag.find( std::make_pair(head_tag.code(), x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapDepTag.find( std::make_pair(dep_tag.code(), x), g_zeroScore ).score(m_nScoreIndex) ;

#define getBigramScoreTemplate(x) \
   retval += cast_weights->m_mapHeadWordTagDepWordTag.find( std::make_pair(head_word_tag_dep_word_tag,x), g_zeroScore ).score(m_nScoreIndex); \
   retval += cast_weights->m_mapHeadWordTagDepWord.find( std::make_pair(head_word_tag_dep_word,x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadWordDepWordTag.find( std::make_pair(head_word_dep_word_tag,x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadWordTagDepTag.find( std::make_pair(head_word_tag_dep_tag,x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadTagDepWordTag.find( std::make_pair(head_tag_dep_word_tag,x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadWordDepWord.find( std::make_pair(head_word_dep_word,x), g_zeroScore ).score(m_nScoreIndex) ; \
   retval += cast_weights->m_mapHeadTagDepTag.find( std::make_pair((head_tag.code()<<CTag::SIZE)+dep_tag.code(),x) , g_zeroScore ).score(m_nScoreIndex) ; 

#define getSurroundingScoreTemplate(x) \
   both_sides = (head_tag.code()<<CTag::SIZE*2) + 0 + dep_tag.code(); \
   for ( index=left_index+1; index<right_index; index++ ) { \
      between_tags = both_sides + (m_lCache[index].tag.code()<<CTag::SIZE); \
      retval += cast_weights->m_mapBetweenTags.find( std::make_pair(between_tags, x), g_zeroScore ).score(m_nScoreIndex) ; \
   } \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.find( std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.find( std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.find( std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.find( std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.find( std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.find( std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.find( std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.find( std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.find( std::make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.find( std::make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.find( std::make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.find( std::make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc__l) retval += cast_weights->m_mapSurroundingTagsLL.find( std::make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_l), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc__r) retval += cast_weights->m_mapSurroundingTagsLR.find( std::make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_r), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_l_) retval += cast_weights->m_mapSurroundingTagsLR.find( std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, CTag(CTag::NONE)), x), g_zeroScore ).score(m_nScoreIndex) ; \
   if (nc_r_) retval += cast_weights->m_mapSurroundingTagsRR.find( std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, CTag(CTag::NONE)), x), g_zeroScore ).score(m_nScoreIndex) ; 

