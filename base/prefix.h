#ifndef BASE_PREFIX_H
#define BASE_PREFIX_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>

// Primary types
#define False (0)
#define True (1)
typedef unsigned char Bool;
typedef unsigned char Byte;
typedef signed char Int8;
typedef unsigned char Uint8;
typedef int16_t Int16;
typedef uint16_t Uint16;
typedef int32_t Int32;
typedef uint32_t Uint32;
typedef int64_t Int64;
typedef uint64_t Uint64;
typedef intptr_t Int;
typedef uintptr_t Uint;
typedef intptr_t Iptr;
typedef uintptr_t Uptr;

// Thread Local (From boost thread_local_alloc.h)
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

#if defined(USE_COMPILER_TLS)
  #define THREAD_LOCAL __thread
#elif defined(USE_WIN32_COMPILER_TLS)
  #define THREAD_LOCAL __declspec(thread)
#else
  #define THREAD_LOCAL 
#endif

#define STATIC_INLINE static
#define EXTERN_API extern

EXTERN_API void PrefixTest(void);

#endif /* BASE_PREFIX_H */
