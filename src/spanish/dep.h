// Copyright (C) University of Oxford 2010
#ifndef _SPANISH_DEPLABEL_H
#define _SPANISH_DEPLABEL_H

#ifdef LABELED
#ifdef ES06_DEPENDENCIES
#include "dependency/label/es06.h"
#else
#include "dependency/label/es09.h"
#endif
#endif

#ifdef ES06_DEPENDENCIES
#include "dependency/rules/es06.h"
#else
#include "dependency/rules/es09.h"
#endif

#endif
