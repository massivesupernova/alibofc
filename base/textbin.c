#include "base/textbin.h"

// [00][00 0000]
// [00 0000] => 0010 0000 ~ 0101 1111 (0x20 ~ 0x5F)
// [00] => 0110 00XX (0x6X) | 0111 XXXX (0x7X)

void TextBinEncode(Input* in, Output* out) {
  Byte ch[8] = {0};
  Byte* start = ch + 4;
  Byte* cur = 0;
  Int nchars = 0;
  while (Read(in, ch, 1)) {
    cur = start;
    if (ch[0] & 0xC0) {
      nchars = Read(in, ch+1, 1);
      if (nchars && (ch[1] & 0xC0)) {
        *cur++ = (0x70 | ((ch[1] & 0xC0) >> 4) | (ch[0] >> 6));
        *cur++ = (ch[0] & 0x3F) + 0x20;
        *cur++ = (ch[1] & 0x3F) + 0x20;
      }
      else {
        *cur++ = (0x60 | (ch[0] >> 6));
        *cur++ = (ch[0] & 0x3F) + 0x20;
        if (nchars) {
          *cur++ = ch[1] + 0x20;
        }
      }
      Write(out, StrE(start, cur));
      if (nchars == 0) {
        return;
      }
    }
    else {
      ch[0] += 0x20;
      Write(out, StrE(ch, ch+1));
    }
  }
}

void TextBinDecode(Input* in, Output* out) {
  Byte ch[8] = {0};
  Byte* start = ch + 4;
  Byte* cur = 0;
  while (Read(in, ch, 1)) {
    cur = start;
    if (ch[0] >= 0x20 && ch[0] <= 0x5F) {
      *cur++ = ch[0] - 0x20;
    }
    else if ((ch[0] & 0xF0) == 0x60) {
      if (Read(in, ch+1, 1) != 1) {
        return; // error format
      }
      *cur++ = (ch[1] - 0x20) | (ch[0] << 6); 
    }
    else if ((ch[0] & 0xF0) == 0x70) {
      if (Read(in, ch+1, 2) != 2) {
        return; // error format
      }
      *cur++ = (ch[1] - 0x20) | (ch[0] << 6);
      *cur++ = (ch[2] - 0x20) | ((ch[0] & 0x0C) << 4);
    }
    else {
      return; // error format
    }
    Write(out, StrE(start, cur));
  }
}

void TextBinTest(void) {

}
