// Copyright (C) University of Oxford 2010

   const CTaggedWord<CTag, TAG_SEPARATOR> &head = m_lCache[head_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &dep = m_lCache[dep_index];

   const int left_index = head_index < dep_index ? head_index : dep_index ;
   const int right_index = head_index < dep_index ? dep_index : head_index ;

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
   static CTag between_tag ;
   static int between_tags, surrounding_tags_l_l, surrounding_tags_l_r, surrounding_tags_r_l, surrounding_tags_r_r;
   static int link_direction ;
   static int link_direction_encode ;
   static int link_distance_encode ;
   static bool nc_ll, nc_lr, nc_rl, nc_rr, nc_l_, nc_r_, nc__l, nc__r ;

   /*----------------------word unigram----------------------*/
   link_distance_encode = getLinkSizeAndDirection(head_index, dep_index) ;
   link_direction = getLinkDirection(head_index, dep_index) ; 
   link_direction_encode = head_index-dep_index>0 ? LINK_DIRECTION_HEAD_RIGHT : LINK_DIRECTION_HEAD_LEFT ;
   const CWord &head_word = head.word;
   const CWord &dep_word = dep.word;
   const CTag &head_tag = head.tag;
   const CTag &dep_tag = dep.tag;
   const CTaggedWord<CTag, TAG_SEPARATOR> &head_word_tag = static_cast<const CTaggedWord<CTag, TAG_SEPARATOR> &>(head);
   const CTaggedWord<CTag, TAG_SEPARATOR> head_word_nil(head_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> head_nil_tag(g_emptyWord, head_tag);
   const CTaggedWord<CTag, TAG_SEPARATOR> &dep_word_tag = static_cast<const CTaggedWord<CTag, TAG_SEPARATOR> &>(dep);
   const CTaggedWord<CTag, TAG_SEPARATOR> dep_word_nil(dep_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> dep_nil_tag(g_emptyWord, dep_tag);
   const CTag &left_tag = m_lCache[left_index].tag;
   const CTag &right_tag = m_lCache[right_index].tag;

   /*----------------------between and surrounding----------------------*/
   const CTag &head_tag_l = ( head_index > 0 ) ? m_lCache[ head_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &head_tag_r = ( head_index < m_lCache.size()-2 ) ? m_lCache[ head_index+1 ].tag : CTag::SENTENCE_END ;
   const CTag &dep_tag_l = ( dep_index > 0 ) ? m_lCache[ dep_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &dep_tag_r = ( dep_index < m_lCache.size()-2 ) ? m_lCache[ dep_index+1 ].tag : CTag::SENTENCE_END ;
   nc_ll = (link_distance_encode<-1||link_distance_encode>1) ;
   nc_lr = (link_distance_encode<0||link_distance_encode>2) ;
   nc_rl = (link_distance_encode>0||link_distance_encode<-2) ;
   nc_rr = (link_distance_encode<-1||link_distance_encode>1) ;
   nc_l_ = (link_distance_encode<0||link_distance_encode>1) ;
   nc_r_ = (link_distance_encode>0||link_distance_encode<-1) ;
   nc__l = (link_distance_encode<-1||link_distance_encode>0) ;
   nc__r = (link_distance_encode>1||link_distance_encode<0) ;

