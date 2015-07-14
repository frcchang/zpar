// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * cfgtemp.h - the definitions for temporary constituents       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _TEMPORARY_OBJECT_H
#define _TEMPORARY_OBJECT_H

/*===============================================================
 *
 * definitions about tmp
 *
 *==============================================================*/

template <typename CConstituentLabel>
class CTemporary: public CConstituentLabel {
public:
   enum {SIZE=CConstituentLabel::SIZE+1}; // other enums from K will be copied

public:
   CTemporary() : CConstituentLabel() {}
   CTemporary(const CTemporary &t) { CConstituentLabel::m_code=t.CConstituentLabel::m_code; }
   CTemporary(const unsigned long &u) { CConstituentLabel::m_code=u; }
   CTemporary(const std::string &s) { load(s); }

public:
   const unsigned long &code() const { return CConstituentLabel::m_code; }
   const unsigned long &hash() const { return CConstituentLabel::m_code; }
   void copy(const CTemporary &c) {CConstituentLabel::m_code = c.CConstituentLabel::m_code; }
   const bool empty() const { return CConstituentLabel::m_code==CConstituentLabel::NONE; }
   bool operator == (const CTemporary &w) const { return CConstituentLabel::m_code == w.CConstituentLabel::m_code; }
   bool operator != (const CTemporary &w) const { return CConstituentLabel::m_code != w.CConstituentLabel::m_code; }
   bool operator < (const CTemporary &w) const { return CConstituentLabel::m_code < w.CConstituentLabel::m_code; }
   bool operator > (const CTemporary &w) const { return CConstituentLabel::m_code > w.CConstituentLabel::m_code; }
   bool operator <= (const CTemporary &w) const { return CConstituentLabel::m_code <= w.CConstituentLabel::m_code; }
   bool operator >= (const CTemporary &w) const { return CConstituentLabel::m_code >= w.CConstituentLabel::m_code; }

   void load(const std::string &s) {
      if (s.size() && s[s.size()-1] == '*') {
         CConstituentLabel::load(s.substr(0, s.size()-1));
         setTemporary(true);
      }
      else {
         CConstituentLabel::load(s);
         setTemporary(false);
      }
   }
   void load(const unsigned long &code) {CConstituentLabel::m_code=code;}
   const std::string &str() const {
      static std::string tmp;
      tmp = CConstituentLabel(clearTmp(CConstituentLabel::m_code)).str();
      if (getTemporary()) {
         tmp += "*";
      }
      return tmp;
   }

   void setTemporary(const bool &temporary) {
      if (temporary)
         CConstituentLabel::m_code |= (1L << CConstituentLabel::SIZE);
      else
         CConstituentLabel::m_code &= (~(1L<<CConstituentLabel::SIZE));
   }
   bool getTemporary() const { return CConstituentLabel::m_code & (1L << CConstituentLabel::SIZE);}

   bool canBeTemporary() const { return true; }
   unsigned long extractConstituentCode() const {
      return clearTmp(CConstituentLabel::m_code);
   }

public:
   static unsigned long encodeTmp(const unsigned long &constituent, const bool &temporary) {
      return (temporary?constituent|(1L<<CConstituentLabel::SIZE):constituent);
   }
   static unsigned long clearTmp(const unsigned long &constituent) {
      return constituent&(~(1L<<CConstituentLabel::SIZE));
   }
};

//===============================================================

template<typename CConstituentLabel>
inline std::istream & operator >> (std::istream &is, CTemporary<CConstituentLabel> &con) {
   std::string s;
   is >> s;
   con.load(s);
   return is;
}

template<typename CConstituentLabel>
inline std::ostream & operator << (std::ostream &os, const CTemporary<CConstituentLabel> &con) {
   os << con.str() ;
   return os;
}

#endif

