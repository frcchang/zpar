#CC=clang++
CC=g++
#CFLAGS=-c -Wall -g -std=c++11	 
CFLAGS=-c -Wall -O4 -std=c++11 
LDFLAGS= #-lpthread
#SOURCES=BaseExtractor.cpp Tagging.cpp NNScorer.cpp ChExtractor.cpp Sentence.cpp Config.cpp	DS_POS.cpp	EasyFirstPOS.cpp	EnExtractor.cpp	GlobalMap.cpp	main.cpp	Pool.cpp	Train.cpp	RBMUtil.cpp	Template.cpp	util.cpp	
SOURCES=BaseExtractor.cpp Tagging.cpp NNScorer.cpp ChExtractor.cpp Sentence.cpp Config.cpp	Analysis.cpp	EasyFirstPOS.cpp	EnExtractor.cpp	GlobalMap.cpp	main.cpp	Pool.cpp	Train.cpp	Template.cpp	util.cpp	
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Tagger

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o Tagger
