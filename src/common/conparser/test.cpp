/****************************************************************
 *                                                              *
 * test.cpp - the testing of English constituent parser.        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "conparser.h"
#include "reader.h"
#include "writer.h"

#include <cstring>

using namespace english;

/*===============================================================
 *
 * test_weight - just load and save weights to see if the same
 *
 *===============================================================*/

void test_weight(const string &sFeatureFile) {

   cout << "Testing weights..." << endl ; cout.flush();

   CDepParser parser(sFeatureFile, true);

   parser.finishtraining();

   cout << "Done. " << endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   const string hint = " feature_file";
   if (argc < 2) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }

   test_weight(argv[1]);

   return 0;

}

