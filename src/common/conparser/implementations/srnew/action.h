#ifndef _COMMON_CON_PARSER_ACTION
#define _COMMON_CON_PARSER_ACTION

/*===============================================================
 *
 * Reduce actions
 *
 *==============================================================*/

class CAction {

public: 
   inline unsigned long encodeAction(const unsigned long &action, const unsigned long &num) {
      assert(action>>(CConstituent::SIZE+4)==0);
      return action | ( num << (CConstituent::SIZE+4) ); // action takes 4 extra bits plus consti (REDUCE, SINGLE_C, HEAD_L, TEMP)
   }
   
protected:
   unsigned long action;

public:
   inline bool isShift() const { return action == 0; }
   inline bool isReduce() const { return action & (1<<(CConstituent::SIZE+3)); }
   inline bool isReduceRoot() const { return action == 1<<(CConstituent::SIZE+3); }
   inline bool isReduceUnary() const { return isReduce() && (action & (1<<(CConstituent::SIZE+1))); }
   inline bool isReduceBinary() const { return isReduce() && !(action & (1<<(CConstituent::SIZE+1)))&&!isReduceRoot(); }

public:
   inline void encodeReduce(const unsigned long &constituent, bool single_child, bool head_left, bool temporary) {
      assert(!single_child || (!head_left&&!temporary));
      action = (1<<(CConstituent::SIZE+3)) | 
               ((temporary?1:0) << (CConstituent::SIZE+2)) | 
               ((single_child?1:0) << (CConstituent::SIZE+1)) | 
               ((head_left?1:0) << CConstituent::SIZE) | 
               constituent ;
   }
   
   inline void encodeShift() {
      action = 0;
   }
   
   inline void encodeReduceRoot() {
      encodeReduce(CConstituent::NONE, false, false, false);
   }

public:
   inline unsigned long getConstituent() const {
      return action & ((1<<CConstituent::SIZE)-1);
   }
   
   inline bool singleChild() const {
      return action & (1<<(CConstituent::SIZE+1));
   }
   
   inline bool headLeft() const {
      return action & (1<<CConstituent::SIZE);
   }
   
   inline bool isTemporary() const {
      return action & (1<<(CConstituent::SIZE+2));
   }
 
public:
   inline string str() const {
      if (isShift()) return "SHIFT";
      assert(isReduce());
      string retval = "REDUCE";
      if (isReduceRoot()) { retval += " ROOT"; return retval; }
      else if (singleChild()) retval += " UNARY";
      else {
         retval += " BINARY";
         retval += (headLeft()) ? " LEFT" : " RIGHT";
         if (isTemporary()) retval += " TMP";
      }
      retval += " "; retval += CConstituent(getConstituent()).str();
      return retval;
   }
   inline void load(const string &s) {
      istringstream iss(s);
      string tmp;
      CConstituent c;
      CAction t;
      bool head_left, temporary;
      iss >> tmp;
      if (tmp=="SHIFT") {
         encodeShift();
      }
      else {
         assert (tmp=="REDUCE"); 
         iss >> tmp;
         if (tmp=="ROOT") {
            encodeReduceRoot();
         }
         else if (tmp=="UNARY") {
            iss >> tmp;
            c.load(tmp);
            encodeReduce(c.code(), true, false, false);
         }
         else {
            assert(tmp=="BINARY");
            iss >> tmp;
            if (tmp=="LEFT") {
               head_left = true;
            }
            else {
               assert(tmp=="RIGHT");
               head_left = false;
            }
            iss >> tmp;
            if (tmp=="TMP") {
               temporary = true;
               iss >> tmp;
            }
            else {
               temporary = false;
            }
            c.load(tmp);
            encodeReduce(c.code(), false, head_left, temporary);
         }
      }
   }

public:
   const unsigned long &hash() const {return action;}
   bool operator == (const CAction &a1) const { return action == a1.action; }
   bool operator != (const CAction &a1) const { return action != a1.action; }
   bool operator < (const CAction &a1) const { return action < a1.action; }
   bool operator > (const CAction &a1) const { return action > a1.action; }
   bool operator <= (const CAction &a1) const { return action <= a1.action; }
   bool operator >= (const CAction &a1) const { return action >= a1.action; }

};

//=================================================================================

inline std::istream & operator >> (std::istream &is, CAction &action) {
   string tmp;
   CConstituent c;
   CAction t;
   bool head_left, temporary;
   is >> tmp;
   if (tmp=="SHIFT") {
      action.encodeShift();
   }
   else {
      assert (tmp=="REDUCE"); 
      is >> tmp;
      if (tmp=="ROOT") {
         action.encodeReduceRoot();
      }
      else if (tmp=="UNARY") {
         is >> tmp;
         c.load(tmp);
         action.encodeReduce(c.code(), true, false, false);
      }
      else {
         assert(tmp=="BINARY");
         is >> tmp;
         if (tmp=="LEFT") {
            head_left = true;
         }
         else {
            assert(tmp=="RIGHT");
            head_left = false;
         }
         is >> tmp;
         if (tmp=="TMP") {
            temporary = true;
            is >> tmp;
         }
         else {
            temporary = false;
         }
         c.load(tmp);
         action.encodeReduce(c.code(), false, head_left, temporary);
      }
   }
}

inline std::ostream & operator << (std::ostream &os, const CAction &action) {
   os << action.str();
}
#endif
