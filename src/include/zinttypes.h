#ifndef _ZINTTYPES_H
#define _ZINTTYPES_H

// C++11 and above, has cstdint
#if __cplusplus > 199711L
  #include <cstdint>

// C++98, use c99 cstdint.h instead
#else
  #include <stdint.h>

#endif

#endif // _ZINTTYPES_H
