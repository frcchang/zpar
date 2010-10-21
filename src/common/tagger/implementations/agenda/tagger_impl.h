// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * The tagger's agenda implementation on segmented 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_IMPL_H
#define TAGGER_IMPL_H

class CStateItem;

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTaggerImpl {
   protected:
      CAgendaSimple<CStateItem> *m_Agenda;
   public:
      CTaggerImpl() { m_Agenda = new CAgendaSimple<CStateItem>(AGENDA_SIZE); }
      virtual ~CTaggerImpl() { 
         delete m_Agenda;
      }
};

#endif
