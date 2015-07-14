ifeq ($(SPANISH_DEPPARSER_LABELED), true)
	SPANISH_DEPPARSER_D = -DLABELED
endif

ifneq ("$(wildcard Makefile.d/Makefile.es.conparser.$(SPANISH_CONPARSER_IMPL))","")
include Makefile.d/Makefile.es.conparser.$(SPANISH_CONPARSER_IMPL)
else
include Makefile.d/Makefile.es.conparser
endif

ifneq ("$(wildcard Makefile.d/Makefile.es.deplabeler.$(SPANISH_DEPLABELER_IMPL))","")
include Makefile.d/Makefile.es.deplabeler.$(SPANISH_DEPLABELER_IMPL)
else
ifneq ("$(wildcard $(SRC_COMMON_DEPLABELER)/implementations/$(SPANISH_DEPLABELER_IMPL)/Makefile)","")
include $(SRC_COMMON_DEPLABELER)/implementations/$(SPANISH_DEPLABELER_IMPL)/Makefile
else
include Makefile.d/Makefile.es.deplabeler
endif
endif

ifneq ("$(wildcard Makefile.d/Makefile.es.depparser.$(SPANISH_DEPPARSER_IMPL))","")
include Makefile.d/Makefile.es.depparser.$(SPANISH_DEPPARSER_IMPL)
else
ifneq ("$(wildcard $(SRC_COMMON_DEPPARSER)/implementations/$(SPANISH_DEPPARSER_IMPL)/Makefile)","")
include $(SRC_COMMON_DEPPARSER)/implementations/$(SPANISH_DEPPARSER_IMPL)/Makefile
else
include Makefile.d/Makefile.es.depparser
endif
endif

ifneq ("$(wildcard Makefile.d/Makefile.es.postagger.$(SPANISH_TAGGER_IMPL))","")
include Makefile.d/Makefile.es.postagger.$(SPANISH_TAGGER_IMPL)
else
ifneq ("$(wildcard $(SRC_SPANISH_TAGGER)/implementations/$(SPANISH_TAGGER_IMPL)/Makefile)","")
include $(SRC_SPANISH_TAGGER)/implementations/$(SPANISH_TAGGER_IMPL)/Makefile
else
include Makefile.d/Makefile.es.postagger
endif
endif

zpar.es: $(OBJECT_DIR) $(DIST_DIR) $(OBJECT_DIR)/reader.o $(OBJECT_DIR)/writer.o $(OBJECT_DIR)/options.o $(OBJECT_DIR)/spanish.postagger.o $(OBJECT_SPANISH_TAGGER)/weight.o $(OBJECT_DIR)/spanish.depparser.o $(OBJECT_SPANISH_DEPPARSER)/weight.o $(OBJECT_DIR)/spanish.deplabeler.o $(OBJECT_SPANISH_DEPLABELER)/weight.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -DTARGET_LANGUAGE=spanish -D$(SPANISH_ANNOTATION) $(SPANISH_DEPPARSER_D) -I$(SRC_SPANISH) -I$(SRC_SPANISH_TAGGER) -I$(SRC_SPANISH_TAGGER)/implementations/$(SPANISH_TAGGER_IMPL) -I$(SRC_COMMON_DEPPARSER) -I$(SRC_COMMON_DEPPARSER)/implementations/$(SPANISH_DEPPARSER_IMPL) -I$(SRC_COMMON_DEPLABELER) -I$(SRC_COMMON_DEPLABELER)/implementations/$(SPANISH_DEPLABELER_IMPL) -c $(SRC_SPANISH)/zpar.es.cpp -o $(OBJECT_DIR)/zpar.es.o
	$(LD) $(LDFLAGS) -o $(DIST_DIR)/zpar.es $(OBJECT_DIR)/zpar.es.o $(OBJECT_SPANISH_TAGGER)/weight.o $(OBJECT_DIR)/spanish.postagger.o $(OBJECT_DIR)/spanish.depparser.o $(OBJECT_SPANISH_DEPPARSER)/weight.o  $(OBJECT_DIR)/spanish.deplabeler.o $(OBJECT_SPANISH_DEPLABELER)/weight.o $(OBJECTS)
	@echo The Spanish zpar.es system compiled - configured with annotation type $(SPANISH_ANNOTATION) - successfully into $(DIST_DIR).

clean.es: clean.es.postagger clean.es.conparser clean.es.depparser clean.es.deplabeler
