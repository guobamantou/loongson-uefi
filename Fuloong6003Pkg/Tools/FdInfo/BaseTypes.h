#ifndef __CALCULATE_ZERO_VECTOR_BASE_TYPES_H__
#define __CALCULATE_ZERO_VECTOR_BASE_TYPES_H__


#if !defined(__GNUC__)
#pragma pack()
#endif

#if !defined(__GNUC__) && (__STDC_VERSION__ < 199901L)
  //
  // No ANSI C 2000 stdint.h integer width declarations, so define equivalents
  //
 
  #if defined(_MSC_EXTENSIONS)
    ///
    /// 8-byte unsigned value
    ///
    typedef unsigned __int64    UINT64;
    ///
    /// 8-byte signed value
    ///
    typedef __int64             INT64;
    ///
    /// 4-byte unsigned value
    ///
    typedef unsigned __int32    UINT32;
    ///
    /// 4-byte signed value
    ///
    typedef __int32             INT32;
    ///
    /// 2-byte unsigned value
    ///
    typedef unsigned short      UINT16;
    ///
    /// 2-byte Character.  Unless otherwise specified all strings are stored in the
    /// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
    ///
    typedef unsigned short      CHAR16;
    ///
    /// 2-byte signed value
    ///
    typedef short               INT16;
    ///
    /// Logical Boolean.  1-byte value containing 0 for FALSE or a 1 for TRUE.  Other
    /// values are undefined.
    ///
    typedef unsigned char       BOOLEAN;
    ///
    /// 1-byte unsigned value
    ///
    typedef unsigned char       UINT8;
    ///
    /// 1-byte Character
    ///
    typedef char                CHAR8;
    ///
    /// 1-byte signed value
    ///
    typedef char                INT8;
  #else

    //
    // Assume standard IA-32 alignment. 
    // Need to check portability of long long
    //

    ///
    /// 8-byte unsigned value
    ///
    typedef unsigned long long  UINT64;
    ///
    /// 8-byte signed value
    ///
    typedef long long           INT64;
    ///
    /// 4-byte unsigned value
    ///
    typedef unsigned int        UINT32;
    ///
    /// 4-byte signed value
    ///
    typedef int                 INT32;
    ///
    /// 2-byte unsigned value
    ///
    typedef unsigned short      UINT16;
    ///
    /// 2-byte Character.  Unless otherwise specified all strings are stored in the
    /// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
    ///
    typedef unsigned short      CHAR16;
    ///
    /// 2-byte signed value
    ///
    typedef short               INT16;
    ///
    /// Logical Boolean.  1-byte value containing 0 for FALSE or a 1 for TRUE.  Other
    /// values are undefined.
    ///
    typedef unsigned char       BOOLEAN;
    ///
    /// 1-byte unsigned value
    ///
    typedef unsigned char       UINT8;
    ///
    /// 1-byte Character
    ///
    typedef char                CHAR8;
    ///
    /// 1-byte signed value
    ///
    typedef char                INT8;
  #endif
#else
  //
  // Use ANSI C 2000 stdint.h integer width declarations
  //
  #include <stdint.h>

  ///
  /// Logical Boolean.  1-byte value containing 0 for FALSE or a 1 for TRUE.  Other
  /// values are undefined.
  ///
  typedef uint8_t   BOOLEAN;
  ///
  /// 1-byte signed value
  ///
  typedef int8_t    INT8;
  ///
  /// 1-byte unsigned value
  ///
  typedef uint8_t   UINT8;
  ///
  /// 2-byte signed value
  ///
  typedef int16_t   INT16;
  ///
  /// 2-byte unsigned value
  ///
  typedef uint16_t  UINT16;
  ///
  /// 4-byte signed value
  ///
  typedef int32_t   INT32;
  ///
  /// 4-byte unsigned value
  ///
  typedef uint32_t  UINT32;
  ///
  /// 8-byte signed value
  ///
  typedef int64_t   INT64;
  ///
  /// 8-byte unsigned value
  ///
  typedef uint64_t  UINT64;
  ///
  /// 1-byte Character
  ///
  typedef char      CHAR8;
  ///
  /// 2-byte Character.  Unless otherwise specified all strings are stored in the
  /// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
  ///
  typedef uint16_t  CHAR16;

#endif

///
/// Unsigned value of native width.  (4 bytes on supported 32-bit processor instructions,
/// 8 bytes on supported 64-bit processor instructions)
///
typedef UINT64  UINTN;
///
/// Signed value of native width.  (4 bytes on supported 32-bit processor instructions,
/// 8 bytes on supported 64-bit processor instructions)
///
typedef INT64   INTN;

typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} GUID;
typedef GUID EFI_GUID;

#define SIGNATURE_16(A, B)        ((A) | (B << 8))
#define SIGNATURE_32(A, B, C, D)  (SIGNATURE_16 (A, B) | (SIGNATURE_16 (C, D) << 16))

#endif