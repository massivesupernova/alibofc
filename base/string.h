#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base/prefix.h"

typedef struct {
  Byte* start;
  Byte* end;
} Tstr;

EXTERN_API Tstr StrC(const char* cstr);
EXTERN_API Tstr StrE(const void* start, const void* end);
EXTERN_API Tstr StrN(const void* start, Int n);

#endif /* BASE_STRING_H */
