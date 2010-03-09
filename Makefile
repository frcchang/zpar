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

# choose between agenda, chart, agendachart etc ## NO SPACE AFTER NAME ###
#
# agenda: the single agenda method - reproduce paper
# agendaplus: try to improve upon the decoding algorithm of agenda
# viterbi: dynamic programming
SEGMENTOR_IMPL = viterbi

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
CHINESE_DEPPARSER_IMPL = combined
CHINESE_DEPPARSER_LABELED = false

# currently support sr implementations 
CHINESE_CONPARSER_IMPL = sr

# currently support only agenda
ENGLISH_TAGGER_IMPL = agenda

# currently support eisner, covington, nivre, combined implementations 
ENGLISH_DEPPARSER_IMPL = nivre
ENGLISH_DEPPARSER_LABELED = true
# currently support sr implementations 
ENGLISH_CONPARSER_IMPL = sr
#================================================================
#
# Debug mode or the run mode (empty)
#
#================================================================

#DEBUG = -DDEBUG
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
# compiler commands
# 
#================================================================

INCLUDES = -I$(SRC_INCLUDES)

CXX = g++
CXXFLAGS = $(INCLUDES) $(DEBUG)

LD=$(CXX)
LDFLAGS =

#================================================================
#
# Shared objects
#
#================================================================

# the objects
OBJECTS = $(OBJECT_DIR)/reader.o $(OBJECT_DIR)/writer.o  $(OBJECT_DIR)/options.o

$(OBJECT_DIR)/%.o: $(SRC_LIBS)/%.cpp $(SRC_INCLUDES)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: zpar

# the directories
$(OBJECT_DIR):
	mkdir $(OBJECT_DIR)
$(DIST_DIR):
	mkdir $(DIST_DIR)

#  tagger
SRC_TAGGER = $(SRC_CHINESE)/tagger
DIST_TAGGER = $(DIST_DIR)/chinese.postagger
OBJECT_TAGGER = $(OBJECT_DIR)/chinese.postagger

#  depparser
SRC_COMMON_DEPPARSER = $(SRC_COMMON)/depparser
ifeq ($(CHINESE_DEPPARSER_IMPL), joint)
	SRC_CHINESE_DEPPARSER = $(SRC_CHINESE)/depparser
else
	SRC_CHINESE_DEPPARSER = $(SRC_COMMON_DEPPARSER)
endif
DIST_DEPPARSER = $(DIST_DIR)/chinese.depparser
OBJECT_DEPPARSER = $(OBJECT_DIR)/chinese.depparser

#  conparser
SRC_COMMON_CONPARSER = $(SRC_COMMON)/conparser
SRC_CHINESE_CONPARSER = $(SRC_COMMON_CONPARSER)
DIST_CONPARSER = $(DIST_DIR)/chinese.conparser
OBJECT_CONPARSER = $(OBJECT_DIR)/chinese.conparser

#----------------------------------------------------------------
#
# zpar general
#
#----------------------------------------------------------------

include Makefile.zpar

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

include Makefile.depparser

#----------------------------------------------------------------
#
# The conparser
#
#----------------------------------------------------------------

include Makefile.conparser

#----------------------------------------------------------------
#
# Miscelaneous
#
#----------------------------------------------------------------

include Makefile.misc

