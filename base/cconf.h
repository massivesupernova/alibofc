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

#if 0
_MSC_VER COMPILER_MSC
_WINDOWS GUI_APPLICATION
_CONSOLE CUI_APPLICATION
__CYGWIN__
__CYGWIN32__
__MINGW32__
__GNUC__
__APPLE_CC__
__STDC_VERSION__
__INTEL_COMPILER
__SUNPRO_CC
__IBMCPP__

#ifdef __linux__
#include "socket_epoll.h"
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#include "socket_kqueue.h"
#endif

/* This directive is currently not supported on Mac OS X (it will give
 * a compiler error), since compile-time TLS is not supported in the Mac OS X
 * executable format. Also, some older versions of MinGW (before GCC 4.x) do
 * not support this directive.
 */
/* FIXME: Check for a PROPER value of __STDC_VERSION__ to know if we have C11 */
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201102L)) && !defined(_Thread_local)
 #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
  #define _Thread_local __thread
 #else
  #define _Thread_local __declspec(thread)
 #endif
#endif

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
//WINVER – used to enable features only available in newer operating systems.
//Define it to 0x0501 for Windows XP, and 0x0600 for Windows Vista.
#include <windows.h>

// https://msdn.microsoft.com/en-us/library/aa384198(VS.85).aspx

#if defined(_WIN16)
#define PLATFORM_WIN16 1
#else
#define PLATFORM_WIN16 0
#endif

#if defined(_WIN32)
#define PLATFORM_WIN32 1
#else
#define PLATFORM_WIN32 0
#endif

/* _WIN32 is also defined by the 64-bit compiler for backward compatibility */
#if defined(_WIN64)
#define PLATFORM_WIN64 1
#else
#define PLATFORM_WIN64 0
#endif

#if defined(_M_IX86)
#define ARCHITECTURE_X86 1
#else
#define ARCHITECTURE_X86 0
#endif

#if defined(_M_X64)
#define ARCHITECTURE_X64 1
#else
#define ARCHITECTURE_X64 0
#endif

/* Intel Itanium platform */
#if defined(_M_IA64)
#define ARCHITECTURE_IA64 1
#else
#define ARCHITECTURE_IA64 0
#endif

#endif

#endif // BASE_CCONF_H
