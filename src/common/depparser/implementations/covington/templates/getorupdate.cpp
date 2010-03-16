
   /*----------------------word bigram----------------------*/
   if ( amount == 0 ) {
      head_word_tag_dep_word_tag.refer( &head_word_tag, &dep_word_tag ) ;
      head_word_tag_dep_word.refer( &head_word_tag, &dep_word_nil ) ;
      head_word_dep_word_tag.refer( &head_word_nil, &dep_word_tag ) ;
      head_word_tag_dep_tag.refer( &head_word_tag, &dep_nil_tag ) ;
      head_tag_dep_word_tag.refer( &head_nil_tag, &dep_word_tag ) ;
      head_word_dep_word.refer( &head_word, &dep_word ) ;
   }
   else {
      head_word_tag_dep_word_tag.allocate( head_word_tag, dep_word_tag ) ;
      head_word_tag_dep_word.allocate( head_word_tag, dep_word_nil ) ;
      head_word_dep_word_tag.allocate( head_word_nil, dep_word_tag ) ;
      head_word_tag_dep_tag.allocate( head_word_tag, dep_nil_tag ) ;
      head_tag_dep_word_tag.allocate( head_nil_tag, dep_word_tag ) ;
      head_word_dep_word.allocate( head_word, dep_word ) ;
   }

   /*----------------------sibling----------------------*/
   if ( sibling_index != DEPENDENCY_LINK_NO_HEAD ) {
      if ( amount == 0 ) {
         sibling_words.refer( &sibling_word, &dep_word ) ;
      }
      else {
         sibling_words.allocate( sibling_word, dep_word ) ;
      }
   }

#define getOrUpdateUnigramScoreTemplate(x) \
   retval += cast_weights->m_mapHeadWordTag.getOrUpdateScore( make_pair(head_word_tag, x), m_nScoreIndex, amount, round) ;\
   retval += cast_weights->m_mapDepWordTag.getOrUpdateScore( make_pair(dep_word_tag, x), m_nScoreIndex, amount, round) ;\
   retval += cast_weights->m_mapHeadWord.getOrUpdateScore( make_pair(head_word, x), m_nScoreIndex, amount, round) ;\
   retval += cast_weights->m_mapDepWord.getOrUpdateScore( make_pair(dep_word, x), m_nScoreIndex, amount, round ) ;\
   retval += cast_weights->m_mapHeadTag.getOrUpdateScore( make_pair(head_tag.code(), x), m_nScoreIndex, amount, round ) ;\
   retval += cast_weights->m_mapDepTag.getOrUpdateScore( make_pair(dep_tag.code(), x), m_nScoreIndex, amount, round ) ;

#define getOrUpdateBigramScoreTemplate(x) \
   retval += cast_weights->m_mapHeadWordTagDepWordTag.getOrUpdateScore( make_pair(head_word_tag_dep_word_tag,x), m_nScoreIndex, amount, round ); \
   retval += cast_weights->m_mapHeadWordTagDepWord.getOrUpdateScore( make_pair(head_word_tag_dep_word,x), m_nScoreIndex, amount, round ) ; \
   retval += cast_weights->m_mapHeadWordDepWordTag.getOrUpdateScore( make_pair(head_word_dep_word_tag,x), m_nScoreIndex, amount, round ) ; \
   retval += cast_weights->m_mapHeadWordTagDepTag.getOrUpdateScore( make_pair(head_word_tag_dep_tag,x), m_nScoreIndex, amount, round ) ; \
   retval += cast_weights->m_mapHeadTagDepWordTag.getOrUpdateScore( make_pair(head_tag_dep_word_tag,x), m_nScoreIndex, amount, round ) ; \
   retval += cast_weights->m_mapHeadWordDepWord.getOrUpdateScore( make_pair(head_word_dep_word,x), m_nScoreIndex, amount, round ) ; \
   retval += cast_weights->m_mapHeadTagDepTag.getOrUpdateScore( make_pair(encodeTags(head_tag,dep_tag),x), m_nScoreIndex, amount, round ) ; 

#define getOrUpdateSurroundingScoreTemplate(x) \
   for ( index=left_index+1; index<right_index; index++ ) { \
      between_tags = both_sides + (m_lCache[index].tag.code()<<PENN_TAG_COUNT_BITS); \
      retval += cast_weights->m_mapBetweenTags.getOrUpdateScore( make_pair(between_tags, x), m_nScoreIndex, amount, round ) ; \
   } \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.getOrUpdateScore( make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.getOrUpdateScore( make_pair(encodeTags(CTag(CTag::NONE), head_tag_l, dep_tag, dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.getOrUpdateScore( make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.getOrUpdateScore( make_pair(encodeTags(CTag(CTag::NONE), head_tag_r, dep_tag, dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_ll) retval += cast_weights->m_mapSurroundingTagsLL.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_lr) retval += cast_weights->m_mapSurroundingTagsLR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_l, CTag(CTag::NONE), dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rl) retval += cast_weights->m_mapSurroundingTagsRL.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc_rr) retval += cast_weights->m_mapSurroundingTagsRR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_r, CTag(CTag::NONE), dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc__l) retval += cast_weights->m_mapSurroundingTagsLL.getOrUpdateScore( make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_l), x), m_nScoreIndex, amount, round ) ; \
   if (nc__r) retval += cast_weights->m_mapSurroundingTagsLR.getOrUpdateScore( make_pair(encodeTags(head_tag, CTag(CTag::NONE), dep_tag, dep_tag_r), x), m_nScoreIndex, amount, round ) ; \
   if (nc_l_) retval += cast_weights->m_mapSurroundingTagsLR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_l, dep_tag, CTag(CTag::NONE)), x), m_nScoreIndex, amount, round ) ; \
   if (nc_r_) retval += cast_weights->m_mapSurroundingTagsRR.getOrUpdateScore( make_pair(encodeTags(head_tag, head_tag_r, dep_tag, CTag(CTag::NONE)), x), m_nScoreIndex, amount, round ) ; 

#define getOrUpdateSiblingScoreTemplate(x) \
   if ( sibling_index != DEPENDENCY_LINK_NO_HEAD ) { \
      retval += cast_weights->m_mapSiblingWords.getOrUpdateScore( make_pair(sibling_words,x), m_nScoreIndex, amount, round ) ;\
      retval += cast_weights->m_mapSiblingWordTag.getOrUpdateScore( make_pair(sibling_word_tag,x), m_nScoreIndex, amount, round ) ;\
      retval += cast_weights->m_mapSiblingTagWord.getOrUpdateScore( make_pair(sibling_tag_word,x), m_nScoreIndex, amount, round ) ;\
      retval += cast_weights->m_mapSiblingTags.getOrUpdateScore( make_pair(encodeTags(dep_tag,sibling_tag),x), m_nScoreIndex, amount, round ) ;\
      between_tags = both_sides + (sibling_tag.code()<<PENN_TAG_COUNT_BITS);\
      retval += cast_weights->m_mapSiblingAndParentTags.getOrUpdateScore( make_pair(between_tags,x), m_nScoreIndex, amount, round ) ;\
   }   

