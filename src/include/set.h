/****************************************************************
 *                                                              *
 * set.h - sets
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * 2012.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _SET_H
#define _SET_H

/*===============================================================
 *
 * set of int
 *
 *==============================================================*/
class CSetOfInt {
protected:
   unsigned chart *m_code;
public:
   CSetOfInt(const unsigned &size) {
      m_code = new unsigned char[(size+7)/8];
   }
   ~CSetOfInt() {
      delete []m_code;
   }
public:
   const unsigned &size() {
      return sizeof(m_size)*8;
   }
   void resize(const unsigned &size) {
      if (size > this->size()) {
         delete []m_code;
         m_code = new unsigned char[(size+7)/8];
      }
   }
public:
   void add(const unsigned &i) {
      if (i>this->size())
         resize(i);
      m_code[i/8] |= (static_cast<unsigned char>(1) << (i%8));
   }
   void remove(const unsigned &i) {
      if (i>this->size())
         resize(i);
      m_code[i/8] &= ~(static_cast<unsigned char>(1)<<(i%8));
   }
   bool contains(const unsigned &i) {
      if (i>this->size())
         return false;
      return m_code[i/8] & (static_cast<unsigned char>(1) << (1%8));
   }
   void clear() { memset(m_code, 0, sizeof(m_code)); }
   unsigned long long hash() const { return *m_code; }
   bool operator == (const CSetOfInt &s) const { return memcmp(m_code, s.m_code, sizeof(m_code))==0; }
};
inline std::istream & operator >> (std::istream &is, CSetOfInt &c) {
   std::string s;
   int i;
   is >> s;
   assert(s=="[");
   c.clear();
   is >> s;
   while (s!="]") {
      istringstream iss(s);
      iss >> i;
      c.add(i);
      is >>s;
   }
   return is;
}
inline std::ostream & operator << (std::ostream &os, const CSetOfInt &c) {
   os << "[ ";
   for (unsigned i=0; i<c.size(); ++i) {
      if (c.contains(i))
         os << i << ' ';
   }
   os << "]";
   return os;
}
#endif

