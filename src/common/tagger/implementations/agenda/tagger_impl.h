/***************************************************************
 *
 * The tagger's agenda implementation on segmented 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_SEGMENTED_IMPL_H
#define TAGGER_SEGMENTED_IMPL_H 1

typedef double SCORE_TYPE ;

//
// Specific global
//
const int AGENDA_SIZE = 16;
//
// General definitions for any method tagger.
//
const int MAX_SENTENCE_SIZE = 512; // restrict size of sentences 

const int LENGTH_MAX_BITS = 3;
const int LENGTH_MAX = 1<<LENGTH_MAX_BITS;

class CStateItem;

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTaggerImpl {
   protected:
      CAgendaBeam<CStateItem> *m_Agenda;
   public:
      CTaggerImpl() { m_Agenda = new CAgendaBeam<CStateItem>(AGENDA_SIZE); }
      virtual ~CTaggerImpl() { 
         delete m_Agenda;
      }
};

inline long int hash(const int &i) {return i;}

#endif
