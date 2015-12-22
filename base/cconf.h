#ifndef BASE_CCONF_H
#define BASE_CCONF_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>

// thread local (from boost thread_local_alloc.h)
#if defined(_WIN32_WCE)
  #if defined(x86)
    #define I386
  #endif
  #if defined(ARM)
    #define ARM32
  #endif
  #define MSWINCE
#else
  #if (defined(_MSDOS) || defined(_MSC_VER)) && (_M_IX86 >= 300) || defined(_WIN32) && !defined(__CYGWIN32__) && !defined(__CYGWIN__)
    #if defined(__LP64__) || defined(_WIN64)
      #define X86_64
    #else
      #define I386
    #endif
    #define MSWIN32	/* or Win64 */
  #endif
  #if defined(_MSC_VER) && defined(_M_IA64)
    #define IA64
    #define MSWIN32	/* Really win64, but we don't treat 64-bit 	*/
			/* variants as a differnt platform.		*/
  #endif
#endif

#if defined(__MINGW32__)
  #define I386
  #define MSWIN32
#endif

#if defined(__BORLANDC__)
  #define I386
  #define MSWIN32
# endif

#if defined(__CYGWIN32__) || defined(__CYGWIN__)
  #define I386
  #define CYGWIN32
#endif

#if defined(MSWIN32) || defined(MSWINCE) || defined(CYGWIN32)
  #if defined(__GNUC__)  /* Fixed for versions past 2.95? */
    #define USE_WIN32_SPECIFIC
  #else
    #define USE_WIN32_COMPILER_TLS
  #endif /* !GNU */
#elif defined(LINUX) && !defined(ARM32) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >=3))
  #define USE_COMPILER_TLS
#elif (defined(GC_DGUX386_THREADS) || defined(GC_OSF1_THREADS) || defined(GC_DARWIN_THREADS) || defined(GC_AIX_THREADS)) || defined(GC_NETBSD_THREADS)
  #define USE_PTHREAD_SPECIFIC
#elif defined(GC_HPUX_THREADS)
  #ifdef __GNUC__
    #define USE_PTHREAD_SPECIFIC
        /* Empirically, as of gcc 3.3, USE_COMPILER_TLS doesn't work.	*/
  #else
    #define USE_COMPILER_TLS
  #endif
#else
  #define USE_CUSTOM_SPECIFIC  /* Use our own.	*/
#endif

#undef capidecl_thread
#undef capidecl_inline
#undef capidecl_const
#undef capidecl_export
#undef capidecl

#if defined(USE_COMPILER_TLS)
  #define capidecl_thread __thread
#elif defined(USE_WIN32_COMPILER_TLS)
  #define capidecl_thread __declspec(thread)
#else
  #define capidecl_thread 
#endif

#define capidecl_inline static
#define capidecl_const static const
#define capidecl_export extern
#define capidecl(specific) capidecl_##specific

typedef unsigned char tbool;
typedef unsigned char ubyte;
typedef signed char tint8;
typedef unsigned char uint8;
typedef int16_t tint16;
typedef uint16_t uint16;
typedef int32_t tint32;
typedef uint32_t uint32;
typedef int64_t tint64;
typedef uint64_t uint64;

#undef localhost_little_endian
#define localhost_little_endian (1)

#undef platform_32_bit
#undef platform_64_bit
#define platform_32_bit 1
#define platform_64_bit 0

#endif // BASE_CCONF_H
