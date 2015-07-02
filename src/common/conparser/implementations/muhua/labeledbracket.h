// Yue Zhang

#ifndef _LABELED_BRACKET_H
#define _LABELED_BRACKET_H

namespace TARGET_LANGUAGE {

class CLabeledBracket {

public:
   unsigned begin;
   unsigned end;
   unsigned long constituent;

public:
   CLabeledBracket(): begin(0), end(0), constituent(0) {}
   CLabeledBracket(const unsigned &begin, const unsigned &end, const unsigned long &constituent): begin(begin), end(end), constituent(constituent) {}
};

inline
std::ostream & operator << (std::ostream &os, const CLabeledBracket &lb) {
   os << '<' << lb.begin << ',' << lb.end << ',' << CConstituent(lb.constituent).str() << '>';
}

} // namespace TARGET_LANGUAGE

#endif
