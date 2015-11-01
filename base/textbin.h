#ifndef BASE_TEXTBIN_H
#define BASE_TEXTBIN_H
// text (message of) binary data (via printable characters) 
#include "base/prefix.h"
#include "base/stream.h"

EXTERN_API void TextBinEncode(Input* in, Output* out);
EXTERN_API void TextBinDecode(Input* in, Output* out);
EXTERN_API void TextBinTest(void);

#endif BASE_TEXTBIN_H
