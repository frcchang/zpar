
   static int index;
   static int temp_index;

   static int head_begin, head_end, dep_begin, dep_end;
   head_begin = item->getWordStart(head_index);
   head_end = item->getWordEnd(head_index);
   dep_begin = item->getWordStart(dep_index);
   dep_end = item->getWordEnd(dep_index);

   /*----------------------word unigram----------------------*/
   const CWord &head_word = (amount == 0) ? m_WordCache.find(head_begin, head_end, sentence) 
                                          : m_WordCache.replace(head_begin, head_end, sentence);
   const CWord &dep_word = (amount == 0) ? m_WordCache.find(dep_begin, dep_end, sentence)
                                         : m_WordCache.replace(dep_begin, dep_end, sentence);
   const CTag head_tag = item->getTag(head_index);
   const CTag dep_tag = item->getTag(dep_index);
   const CTaggedWord<CTag> head_word_tag(head_word, head_tag);
   const CTaggedWord<CTag> dep_word_tag(dep_word, dep_tag);
   const CTaggedWord<CTag> head_word_nil(head_word, PENN_TAG_NONE);
   const CTaggedWord<CTag> head_nil_tag(g_emptyWord, head_tag);
   const CTaggedWord<CTag> dep_word_nil(dep_word, PENN_TAG_NONE);
   const CTaggedWord<CTag> dep_nil_tag(g_emptyWord, dep_tag);

   static int left_index; 
   static int right_index; 
   left_index = head_index<dep_index ? head_index : dep_index;
   right_index = head_index<dep_index ? dep_index : head_index;
   // it is the last word in item only when the characters in the sentence are processed
   assert( right_index+1<item->size() || item->charactersize() == sentence->size() );

   const CTag &left_tag = item->getTag(left_index);
   const CTag &right_tag = item->getTag(right_index);

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
   both_sides = (head_tag.code()<<PENN_TAG_COUNT_BITS*2) + 0 + dep_tag.code(); 
   static unsigned long seen_tags;
   static int between_tags, between_tag;
   // note that it is safe to use head_index+1 < item->size() as the condition for PENN_TAG_END 
   const unsigned long &head_tag_l = ( head_index > 0 ) ? item->getTag( head_index-1 ) : PENN_TAG_BEGIN ;
   const unsigned long &head_tag_r = ( head_index+1 < item->size() ) ? item->getTag( head_index+1 ) : PENN_TAG_END ;
   const unsigned long &dep_tag_l = ( dep_index > 0 ) ? item->getTag( dep_index-1 ) : PENN_TAG_BEGIN ;
   const unsigned long &dep_tag_r = ( dep_index+1 < item->size() ) ? item->getTag( dep_index+1 ) : PENN_TAG_END ;
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
   sibling_index = item->sibling(dep_index);
   static int sibling_start, sibling_end;
   sibling_start = (sibling_index<0) ? -1 : static_cast<int>(item->getWordStart(sibling_index));
   sibling_end = (sibling_index<0) ? -1 : static_cast<int>(item->getWordEnd(sibling_index));

   const CWord &sibling_word = (sibling_start==-1) ? g_emptyWord 
                                                   : ((amount == 0) ? m_WordCache.find(sibling_start, sibling_end, sentence)
                                                                   : m_WordCache.replace(sibling_start, sibling_end, sentence));
   const CTag &sibling_tag = (sibling_start==-1) ? PENN_TAG_NONE : item->getTag(sibling_index);
   const CTaggedWord<CTag> sibling(sibling_word, sibling_tag);

   const CTaggedWord<CTag> sibling_word_tag(sibling_word, dep_tag);
   const CTaggedWord<CTag> sibling_tag_word(dep_word, sibling_tag);

   static CTwoWords sibling_words ;

   static int sibling_distance_encode ;
   static int sibling_direction_encode ;
   if (sibling_index != DEPENDENCY_LINK_NO_HEAD) {
      sibling_distance_encode = getLinkSizeAndDirection(sibling_index, dep_index);
      sibling_direction_encode = sibling_index-dep_index>0 ? LINK_DIRECTION_HEAD_RIGHT : LINK_DIRECTION_HEAD_LEFT ;
   }

