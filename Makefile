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
DEPPARSER_IMPL = arceager

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
CHINESE_TAGGER_IMPL = agenda

# Chinese dependency parser
#
# currently support eisner, covington, nivre, combined and joint implementations 
CHINESE_DEPPARSER_IMPL = arceager
CHINESE_DEPPARSER_LABELED = true
CHINESE_DEPLABELER_IMPL = naive

# currently support sr implementations 
CHINESE_CONPARSER_IMPL = acl13
CHINESE_CONPARSER_JOINT_OR_CASCADE = JOINT_CONPARSER

# currently support only agenda
ENGLISH_TAGGER_IMPL = collins

# currently support eisner, covington, nivre, combined implementations 
ENGLISH_DEPPARSER_IMPL = arceager
ENGLISH_DEPPARSER_LABELED = true
ENGLISH_DEPLABELER_IMPL = naive

# currently support sr implementations 
ENGLISH_CONPARSER_IMPL = muhua

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

#DEBUG = -DDEBUG -g
DEBUG = -DNDEBUG

#================================================================
#
# directory configurations
#
#================================================================

BASE_DIR = .
include Makefile.common

#================================================================
#
# cross platform configurations
# 
#================================================================

ifeq ($(OS),Windows_NT)
#use good old GNU mkdir instead of MSDOS mkdir on Windows
MKDIR=gmkdir -p
else
MKDIR=mkdir -p
endif

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
	$(MKDIR) $(OBJECT_DIR)
	$(MKDIR) $(OBJECT_DIR)/linguistics
	$(MKDIR) $(OBJECT_DIR)/learning
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: zpar

# the directories
$(OBJECT_DIR):
	$(MKDIR) $(OBJECT_DIR)
$(DIST_DIR):
	$(MKDIR) $(DIST_DIR)

#  tagger
SRC_TAGGER = $(SRC_CHINESE)/tagger
DIST_TAGGER = $(DIST_DIR)/chinese.postagger
OBJECT_TAGGER = $(OBJECT_DIR)/chinese.postagger
$(DIST_TAGGER):
	$(MKDIR) $(DIST_TAGGER)
$(OBJECT_TAGGER):
	$(MKDIR) $(OBJECT_TAGGER)

SRC_ENGLISH_TAGGER = $(SRC_COMMON)/tagger
DIST_ENGLISH_TAGGER = $(DIST_DIR)/english.postagger
OBJECT_ENGLISH_TAGGER = $(OBJECT_DIR)/english.postagger
$(DIST_ENGLISH_TAGGER):
	$(MKDIR) $(DIST_ENGLISH_TAGGER)
$(OBJECT_ENGLISH_TAGGER):
	$(MKDIR) $(OBJECT_ENGLISH_TAGGER)
	
SRC_SPANISH_TAGGER = $(SRC_COMMON)/tagger
DIST_SPANISH_TAGGER = $(DIST_DIR)/spanish.postagger
OBJECT_SPANISH_TAGGER = $(OBJECT_DIR)/spanish.postagger
$(DIST_SPANISH_TAGGER):
	$(MKDIR) $(DIST_SPANISH_TAGGER)
$(OBJECT_SPANISH_TAGGER):
	$(MKDIR) $(OBJECT_SPANISH_TAGGER)

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
	ifeq ($(CHINESE_CONPARSER_IMPL), acl13)
		SRC_CHINESE_CONPARSER = $(SRC_CHINESE)/conparser
	else
		SRC_CHINESE_CONPARSER = $(SRC_COMMON_CONPARSER)
	endif
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


ifeq ($(CHINESE_CONPARSER_IMPL), jcad)
	OBJ_CHINESE_CONSTITUENT = $(OBJECT_CONPARSER)/constituent.o $(OBJECT_CONPARSER)/jointconstituent.o
else
	ifeq ($(CHINESE_CONPARSER_IMPL), acl13)
		OBJ_CHINESE_CONSTITUENT = $(OBJECT_CONPARSER)/constituent.o $(OBJECT_CONPARSER)/jointconstituent.o
	else
		OBJ_CHINESE_CONSTITUENT = $(OBJECT_CONPARSER)/constituent.o
	endif
endif

$(DIST_CONPARSER):
	$(MKDIR) $(DIST_CONPARSER)
$(OBJECT_CONPARSER):
	$(MKDIR) $(OBJECT_CONPARSER)

$(DIST_DEPLABELER):
	$(MKDIR) $(DIST_DEPLABELER)
$(OBJECT_DEPLABELER):
	$(MKDIR) $(OBJECT_DEPLABELER)

# the flags for train
ifeq ($(CHINESE_TAGGER_IMPL), segmented) # if segmented	
	TAGGER_TRAIN_FLAGS = -DSEGMENTED
	TAGGER_TEST_FLAGS = -DSEGMENTED
else 
	ifeq ($(CHINESE_TAGGER_IMPL), bidirectional) # else if bidirectional
		TAGGER_TRAIN_FLAGS = -DSEGMENTED -DAUTO
		TAGGER_TEST_FLAGS = -DSEGMENTED
	endif
endif


ifeq ($(CHINESE_DEPPARSER_LABELED), true)
	CHINESE_DEPPARSER_D = -DLABELED
endif

ifeq ($(ENGLISH_DEPPARSER_LABELED), true)
	ENGLISH_DEPPARSER_D = -DLABELED
endif

ifeq ($(CHINESE_DEPPARSER_IMPL), combined)
	CHINESE_DEPPARSER_D := $(CHINESE_DEPPARSER_D) -DCOMBINED
	CHINESE_DEPPARSER_IMPL = nivre
endif

ifeq ($(ENGLISH_DEPPARSER_IMPL), combined)
	ENGLISH_DEPPARSER_D := $(ENGLISH_DEPPARSER_D) -DCOMBINED
	ENGLISH_DEPPARSER_IMPL = nivre
endif

#====================================================

$(DIST_DEPPARSER):
	$(MKDIR) $(DIST_DEPPARSER)
$(OBJECT_DEPPARSER):
	$(MKDIR) $(OBJECT_DEPPARSER)

SRC_SEGMENTOR = $(SRC_CHINESE)/segmentor
DIST_SEGMENTOR = $(DIST_DIR)/segmentor
OBJECT_SEGMENTOR = $(OBJECT_DIR)/segmentor
$(DIST_SEGMENTOR):
	$(MKDIR) $(DIST_SEGMENTOR)
$(OBJECT_SEGMENTOR):
	$(MKDIR) $(OBJECT_SEGMENTOR)

include Makefile.zpar.zh
include Makefile.zpar.en
include Makefile.zpar.ge
include Makefile.zpar.es
include Makefile.zpar.mvt

zpar: zpar.ge

#----------------------------------------------------------------
#
# The sentence boundary detector
#
#----------------------------------------------------------------

include Makefile.doc2snt

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

