#include <iostream>
#include "reader.h"

void CDepParser::TestSuite1() {

  // simulating the training and test process
  const char * train_conll_context[] = {
    "1	Den	_	P	PP	person=3|gender=common|number=sing|case=unmarked|reflexive=no|register=unmarked	7	nobj	_	_",
    "2	modtog	_	V	VA	mood=indic|tense=past|voice=active	0	ROOT	_	_",
    "3	han	_	P	PP	person=3|gender=common|number=sing|case=nom|reflexive=no|register=unmarked	2	subj	_	_",
    "4	i_øvrigt	_	RG	RG	degree=unmarked	2	mod	_	_",
    "5	Kulturministeriets	_	N	NC	gender=neuter|number=sing|case=gen|def=def	2	dobj	_	_",
    "6	børnebogspris	_	N	NC	gender=common|number=sing|case=unmarked|def=indef	5	possd	_	_",
    "7	for	_	SP	SP	_	2	pobj	_	_",
    "8	i	_	SP	SP	_	2	mod	_	_",
    "9	1990	_	A	AC	case=unmarked	8	nobj	_	_",
    "10	.	_	X	XP	_	2	pnct	_	_"
  };

  std::string buffer = "";
  for (int i = 0; i < 10; ++ i) {
    buffer += train_conll_context[i];
  }
  std::cerr << buffer << std::endl;

  std::stringstream S(buffer);
  CDependencyParse oracle_tree;
  S >> oracle_tree;

  train(oracle_tree, 1);
}
