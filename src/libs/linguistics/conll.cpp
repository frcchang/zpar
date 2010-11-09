#include "linguistics/conll.h"

void readCoNLLFeats(vector<CCoNLLFeats> &output, const string &input) {
   istringstream iss(input);
   output.clear();
   string line;
   getline(iss, line, '|');
   while (iss && !line.empty()) {
      output.push_back(CCoNLLFeats(line));
      getline(iss, line, '|');
   }
}

