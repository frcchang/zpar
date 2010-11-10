// Copyright (C) University of Oxford 2010
#define getLabelAndDirectionOrDistanceEncode(label, dod) ((dod<<CDependencyLabel::SIZE)|label)

#define getOrUpdateLabeledScoreTemplate(x)\
   retval += cast_weights->m_mapLabel.getOrUpdateScore( x , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadWordLabel.getOrUpdateScore( std::make_pair(head_word, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepWordLabel.getOrUpdateScore( std::make_pair(dep_word, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadWordTagLabel.getOrUpdateScore( std::make_pair(head_word_tag, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepWordTagLabel.getOrUpdateScore( std::make_pair(dep_word_tag, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadTagLabel.getOrUpdateScore( std::make_pair(head_tag.code(), x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepTagLabel.getOrUpdateScore( std::make_pair(dep_tag.code(), x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( std::make_pair(head_tag_lm, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( std::make_pair(head_tag_mr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapHeadSurroundingTagsLabel.getOrUpdateScore( std::make_pair(head_tag_lmr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( std::make_pair(dep_tag_lm, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( std::make_pair(dep_tag_mr, x) , m_nScoreIndex , amount , round ) ;\
   retval += cast_weights->m_mapDepSurroundingTagsLabel.getOrUpdateScore( std::make_pair(dep_tag_lmr, x) , m_nScoreIndex , amount , round ) ;

