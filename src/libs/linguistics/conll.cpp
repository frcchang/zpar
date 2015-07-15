#include "linguistics/conll.h"

void readCoNLLFeats(std::vector<CCoNLLFeats> &output, const std::string &input) {
   output.clear();
   output.push_back(CCoNLLFeats(input));
   std::istringstream iss(input);
   std::string line;
   getline(iss, line, '|');
   if (line=="_")
      return;
   while (iss && !line.empty()) {
      output.push_back(CCoNLLFeats(line));
      getline(iss, line, '|');
   }
}

