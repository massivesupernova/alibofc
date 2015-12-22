#ifndef BASE_PREFIX_H
#define BASE_PREFIX_H
#include "base/cconf.h"

capidecl(const) ubyte ubyte_max = 0xFF;
capidecl(const) uint8 uint8_max = 0xFF;
capidecl(const) tint8 tint8_max = 0x7F;
capidecl(const) tint8 tint8_min = 0x80;
capidecl(const) uint16 uint16_max = 0xFFFF;
capidecl(const) tint16 tint16_max = 0x7FFF;
capidecl(const) tint16 tint16_min = 0x8000;
capidecl(const) uint32 uint32_max = 0xFFFFFFFF;
capidecl(const) tint32 tint32_max = 0x7FFFFFFF;
capidecl(const) tint32 tint32_min = 0x80000000;
capidecl(const) uint64 uint64_max = 0xFFFFFFFFFFFFFFFF;
capidecl(const) tint64 tint64_max = 0x7FFFFFFFFFFFFFFF;
capidecl(const) tint64 tint64_min = 0x8000000000000000;

#if platform_32_bit
typedef tint32 tint;
typedef uint32 uint;
capidecl(const) uint uint_max = 0xFFFFFFFF;
capidecl(const) tint tint_max = 0x7FFFFFFF;
capidecl(const) tint tint_min = 0x80000000;
#elif platform_64_bit
typedef tint64 tint;
typedef uint64 uint;
capidecl(const) uint uint_max = 0xFFFFFFFFFFFFFFFF;
capidecl(const) tint tint_max = 0x7FFFFFFFFFFFFFFF;
capidecl(const) tint tint_min = 0x8000000000000000;
#else
#error "platform unsupported"
#endif

#undef capi_loge
#undef capi_exit
#undef capi_utest
#undef capi_check
#undef capi_logd

#define capi_loge(fmt, ...) xcapilog("[LOGE] %s Ln %d " fmt " \n", __FILE__, __LINE__, ## __VA_ARGS__)
#define capi_exit(fmt, ...) xcapiexit("[EXIT] %s Ln %d " fmt " \n", __FILE__, __LINE__, ## __VA_ARGS__)
#define capi_utest(expr) ((expr) ? ((void)0) : xcapilog("[TEST] %s Ln %d (" #expr ") faild\n", __FILE__, __LINE__))

#if defined(CAPI_DEBUG)
#define capi_check(expr) ((expr) ? ((void)0) : xcapiexit("[CHCK] %s Ln %d (" #expr "): faild\n", __FILE__, __LINE__))
#define capi_logd(fmt, ...) xcapilog("[LOGD] %s Ln %d " fmt " \n", __FILE__, __LINE__, ## __VA_ARGS__)
#else
#define capi_check(expr)
#define capi_logd(fmt, ...)
#endif

#define KNull     (0x00)
#define KFalse    (0x00)
#define KTrue     (0x01)
#define KBell     (0x07)
#define KBspace   (0x08)
#define KTab      (0x09)
#define KEnter    (0x0A)
#define KNline    (0x0A)
#define KVtab     (0x0B)
#define KNpage    (0x0C)
#define KRet      (0x0D)
#define KSpace    (0x20)
#define KDel      (0x7F)
#define KEsc      (0x1B)

#define KExclam   ('!') //0x21
#define KDquote   ('"')
#define KHash     ('#')
#define KDollar   ('$')
#define KPercent  ('%')
#define KAmp      ('&')
#define KSquote   ('\'')
#define KLparen   ('(')
#define KRparen   (')')
#define KAsterisk ('*')
#define KPlus     ('+')
#define KComma    (',')
#define KMinus    ('-')
#define KPeriod   ('.')
#define KSlash    ('/') //0x2F
#define KColon    (':') //0x3A
#define KScolon   (';')
#define KLess     ('<')
#define KEqual    ('=')
#define KGreat    ('>')
#define KQuestion ('?')
#define KAt       ('@') //0x40
#define KLbracket ('[') //0x5B
#define KBslash   ('\\')
#define KRbracket (']')
#define KCaret    ('^')
#define KUscore   ('_')
#define KBquote   ('`') //0x60
#define KLcurl    ('{') //0x7B
#define KVbar     ('|')
#define KRcurl    ('}')
#define KTilde    ('~') //0x7E
#define KFileEnd  (0xFFFF)

capidecl(inline) tbool ascii_ctrl(uint16 ch) { 
  return (((ch) >= 0x00 && (ch) <= 0x1F) || ((ch) == 0x7F)); 
}
capidecl(inline) tbool ascii_print(uint16 ch) { 
  return (((ch) >= 0x20 && (ch) <= 0x7E)); 
}
capidecl(inline) tbool ascii_punct(uint16 ch) { 
  return (((ch) >= 0x21 && (ch) <= 0x2F) || ((ch) >= 0x3A && (ch) <= 0x40) || 
          ((ch) >= 0x5B && (ch) <= 0x60) || ((ch) >= 0x7B && (ch) <= 0x7E)); 
}
capidecl(inline) tbool ascii_number(uint16 ch) { 
  return (((ch) >= 0x30 && (ch) <= 0x39)); 
}  
capidecl(inline) tbool ascii_letter(uint16 ch) { 
  return (((ch) >= 0x41 && (ch) <= 0x5A) || ((ch) >= 0x61 && (ch) <= 0x7A)); 
}
capidecl(inline) tbool ascii_upper(uint16 ch) { 
  return (((ch) >= 0x41 && (ch) <= 0x5A)); 
} 
capidecl(inline) tbool ascii_lower(uint16 ch) { 
  return (((ch) >= 0x61 && (ch) <= 0x7A)); 
}
capidecl(inline) tbool ascii_alphanum(uint16 ch) { 
  return (ascii_letter(ch) || ascii_number(ch)); 
}
capidecl(inline) tbool ascii_hexdigit(uint16 ch) { 
  return (ascii_number(ch) || ((ch) >= 0x41 && (ch) <= 0x46) || ((ch) >= 0x61 && (ch) <= 0x66)); 
}
capidecl(inline) tbool ascii_textfileend(uint16 ch) {
  return ((ch) == (uint16)KFileEnd || (ch) == 0); 
} 
capidecl(inline) tbool ascii_textlineend(uint16 ch) { 
  return (ascii_textfileend(ch) || (ch) == KRet || (ch) == KNline); 
} 
capidecl(inline) tbool ascii_noeolspace(uint16 ch) {
  return ((ch) == KTab || (ch) == KVtab || (ch) == KNpage || (ch) == KSpace);
}
capidecl(inline) tbool ascii_whitespace(uint16 ch) { 
  return (ascii_noeolspace(ch) || ascii_textlineend(ch)); 
}

capidecl(export) const ubyte* ascii_lowerdigits(void);
capidecl(export) const ubyte* ascii_upperdigits(void);

typedef struct {
  const ubyte* start;
  const ubyte* end;
} cstr;

typedef struct {
  ubyte* start;
  ubyte* end;
} tstr;

capidecl(export) cstr cstrz(const char* cstr);
capidecl(export) cstr cstre(const void* start, const void* end);
capidecl(export) cstr cstrn(const void* start, tint n);
capidecl(export) tstr tstrz(char* cstr);
capidecl(export) tstr tstre(void* start, void* end);
capidecl(export) tstr tstrn(void* start, tint n);

capidecl(export) tbool is_little_endian(void);

capidecl(export) void xcapilog(const char* fmt, ...);
capidecl(export) void xcapiexit(const char* fmt, ...);
capidecl(export) void test_prefix(void);

#endif /* BASE_PREFIX_H */
