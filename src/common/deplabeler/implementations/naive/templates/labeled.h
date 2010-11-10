// Copyright (C) University of Oxford 2010

#define getOrUpdateLabeledScoreTemplate(x)\
   retval += cast_weights->m_mapLabel.getOrUpdateScore( std::make_pair(label, x) , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(word_label_int, &head_word, &label, &x);\
   retval += cast_weights->m_mapHeadWordLabel.getOrUpdateScore( word_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(word_label_int, &dep_word, &label, &x);\
   retval += cast_weights->m_mapDepWordLabel.getOrUpdateScore( word_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(taggedword_label_int, &head_word_tag, &label, &x);\
   retval += cast_weights->m_mapHeadWordTagLabel.getOrUpdateScore( taggedword_label_int, m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(taggedword_label_int, &dep_word_tag, &label, &x);\
   retval += cast_weights->m_mapDepWordTagLabel.getOrUpdateScore( taggedword_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tag_label_int, &head_tag, &label, &x);\
   retval += cast_weights->m_mapHeadTagLabel.getOrUpdateScore( tag_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tag_label_int, &dep_tag, &label, &x);\
   retval += cast_weights->m_mapDepTagLabel.getOrUpdateScore( tag_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &head_tag_lm, &label, &x);\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int, m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &head_tag_mr, &label, &x);\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &head_tag_lmr, &label, &x);\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &dep_tag_lm, &label, &x);\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int , m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &dep_tag_mr, &label, &x);\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int, m_nScoreIndex , amount , round ) ;\
   refer_or_allocate_tuple3(tagset3_label_int, &dep_tag_lmr, &label, &x);\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( tagset3_label_int , m_nScoreIndex , amount , round ) ;

