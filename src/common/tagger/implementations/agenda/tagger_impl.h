/***************************************************************
 *
 * The tagger's agenda implementation on segmented 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_SEGMENTED_IMPL_H
#define TAGGER_SEGMENTED_IMPL_H 1

#include "macros.h"

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

#endif
