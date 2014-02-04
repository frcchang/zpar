// Copyright (C) University of Oxford 2010

#ifndef _CHINESE_DEP_H
#define _CHINESE_DEP_H

namespace chinese {

#ifndef CHINESE_DEP_SET
#define CHINESE_DEP_SET ctb
#endif

#ifdef LABELED
#define INCLUDE_FILE(M) MACROTOSTRING(dependency/label/M.h)
#include INCLUDE_FILE(CHINESE_DEP_SET)
#undef INCLUDE_FILE
#endif
#define INCLUDE_FILE(M) MACROTOSTRING(dependency/rules/M.h)
#include INCLUDE_FILE(CHINESE_DEP_SET)
#undef INCLUDE_FILE

}

//=============================================================

#ifdef LABELED
#endif

#endif
