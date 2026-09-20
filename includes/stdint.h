#ifndef _STANDALONE_STDINT_H
#define _STANDALONE_STDINT_H

/* ==========================================================================
   1. Exact-width integer types
   ========================================================================== */
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__)
    /* 64-bit platforms */
    typedef long           int64_t;
    typedef unsigned long  uint64_t;
#else
    /* 32-bit platforms */
    typedef long long      int64_t;
    typedef unsigned long long uint64_t;
#endif

/* ==========================================================================
   2. Pointer-width integer types
   ========================================================================== */
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__)
    typedef long           intptr_t;
    typedef unsigned long  uintptr_t;
#else
    typedef int            intptr_t;
    typedef unsigned int   uintptr_t;
#endif

/* ==========================================================================
   3. Greatest-width integer types
   ========================================================================== */
typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

/* ==========================================================================
   4. Limits of exact-width integer types
   ========================================================================== */
#define INT8_MIN         (-127 - 1)
#define INT16_MIN        (-32767 - 1)
#define INT32_MIN        (-2147483647 - 1)
#define INT64_MIN        (-9223372036854775807LL - 1)

#define INT8_MAX         127
#define INT16_MAX        32767
#define INT32_MAX        2147483647
#define INT64_MAX        9223372036854775807LL

#define UINT8_MAX        255
#define UINT16_MAX       65535
#define UINT32_MAX       4294967295U
#define UINT64_MAX       18446744073709551615ULL

/* ==========================================================================
   5. Limits of pointer and greatest-width types
   ========================================================================== */
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__LP64__)
    #define INTPTR_MIN   INT64_MIN
    #define INTPTR_MAX   INT64_MAX
    #define UINTPTR_MAX  UINT64_MAX
#else
    #define INTPTR_MIN   INT32_MIN
    #define INTPTR_MAX   INT32_MAX
    #define UINTPTR_MAX  UINT32_MAX
#endif

#define INTMAX_MIN       INT64_MIN
#define INTMAX_MAX       INT64_MAX
#define UINTMAX_MAX      UINT64_MAX

/* ==========================================================================
   6. Macros for integer constants
   ========================================================================== */
#define INT8_C(c)        c
#define INT16_C(c)       c
#define INT32_C(c)       c
#define INT64_C(c)       c ## LL

#define UINT8_C(c)       c
#define UINT16_C(c)      c
#define UINT32_C(c)      c ## U
#define UINT64_C(c)      c ## ULL

#define INTMAX_C(c)      c ## LL
#define UINTMAX_C(c)     c ## ULL

#endif /* _STANDALONE_STDINT_H */

