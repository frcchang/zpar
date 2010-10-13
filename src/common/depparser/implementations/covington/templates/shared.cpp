// Copyright (C) University of Oxford 2010

   static int index;
   static int temp_index;

   /*----------------------word unigram----------------------*/
   const CTaggedWord<CTag, TAG_SEPARATOR> &head_word_tag = m_lCache[head_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &dep_word_tag = m_lCache[dep_index];
   const CWord &head_word = static_cast<const CWord&>(head_word_tag);
   const CWord &dep_word = static_cast<const CWord&>(dep_word_tag);
   const CTag &head_tag = head_word_tag.tag;
   const CTag &dep_tag = dep_word_tag.tag;
   const CTaggedWord<CTag, TAG_SEPARATOR> head_word_nil(head_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> head_nil_tag(g_emptyWord, head_tag);
   const CTaggedWord<CTag, TAG_SEPARATOR> dep_word_nil(dep_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> dep_nil_tag(g_emptyWord, dep_tag);

   static int left_index; 
   static int right_index; 
   left_index = head_index<dep_index ? head_index : dep_index;
   right_index = head_index<dep_index ? dep_index : head_index;
   const CTag &left_tag = m_lCache[left_index].tag;
   const CTag &right_tag = m_lCache[right_index].tag;

   /*----------------------word bigram----------------------*/
   static CTwoTaggedWords head_word_tag_dep_word_tag ;
   static CTwoTaggedWords head_word_tag_dep_word ;
   static CTwoTaggedWords head_word_dep_word_tag ;
   static CTwoTaggedWords head_word_tag_dep_tag ;
   static CTwoTaggedWords head_tag_dep_word_tag ;
   static CTwoTaggedWords head_firstchar_tag_dep_word_tag ;
   static CTwoTaggedWords head_lastchar_tag_dep_word_tag ;
   static CTwoTaggedWords head_word_tag_dep_firstchar_tag ;
   static CTwoTaggedWords head_word_tag_dep_lastchar_tag ;
   static CTwoWords head_word_dep_word ;

   static int link_direction ;
   static int link_direction_encode ;
   static int link_distance_encode ;
   link_distance_encode = getLinkSizeAndDirection(head_index, dep_index) ;
   link_direction = getLinkDirection(head_index, dep_index) ; 
   link_direction_encode = getLinkDirectionEncode(head_index, dep_index);

   /*----------------------between and surrounding----------------------*/
   static int both_sides; // cache the sum of two tags
   both_sides = (head_tag.code()<<CTag::SIZE*2) + 0 + dep_tag.code(); 
   static unsigned long seen_tags;
   static int between_tags, between_tag;
   const CTag &head_tag_l = ( head_index > 0 ) ? m_lCache[ head_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &head_tag_r = ( head_index+1 < m_lCache.size() ) ? m_lCache[ head_index+1 ].tag : CTag::SENTENCE_END ;
   const CTag &dep_tag_l = ( dep_index > 0 ) ? m_lCache[ dep_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &dep_tag_r = ( dep_index+1 < m_lCache.size() ) ? m_lCache[ dep_index+1 ].tag : CTag::SENTENCE_END ;
   static bool nc_ll, nc_lr, nc_rl, nc_rr, nc_l_, nc_r_, nc__l, nc__r ;
   nc_ll = (link_distance_encode<-1||link_distance_encode>1) ;
   nc_lr = (link_distance_encode<0||link_distance_encode>2) ;
   nc_rl = (link_distance_encode>0||link_distance_encode<-2) ;
   nc_rr = (link_distance_encode<-1||link_distance_encode>1) ;
   nc_l_ = (link_distance_encode<0||link_distance_encode>1) ;
   nc_r_ = (link_distance_encode>0||link_distance_encode<-1) ;
   nc__l = (link_distance_encode<-1||link_distance_encode>0) ;
   nc__r = (link_distance_encode>1||link_distance_encode<0) ;

   /*----------------------sibling----------------------*/
   static int sibling_index;
   static int next_sibling_index;
   sibling_index = item->sibling(dep_index);
   next_sibling_index = ( sibling_index==DEPENDENCY_LINK_NO_HEAD ? DEPENDENCY_LINK_NO_HEAD : item->sibling(sibling_index) );

   const CTaggedWord<CTag, TAG_SEPARATOR> &sibling = sibling_index==DEPENDENCY_LINK_NO_HEAD ? g_emptyTaggedWord : m_lCache[sibling_index] ;
   const CTaggedWord<CTag, TAG_SEPARATOR> &next_sibling = next_sibling_index==DEPENDENCY_LINK_NO_HEAD ? g_emptyTaggedWord : m_lCache[sibling_index] ;

   const CWord &sibling_word = static_cast<const CWord&>(sibling);

   const CTag &sibling_tag = sibling.tag;
   const CTag &next_sibling_tag = next_sibling.tag;

   const CTaggedWord<CTag, TAG_SEPARATOR> sibling_word_tag(sibling_word, dep_tag);
   const CTaggedWord<CTag, TAG_SEPARATOR> sibling_tag_word(dep_word, sibling_tag);

   static CTwoWords sibling_words ;

   static int sibling_distance_encode ;
   static int sibling_direction_encode ;
   if (sibling_index != DEPENDENCY_LINK_NO_HEAD) {
      sibling_distance_encode = getLinkSizeAndDirection(sibling_index, dep_index);
      sibling_direction_encode = sibling_index-dep_index>0 ? LINK_DIRECTION_HEAD_RIGHT : LINK_DIRECTION_HEAD_LEFT ;
   }

   /*----------------------two link---------------------*/
   static int dep_lmd_index;
   static int dep_rmd_index;
   dep_lmd_index = item->leftmostdep( dep_index );
   dep_rmd_index = item->rightmostdep( dep_index );
   static unsigned long dep_lmd_tag;
   static unsigned long dep_rmd_tag;
   dep_lmd_tag = (dep_lmd_index == DEPENDENCY_LINK_NO_HEAD) ? CTag::NONE : m_lCache[dep_lmd_index].tag.code();
   dep_rmd_tag = (dep_rmd_index == DEPENDENCY_LINK_NO_HEAD) ? CTag::NONE : m_lCache[dep_rmd_index].tag.code();

   /*---------------------next word and tag--------------*/
   static int next_index;
   next_index = right_index+1<m_lCache.size() ? right_index+1 : -1;
   const CTaggedWord<CTag, TAG_SEPARATOR> &next_word_tag = (next_index == -1) ? g_emptyTaggedWord : m_lCache[next_index] ;
   const CWord &next_word = static_cast<const CWord&>(next_word_tag);
   const CTag &next_tag = next_word_tag.tag;
   const CTaggedWord<CTag, TAG_SEPARATOR> next_word_nil(next_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> next_nil_tag(g_emptyWord, next_tag);

   static CTwoTaggedWords dep_word_tag_next_word_tag;
   static CTwoTaggedWords dep_word_tag_next_tag;
   static CTwoTaggedWords dep_tag_next_word_tag;
   static CTwoTaggedWords dep_word_tag_next_word;
   static CTwoTaggedWords dep_word_next_word_tag;
   static CTwoWords dep_word_next_word;

