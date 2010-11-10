// Copyright (C) University of Oxford 2010

   /*----------------------word bigram----------------------*/
   head_word_tag_dep_word_tag.allocate( head_word_tag, dep_word_tag ) ;
   head_word_tag_dep_word.allocate( head_word_tag, dep_word_nil ) ;
   head_word_dep_word_tag.allocate( head_word_nil, dep_word_tag ) ;
   head_word_tag_dep_tag.allocate( head_word_tag, dep_nil_tag ) ;
   head_tag_dep_word_tag.allocate( head_nil_tag, dep_word_tag ) ;
   head_word_dep_word.allocate( head_word, dep_word ) ;

#define updateUnigrams(x) \
   cast_weights->m_mapHeadWordTag[ std::make_pair(head_word_tag, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepWordTag[ std::make_pair(dep_word_tag, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWord[ std::make_pair(head_word, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepWord[ std::make_pair(dep_word, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTag[ std::make_pair(head_tag.code(), x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepTag[ std::make_pair(dep_tag.code(), x) ].updateCurrent( amount , round ) ;

#define updateBigrams(x) \
   cast_weights->m_mapHeadWordTagDepWordTag[ std::make_pair(head_word_tag_dep_word_tag,x) ].updateCurrent( amount , round ); \
   cast_weights->m_mapHeadWordTagDepWord[ std::make_pair(head_word_tag_dep_word,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordDepWordTag[ std::make_pair(head_word_dep_word_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordTagDepTag[ std::make_pair(head_word_tag_dep_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTagDepWordTag[ std::make_pair(head_tag_dep_word_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordDepWord[ std::make_pair(head_word_dep_word,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTagDepTag[ std::make_pair((head_tag.code()<<CTag::SIZE)+dep_tag.code(),x) ].updateCurrent( amount , round ) ;

#define updateSurrounding(x) \
   both_sides = (head_tag.code()<<CTag::SIZE*2) + 0 + dep_tag.code() ; \
   for ( index=left_index+1; index<right_index; index++ ) { \
      between_tags = both_sides + (m_lCache[index].tag.code()<<CTag::SIZE) ; \
      cast_weights->m_mapBetweenTags[ std::make_pair(between_tags, x) ].updateCurrent( amount , round ) ; \
   } \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ std::make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ std::make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ std::make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ std::make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ std::make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc__l) cast_weights->m_mapSurroundingTagsLL[ std::make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc__r) cast_weights->m_mapSurroundingTagsLR[ std::make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_l_) cast_weights->m_mapSurroundingTagsLR[ std::make_pair(encodeTags(head_tag, head_tag_l, dep_tag, CTag(CTag::NONE)), x) ].updateCurrent( amount , round ) ; \
   if (nc_r_) cast_weights->m_mapSurroundingTagsRR[ std::make_pair(encodeTags(head_tag, head_tag_r, dep_tag, CTag(CTag::NONE)), x) ].updateCurrent( amount , round ) ;

