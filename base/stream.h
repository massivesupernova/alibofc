#ifndef BASE_STREAM_H
#define BASE_STREAM_H

#include "base/prefix.h"
#include "base/string.h"

struct InputImpl;
typedef struct {
  struct InputImpl* impl;
  Byte* (*get)(struct InputImpl* impl);
} Input;

STATIC_INLINE Byte* Get(Input* self) {
  return self->get(self->impl);
}

struct OutputImpl;
typedef struct {
  struct OutputImpl* impl;
  Int (*put)(struct OutputImpl* impl, Tstr str);
} Output;

STATIC_INLINE Int Put(Output* self, Tstr str) {
  return self->put(self->impl, str);
}

#endif /* BASE_STREAM_H */
