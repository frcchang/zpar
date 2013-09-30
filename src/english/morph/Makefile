main: penn_lexicon_unit_test aux_lexicon_unit_test

morph_o:
	g++ -I ../../include/ -I ../../ -I ../../english/ -I ../../libs/ -c morph.cpp -o morph.o 2>&1 | g++colors.sh

penn_lexicon_o: morph_o
	g++ -I ../../include/ -I ../../ -I ../../english/ -I ../../libs/ -c penn_lexicon.cpp -o penn_lexicon.o 2>&1 | g++colors.sh

aux_lexicon_o: penn_lexicon_o morph_o
	g++ -I ../../include/ -I ../../ -I ../../english/ -I ../../libs/ -c aux_lexicon.cpp -o aux_lexicon.o 2>&1 | g++colors.sh

penn_lexicon_unit_test: morph_o
	g++ -I ../../include/ -I ../../ -I ../../english/ -I ../../libs/ test_penn_lexicon.cpp morph.o -o test_penn_lexicon.out 2>&1 | g++colors.sh
	
aux_lexicon_unit_test: penn_lexicon_o morph_o
	g++ -I ../../include/ -I ../../ -I ../../english/ -I ../../libs/ test_aux_lexicon.cpp penn_lexicon.o morph.o -o test_aux_lexicon.out 2>&1 | g++colors.sh
