#****************************************************************
#
# Makefile
#
# Yue Zhang
# Computing lab, Oxford. 2006.10 - 2008.2
#
#****************************************************************

# Makable targe systems include:
#
# === Chinese ===
# segmentor - Chinese word segmentor
# chinese.postagger - Chinese POS tagger (joint / single)
# chinese.depparser - Chinese dependency parser
#
# === English ===
# english.postagger - English POS tagger
# english.depparser - English dependency parser

#================================================================
#
# Configurations
#
#================================================================

# the generic tagger
TAGGER_IMPL = collins

# the generic depparser
DEPPARSER_IMPL = miguel

# the generic conparser
CONPARSER_IMPL = srnew

# choose between agenda, agendachart etc ## NO SPACE AFTER NAME ###
#
# agenda: the single agenda method - reproduce paper
# agendaplus: try to improve upon the decoding algorithm of agenda
# viterbi: dynamic programming
SEGMENTOR_IMPL = agenda

# Chinese postagger implementations
#
# joint taggers include the following implementations
# agendachart: combining agenda and chart, this is the best system - reproduce paper
# 
# taggers on segmented sentences include the following implementations
# segmented: the unidirectional trigram tagger
CHINESE_TAGGER_IMPL = dbn

# Chinese dependency parser
#
# currently support eisner, covington, nivre, combined and joint implementations 
CHINESE_DEPPARSER_IMPL = arceager-mvt
CHINESE_DEPPARSER_LABELED = true
CHINESE_DEPLABELER_IMPL = naive

# currently support sr implementations 
CHINESE_CONPARSER_IMPL = jcad

# currently support only agenda
ENGLISH_TAGGER_IMPL = collins

# currently support eisner, covington, nivre, combined implementations 
ENGLISH_DEPPARSER_IMPL = arceager
ENGLISH_DEPPARSER_LABELED = true
ENGLISH_DEPLABELER_IMPL = naive

# currently support sr implementations 
ENGLISH_CONPARSER_IMPL = miguel

# Spanish pos tagger
SPANISH_TAGGER_IMPL = collins

# Spanish dependency parser
SPANISH_DEPPARSER_IMPL = arceager
SPANISH_DEPPARSER_LABELED = true
SPANISH_DEPLABELER_IMPL = naive

# Spanish annotation. Supported: ES06_DEPENDENCIES, ES09_DEPENDENCIES
SPANISH_ANNOTATION = ES09_DEPENDENCIES

#no Spanish constituency parser at the moment

#================================================================
#
# Debug mode or the run mode (empty)
#
#================================================================

DEBUG = -DDEBUG -g
#DEBUG = -DNDEBUG

#================================================================
#
# directory configurations
#
#================================================================

BASE_DIR = .
include Makefile.common

#================================================================
#
# compiler commands
# 
#================================================================

INCLUDES = -I$(SRC_INCLUDES)

CXX = g++
CXXFLAGS = -w -W -O3 $(INCLUDES) $(DEBUG)

LD=$(CXX)
LDFLAGS =

#================================================================
#
# Shared objects
#
#================================================================

# the objects
LINGUISTICS_OBJECTS = $(OBJECT_DIR)/linguistics/lemma.o $(OBJECT_DIR)/linguistics/conll.o
LEARNING_OBJECTS = $(OBJECT_DIR)/learning/dbn.o
OBJECTS = $(OBJECT_DIR)/reader.o $(OBJECT_DIR)/writer.o $(OBJECT_DIR)/options.o $(LINGUISTICS_OBJECTS) $(LEARNING_OBJECTS)

$(OBJECT_DIR)/%.o: $(SRC_LIBS)/%.cpp $(SRC_INCLUDES)/%.h
	mkdir -p $(OBJECT_DIR)
	mkdir -p $(OBJECT_DIR)/linguistics
	mkdir -p $(OBJECT_DIR)/learning
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: zpar

# the directories
$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)
$(DIST_DIR):
	mkdir -p $(DIST_DIR)

#  tagger
SRC_TAGGER = $(SRC_CHINESE)/tagger
DIST_TAGGER = $(DIST_DIR)/chinese.postagger
OBJECT_TAGGER = $(OBJECT_DIR)/chinese.postagger
$(DIST_TAGGER):
	mkdir $(DIST_TAGGER)
$(OBJECT_TAGGER):
	mkdir $(OBJECT_TAGGER)

SRC_ENGLISH_TAGGER = $(SRC_COMMON)/tagger
DIST_ENGLISH_TAGGER = $(DIST_DIR)/english.postagger
OBJECT_ENGLISH_TAGGER = $(OBJECT_DIR)/english.postagger
$(DIST_ENGLISH_TAGGER):
	mkdir $(DIST_ENGLISH_TAGGER)
$(OBJECT_ENGLISH_TAGGER):
	mkdir $(OBJECT_ENGLISH_TAGGER)
	
SRC_SPANISH_TAGGER = $(SRC_COMMON)/tagger
DIST_SPANISH_TAGGER = $(DIST_DIR)/spanish.postagger
OBJECT_SPANISH_TAGGER = $(OBJECT_DIR)/spanish.postagger
$(DIST_SPANISH_TAGGER):
	mkdir $(DIST_SPANISH_TAGGER)
$(OBJECT_SPANISH_TAGGER):
	mkdir $(OBJECT_SPANISH_TAGGER)

#  depparser
SRC_COMMON_DEPPARSER = $(SRC_COMMON)/depparser
#ifeq ($(CHINESE_DEPPARSER_IMPL), joint)
#	SRC_CHINESE_DEPPARSER = $(SRC_CHINESE)/depparser
#else
#	SRC_CHINESE_DEPPARSER = $(SRC_COMMON_DEPPARSER)
#endif
SRC_CHINESE_DEPPARSER = $(SRC_COMMON_DEPPARSER)
DIST_DEPPARSER = $(DIST_DIR)/chinese.depparser
OBJECT_DEPPARSER = $(OBJECT_DIR)/chinese.depparser
DIST_ENGLISH_DEPPARSER = $(DIST_DIR)/english.depparser
OBJECT_ENGLISH_DEPPARSER = $(OBJECT_DIR)/english.depparser
DIST_SPANISH_DEPPARSER = $(DIST_DIR)/spanish.depparser
OBJECT_SPANISH_DEPPARSER = $(OBJECT_DIR)/spanish.depparser

# deplabeler
SRC_COMMON_DEPLABELER = $(SRC_COMMON)/deplabeler
SRC_CHINESE_DEPLABELER = $(SRC_COMMON_DEPLABELER)
DIST_DEPLABELER = $(DIST_DIR)/chinese.deplabeler
OBJECT_DEPLABELER = $(OBJECT_DIR)/chinese.deplabeler
SRC_ENGLISH_DEPLABELER = $(SRC_COMMON_DEPLABELER)
DIST_ENGLISH_DEPLABELER = $(DIST_DIR)/english.deplabeler
OBJECT_ENGLISH_DEPLABELER = $(OBJECT_DIR)/english.deplabeler
SRC_SPANISH_DEPLABELER = $(SRC_COMMON_DEPLABELER)
DIST_SPANISH_DEPLABELER = $(DIST_DIR)/spanish.deplabeler
OBJECT_SPANISH_DEPLABELER = $(OBJECT_DIR)/spanish.deplabeler

#  conparser
SRC_COMMON_CONPARSER = $(SRC_COMMON)/conparser
SRC_CHINESE_CONPARSER = $(SRC_COMMON_CONPARSER)
ifeq ($(CHINESE_CONPARSER_IMPL), jcad)
	SRC_CHINESE_CONPARSER = $(SRC_CHINESE)/conparser
else
	SRC_CHINESE_CONPARSER = $(SRC_COMMON_CONPARSER)
endif
SRC_ENGLISH_CONPARSER = $(SRC_COMMON_CONPARSER)
DIST_CONPARSER = $(DIST_DIR)/chinese.conparser
OBJECT_CONPARSER = $(OBJECT_DIR)/chinese.conparser
DIST_ENGLISH_CONPARSER = $(DIST_DIR)/english.conparser
OBJECT_ENGLISH_CONPARSER = $(OBJECT_DIR)/english.conparser

#----------------------------------------------------------------
#
# zpar general
#
#----------------------------------------------------------------

include Makefile.zpar
include Makefile.zpar.en
include Makefile.zpar.ge
include Makefile.zpar.es

#----------------------------------------------------------------
#
# The sentence boundary detector
#
#----------------------------------------------------------------

include Makefile.doc2snt

#----------------------------------------------------------------
#
# The segmentor
#
#----------------------------------------------------------------

include Makefile.segmentor

#----------------------------------------------------------------
#
# The pos taggers (Chinese and English)
#
#----------------------------------------------------------------

include Makefile.postagger

#----------------------------------------------------------------
#
# The depparsers (Chinese and English)
#
#----------------------------------------------------------------

ifeq ($(ENGLISH_DEPPARSER_IMPL),morphparser)
include Makefile.depparser.morph
else
include Makefile.depparser
endif

include Makefile.deplabeler

#----------------------------------------------------------------
#
# The conparser
#
#----------------------------------------------------------------

include Makefile.conparser

#----------------------------------------------------------------
#
# The ccgparser
#
#----------------------------------------------------------------

include Makefile.ccg

#----------------------------------------------------------------
#
# Miscelaneous
#
#----------------------------------------------------------------

include Makefile.misc
#include Makefile.rr

