#ifndef BASE_STREAM_H
#define BASE_STREAM_H

#include "base/prefix.h"
#include "base/string.h"

struct InputImpl;
typedef struct {
  struct InputImpl* impl;
  Int (*read)(struct InputImpl* impl, Byte* outBuf, Int readBytes);
} Input;

STATIC_INLINE Int Read(Input* self, Byte* outBuf, Int readBytes) {
  return self->read(self->impl, outBuf, readBytes);
}

struct OutputImpl;
typedef struct {
  struct OutputImpl* impl;
  Int (*write)(struct OutputImpl* impl, Tstr str);
} Output;

STATIC_INLINE Int Write(Output* self, Tstr str) {
  return self->write(self->impl, str);
}

#endif /* BASE_STREAM_H */
