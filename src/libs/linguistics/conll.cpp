#include "linguistics/conll.h"

void readCoNLLFeats(std::vector<CCoNLLFeats> &outout, const std::string &input) {
   outout.clear();
   outout.push_back(CCoNLLFeats(input));
   std::istringstream iss(input);
   std::string line;
   getline(iss, line, '|');
   if (line=="_")
      return;
   while (iss && !line.empty()) {
      outout.push_back(CCoNLLFeats(line));
      getline(iss, line, '|');
   }
}

