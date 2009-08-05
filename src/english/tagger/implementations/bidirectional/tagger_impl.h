/***************************************************************
 *
 * The tagger's agenda n chart implementation 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_AGENDACHART_IMPL_H
#define TAGGER_AGENDACHART_IMPL_H

typedef double SCORE_TYPE ;

//
// Specific global
//
const int AGENDA_SIZE = 16;
//
// General definitions for any method tagger.
//
const int MAX_SENTENCE_SIZE = 512; // restrict size of sentences 

const int LENGTH_MAX_BITS = 4;
const int LENGTH_MAX = 1<<LENGTH_MAX_BITS;

class CStateItem;

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTaggerImpl {
   protected:
      CAgendaChart<CStateItem> m_Chart;
   public:
      CTaggerImpl() : m_Chart(AGENDA_SIZE) { }
      virtual ~CTaggerImpl() { }
};

#endif
