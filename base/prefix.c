#line 2 "base/prefix.c"
#include "base/prefix.h"

typedef union {
  ubyte a[4];
  uint32 b;
} endian_t;

tbool is_little_endian(void) {
  static endian_t value;
  if (value.b == 0) {
    value.b = 0xdeadbeef;
  }
  return (value.a[0] == 0xef);
}

const ubyte* ascii_lowerdigits(void) {
  static const ubyte lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  return lower;
}

const ubyte* ascii_upperdigits(void) {
  static const ubyte upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  return upper;
}

void test_prefix(void) {
  capi_utest(sizeof(tbool) == 1);
  capi_utest(sizeof(ubyte) == 1);
  capi_utest(sizeof(tint8) == 1);
  capi_utest(sizeof(uint8) == 1);
  capi_utest(ubyte_max == 255);
  capi_utest(uint8_max == 255);
  capi_utest(tint8_max == 127);
  capi_utest(tint8_min == -127 - 1);

  capi_utest(sizeof(tint16) == 2);
  capi_utest(sizeof(uint16) == 2);
  capi_utest(uint16_max == 65535);
  capi_utest(tint16_max == 32767);
  capi_utest(tint16_min == -32767 - 1);
 
  capi_utest(sizeof(tint32) == 4);
  capi_utest(sizeof(uint32) == 4);
  capi_utest(uint32_max == 4294967295);
  capi_utest(tint32_max == 2147483647);
  capi_utest(tint32_min == -2147483647 - 1);

  capi_utest(sizeof(tint64) == 8);
  capi_utest(sizeof(uint64) == 8);
  capi_utest(uint64_max == 18446744073709551615);
  capi_utest(tint64_max == 9223372036854775807);
  capi_utest(tint64_min == -9223372036854775807 - 1);

  capi_utest(sizeof(tint) == sizeof(void*));
  capi_utest(sizeof(uint) == sizeof(void*));

#if platform_32_bit
  capi_utest(uint_max == uint32_max);
  capi_utest(tint_max == tint32_max);
  capi_utest(tint_min == tint32_min);
#elif platform_64_bit
  capi_utest(uint_max == uint64_max);
  capi_utest(tint_max == tint64_max);
  capi_utest(tint_min == tint64_max);
#else
#error "platform unsupported"
#endif

#if localhost_little_endian
  capi_utest(is_little_endian());
#else
  capi_utest(!is_little_endian());
#endif
  
  capi_utest(KNull == 0x00);
  capi_utest(KFalse == 0x00);
  capi_utest(KTrue == 0x01);
  capi_utest(KBell == '\a');
  capi_utest(KBspace == '\b');
  capi_utest(KTab == '\t');
  capi_utest(KEnter == '\n');
  capi_utest(KNline == '\n');
  capi_utest(KVtab == '\v');
  capi_utest(KNpage == '\f');
  capi_utest(KRet == '\r');
  capi_utest(KSpace == ' ');
  capi_utest(KDel == 0x7F);
  capi_utest(KEsc == 0x1B);

  capi_utest(KExclam == '!');
  capi_utest(KDquote == '"');
  capi_utest(KHash == '#');
  capi_utest(KDollar == '$');
  capi_utest(KPercent == '%');
  capi_utest(KAmp == '&');
  capi_utest(KSquote == '\'');
  capi_utest(KLparen == '(');
  capi_utest(KRparen == ')');
  capi_utest(KAsterisk == '*');
  capi_utest(KPlus == '+');
  capi_utest(KComma == ',');
  capi_utest(KMinus == '-');
  capi_utest(KPeriod == '.');
  capi_utest(KSlash == '/');
  capi_utest(KColon == ':');
  capi_utest(KScolon == ';');
  capi_utest(KLess == '<');
  capi_utest(KEqual == '=');
  capi_utest(KGreat == '>');
  capi_utest(KQuestion == '?');
  capi_utest(KAt == '@');
  capi_utest(KLbracket == '[');
  capi_utest(KBslash == '\\');
  capi_utest(KRbracket == ']');
  capi_utest(KCaret  == '^');
  capi_utest(KUscore == '_');
  capi_utest(KBquote == '`');
  capi_utest(KLcurl == '{');
  capi_utest(KVbar == '|');
  capi_utest(KRcurl == '}');
  capi_utest(KTilde == '~');
  capi_utest(KFileEnd == 0xFFFF);
}
