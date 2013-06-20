// Copyright (C) University of Oxford 2010
#ifndef _ENGLISH_DEPLABEL_H
#define _ENGLISH_DEPLABEL_H

#ifdef LABELED
#ifdef STANFORD_DEPENDENCIES
#include "dependency/label/stanford.h"
#else
#include "dependency/label/penn.h"
#endif
#endif

#ifdef STANFORD_DEPENDENCIES
#include "dependency/rules/stanford.h"
#else
#include "dependency/rules/penn.h"
#endif

#endif
