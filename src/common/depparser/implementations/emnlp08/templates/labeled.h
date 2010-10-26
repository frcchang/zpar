// Copyright (C) University of Oxford 2010
#define getLabelAndDirectionOrDistanceEncode(label, dod) ((dod<<CDependencyLabel::SIZE)|label)

#define getOrUpdateLabeledScoreTemplate(x)\
   retval += cast_weights->m_mapLabel.getOrUpdateScore( x , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadWordLabel.getOrUpdateScore( make_pair(head_word, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepWordLabel.getOrUpdateScore( make_pair(dep_word, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadWordTagLabel.getOrUpdateScore( make_pair(head_word_tag, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepWordTagLabel.getOrUpdateScore( make_pair(dep_word_tag, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadTagLabel.getOrUpdateScore( make_pair(head_tag.code(), x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepTagLabel.getOrUpdateScore( make_pair(dep_tag.code(), x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( make_pair(head_tag_lm, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( make_pair(head_tag_mr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( make_pair(head_tag_lmr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( make_pair(dep_tag_lm, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( make_pair(dep_tag_mr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( make_pair(dep_tag_lmr, x) , m_nScoreIndex , amount , round ) ;

