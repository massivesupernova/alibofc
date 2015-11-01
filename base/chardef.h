#ifndef BASE_CHARDEF_H
#define BASE_CHARDEF_H

#include "base/prefix.h"

#define IsNUL(ch) ((ch) == 0x00) // null character '\0'
#define IsSOH(ch) ((ch) == 0x01) // start of heading
#define IsSTX(ch) ((ch) == 0x02) // start of text
#define IsETX(ch) ((ch) == 0x03) // end of text
#define IsEOT(ch) ((ch) == 0x04) // end of transmission
#define IsENQ(ch) ((ch) == 0x05) // enquiry
#define IsACK(ch) ((ch) == 0x06) // acknowledge
#define IsBEL(ch) ((ch) == 0x07) // bell '\a'
#define IsBSP(ch) ((ch) == 0x08) // backspace '\b'
#define IsTAB(ch) ((ch) == 0x09) // horizontal tab (HT) '\t'
#define IsENT(ch) ((ch) == 0x0A) // line feed (LF), new line (NL), end of line (EOL), enter '\n'
#define IsVTB(ch) ((ch) == 0x0B) // vertical tab (VT) '\v'
#define IsFFD(ch) ((ch) == 0x0C) // form feed (FF), new page (NP) '\f'
#define IsRET(ch) ((ch) == 0x0D) // carriage return (CR) '\r'
#define IsSOT(ch) ((ch) == 0x0E) // shift out
#define IsSIN(ch) ((ch) == 0x0F) // shift in
#define IsDLE(ch) ((ch) == 0x10) // data link escape
#define IsDC1(ch) ((ch) == 0x11) // device control 1
#define IsDC2(ch) ((ch) == 0x12) // device control 2
#define IsDC3(ch) ((ch) == 0x13) // device control 3
#define IsDC4(ch) ((ch) == 0x14) // device control 4
#define IsNAK(ch) ((ch) == 0x15) // negative acknowledge
#define IsSYN(ch) ((ch) == 0x16) // synchronous idle
#define IsETB(ch) ((ch) == 0x17) // end of transmit block
#define IsCAN(ch) ((ch) == 0x18) // cancel
#define IsEME(ch) ((ch) == 0x19) // end of medium
#define IsSUB(ch) ((ch) == 0x1A) // substitute 
#define IsESC(ch) ((ch) == 0x1B) // escape
#define IsFSP(ch) ((ch) == 0x1C) // file separator
#define IsGSP(ch) ((ch) == 0x1D) // group separator
#define IsRSP(ch) ((ch) == 0x1E) // record separator
#define IsUSP(ch) ((ch) == 0x00) // unit separator
#define IsSPC(ch) ((ch) == 0x20) // the space character
#define IsDEL(ch) ((ch) == 0x7F) // delete

#define KNull     (0x00)
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

static const Byte GHexDigitLower[17] = "0123456789abcdef";
static const Byte GHexDigitUpper[17] = "0123456789ABCDEF";

#define CaseNumber case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'

// use inline not marco is because: macro can only use macro arg once to avoid side effect
STATIC_INLINE Bool IsCtrl(Uint16 ch) { return (((ch) >= 0x00 && (ch) <= 0x1F) || ((ch) == 0x7F)); }
STATIC_INLINE Bool IsPrint(Uint16 ch) { return (((ch) >= 0x20 && (ch) <= 0x7E)); }
STATIC_INLINE Bool IsPunct(Uint16 ch) { return (((ch) >= 0x21 && (ch) <= 0x2F) || ((ch) >= 0x3A && (ch) <= 0x40) || ((ch) >= 0x5B && (ch) <= 0x60) || ((ch) >= 0x7B && (ch) <= 0x7E)); }
STATIC_INLINE Bool IsNumber(Uint16 ch) { return (((ch) >= 0x30 && (ch) <= 0x39)); }  
STATIC_INLINE Bool IsLetter(Uint16 ch) { return (((ch) >= 0x41 && (ch) <= 0x5A) || ((ch) >= 0x61 && (ch) <= 0x7A)); }
STATIC_INLINE Bool IsUpper(Uint16 ch) { return (((ch) >= 0x41 && (ch) <= 0x5A)); } 
STATIC_INLINE Bool IsLower(Uint16 ch) { return (((ch) >= 0x61 && (ch) <= 0x7A)); }
STATIC_INLINE Bool IsAlphaNum(Uint16 ch) { return (IsLetter(ch) || IsNumber(ch)); }
STATIC_INLINE Bool IsHexDigit(Uint16 ch) { return (IsNumber(ch) || ((ch) >= 0x41 && (ch) <= 0x46) || ((ch) >= 0x61 && (ch) <= 0x66)); }
STATIC_INLINE Bool EndOfTextFile(Uint16 ch) { return ((((Uint16)(ch)) == (Uint16)KFileEnd) || (ch) == 0); } // use different style of name to avoid wrong use with IsTextLineEnd
STATIC_INLINE Bool IsTextLineEnd(Uint16 ch) { return (EndOfTextFile(ch) || (ch) == KRet || (ch) == KNline); } 
STATIC_INLINE Bool IsWhiteSpace(Uint16 ch) { return (((ch) == KTab || (ch) == KVtab || (ch) == KNpage || (ch) == KSpace) || IsTextLineEnd(ch)); }

#endif /* BASE_CHARDEF_H */
