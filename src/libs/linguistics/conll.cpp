#include "linguistics/conll.h"

void readCoNLLFeats(std::vector<CCoNLLFeats> &outout, const std::string &input) {
   std::istringstream iss(input);
   outout.clear();
   std::string line;
   getline(iss, line, '|');
   while (iss && !line.empty()) {
      outout.push_back(CCoNLLFeats(line));
      getline(iss, line, '|');
   }
}

