// Copyright (C) University of Oxford 2010
#ifndef _COMMON_CON_PARSER_ACTION
#define _COMMON_CON_PARSER_ACTION

/*===============================================================
 *
 * type actions
 *
 *==============================================================*/

class CActionType {

public:
   static const unsigned long SIZE=3;
   enum CODE {NO_ACTION=0, SHIFT=1, REDUCE_BINARY=2, REDUCE_UNARY=3, POP_ROOT=4};

public:
   unsigned long code;

public:
   const unsigned long &hash() const {return code;}
   bool operator == (const CActionType &a1) const { return code == a1.code; }
   bool operator != (const CActionType &a1) const { return code != a1.code; }
   bool operator < (const CActionType &a1) const { return code < a1.code; }
   bool operator > (const CActionType &a1) const { return code > a1.code; }

};

inline std::istream & operator >> (std::istream &is, CActionType &action) {
   std::string s;
   is >> s;
   if (s=="NO_ACTION")
      action.code = CActionType::NO_ACTION;
   else if (s=="SHIFT")
      action.code = CActionType::SHIFT;
   else if (s=="REDUCE_UNARY")
      action.code = CActionType::REDUCE_UNARY;
   else if (s=="REDUCE_BINARY")
      action.code = CActionType::REDUCE_BINARY;
   else if (s=="POP_ROOT")
      action.code = CActionType::POP_ROOT;
   else
      THROW("Action type unrecognized (" << s << ")");
   return is;
}

inline std::ostream & operator << (std::ostream &os, const CActionType &action) {
   switch(action.code) {
   case CActionType::NO_ACTION:
      os << "NO_ACTION";
      break;
   case CActionType::SHIFT:
      os << "SHIFT";
      break;
   case CActionType::REDUCE_UNARY:
      os << "REDUCE_UNARY";
      break;
   case CActionType::REDUCE_BINARY:
      os << "REDUCE_BINARY";
      break;
   case CActionType::POP_ROOT:
      os << "POP_ROOT";
      break;
   default:
      THROW("Internal error: unknown action type code (" << action.code << ")");
   }
   return os;
}

/*===============================================================
 *
 * actions
 *
 *==============================================================*/

class CAction {

public:
   const static unsigned long MAX=(1L<<(CConstituent::SIZE+1+1+CActionType::SIZE));

protected:
   // CONSTITUENT(CConstituent::SIZE) TEMP(1 bit) HEAD(1) ACTIONTYPE(CActionType::SIZE)
   const static unsigned long HEADLEFT_SHIFT = CActionType::SIZE;
   const static unsigned long TEMPORARY_SHIFT = CActionType::SIZE+1;
   const static unsigned long CONSTITUENT_SHIFT = CActionType::SIZE+2;

   const static unsigned long ACTIONTYPE_MASK = (1L<<CActionType::SIZE)-1;
   const static unsigned long HEADLEFT_MASK = 1L<<HEADLEFT_SHIFT;
   const static unsigned long TEMPORARY_MASK = 1L<<TEMPORARY_SHIFT;
   const static unsigned long CONSTITUENT_MASK = ((1L<<CConstituent::SIZE)-1)<<CONSTITUENT_SHIFT;

//public: 
//   inline unsigned long encodeAction(const unsigned long &action, const unsigned long &num) {
//      assert(action>>(CConstituent::SIZE+4)==0);
//      return action | ( num << (CConstituent::SIZE+4) ); // action takes 4 extra bits plus consti (REDUCE, SINGLE_C, HEAD_L, TEMP)
//   }
   
protected:
   unsigned long action;

public:
   void clear() { action=0; }

public:
  CAction() : action(0) {}

public:
   inline bool isNone() const { return type()==CActionType::NO_ACTION; }
   inline bool isShift() const { return type()==CActionType::SHIFT; }
//   inline bool isReduce() const { return isReduceUnary() || isReduceBinary(); }
   inline bool isReduceRoot() const { return type()==CActionType::POP_ROOT; }
   inline bool isReduceUnary() const { return type()==CActionType::REDUCE_UNARY; }
   inline bool isReduceBinary() const { return type()==CActionType::REDUCE_BINARY; }

public:
   inline void encodeReduce(const unsigned long &constituent, bool single_child, bool head_left, bool temporary) {
      assert(!single_child || (!head_left&&!temporary));
#ifdef NO_TEMP_CONSTITUENT
      action = (constituent<<CONSTITUENT_SHIFT |
#else
      action = ((CConstituent::encodeTmp(constituent, temporary))<<CONSTITUENT_SHIFT |
#endif
                temporary<<TEMPORARY_SHIFT |
                head_left<<HEADLEFT_SHIFT |
               (single_child ? static_cast<unsigned long>(CActionType::REDUCE_UNARY) : static_cast<unsigned long>(CActionType::REDUCE_BINARY)));
   }
   
   inline void encodeShift(const unsigned long &constituent=CConstituent::NONE) {
      action = (constituent<<CONSTITUENT_SHIFT |
                CActionType::SHIFT);
   }
   
   inline void encodeReduceRoot() {
      action = CActionType::POP_ROOT;
   }

public:
   inline unsigned long type() const {
      return action & ACTIONTYPE_MASK;
   }
   inline unsigned long getConstituent() const {
      return (action & CONSTITUENT_MASK) >> CONSTITUENT_SHIFT;
   }
   
   inline bool singleChild() const {
      return type() == CActionType::REDUCE_UNARY;
   }
   
   inline bool headLeft() const {
      return action & HEADLEFT_MASK;
   }
   
   inline bool isTemporary() const {
      return action & TEMPORARY_MASK;
   }
 
public:
   inline std::string str() const {
      if (isNone()) { return "NONE"; }
      if (isReduceRoot()) { return "REDUCE ROOT"; }
      std::string retval;
      if (isShift()) {
         retval =  "SHIFT";
      }
      else {
          retval = "REDUCE";
         if (isReduceUnary()) 
            retval += " UNARY";
         else {
            ASSERT(isReduceBinary(), "Internal error: unknown action code ("<<action<<')');
            retval += " BINARY";
            retval += (headLeft()) ? " LEFT" : " RIGHT";
            if (isTemporary()) retval += " TMP";
         }
      }
      retval += " "; retval += CConstituent(getConstituent()).str();
      return retval;
   }
   inline void load(const std::string &s) {
      std::istringstream iss(s);
      std::string tmp;
      CConstituent c;
      CAction t;
      bool head_left, temporary;
      iss >> tmp;

      if (tmp=="NONE") {
         clear();
      }
      else if (tmp=="SHIFT") {
         iss >> tmp;
         c.load(tmp);
         encodeShift(c.code());
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
   const unsigned long &code() const {return action;}
   const unsigned long &hash() const {return action;}
   bool operator == (const CAction &a1) const { return action == a1.action; }
   bool operator != (const CAction &a1) const { return action != a1.action; }
   bool operator < (const CAction &a1) const { return action < a1.action; }
   bool operator > (const CAction &a1) const { return action > a1.action; }

};

//=================================================================================

inline std::istream & operator >> (std::istream &is, CAction &action) {
   std::string tmp;
   CConstituent c;
   CAction t;
   bool head_left, temporary;
   is >> tmp;
   if (tmp=="NONE") {
      action.clear();
   }
   else if (tmp=="SHIFT") {
      is >> tmp; c.load(tmp);
      action.encodeShift(c.code());
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
   return is;
}

inline std::ostream & operator << (std::ostream &os, const CAction &action) {
   os << action.str();
   return os;
}
#endif
