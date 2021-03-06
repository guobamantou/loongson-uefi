#ifndef _NPY_COMMON_H_
#define _NPY_COMMON_H_

/* This is auto-generated */
#include "numpyconfig.h"

/* enums for detected endianness */
enum {
	NPY_CPU_UNKNOWN_ENDIAN,
	NPY_CPU_LITTLE,
	NPY_CPU_BIG,
};

/* Some platforms don't define bool, long long, or long double.
   Handle that here.
*/

#define NPY_BYTE_FMT "hhd"
#define NPY_UBYTE_FMT "hhu"
#define NPY_SHORT_FMT "hd"
#define NPY_USHORT_FMT "hu"
#define NPY_INT_FMT "d"
#define NPY_UINT_FMT "u"
#define NPY_LONG_FMT "ld"
#define NPY_ULONG_FMT "lu"
#define NPY_FLOAT_FMT "g"
#define NPY_DOUBLE_FMT "g"

#ifdef PY_LONG_LONG
typedef PY_LONG_LONG npy_longlong;
typedef unsigned PY_LONG_LONG npy_ulonglong;
#  ifdef _MSC_VER
#    define NPY_LONGLONG_FMT         "I64d"
#    define NPY_ULONGLONG_FMT        "I64u"
#    define NPY_LONGLONG_SUFFIX(x)   (x##i64)
#    define NPY_ULONGLONG_SUFFIX(x)  (x##Ui64)
#  else
        /* #define LONGLONG_FMT   "lld"      Another possible variant
           #define ULONGLONG_FMT  "llu"

           #define LONGLONG_FMT   "qd"   -- BSD perhaps?
           #define ULONGLONG_FMT   "qu"
        */
#    define NPY_LONGLONG_FMT         "Ld"
#    define NPY_ULONGLONG_FMT        "Lu"
#    define NPY_LONGLONG_SUFFIX(x)   (x##LL)
#    define NPY_ULONGLONG_SUFFIX(x)  (x##ULL)
#  endif
#else
typedef long npy_longlong;
typedef unsigned long npy_ulonglong;
#  define NPY_LONGLONG_SUFFIX(x)  (x##L)
#  define NPY_ULONGLONG_SUFFIX(x) (x##UL)
#endif


typedef unsigned char npy_bool;
#define NPY_FALSE 0
#define NPY_TRUE 1


#if NPY_SIZEOF_LONGDOUBLE == NPY_SIZEOF_DOUBLE
        typedef double npy_longdouble;
        #define NPY_LONGDOUBLE_FMT "g"
#else
        typedef long double npy_longdouble;
        #define NPY_LONGDOUBLE_FMT "Lg"
#endif

#ifndef Py_USING_UNICODE
#error Must use Python with unicode enabled.
#endif


typedef signed char npy_byte;
typedef unsigned char npy_ubyte;
typedef unsigned short npy_ushort;
typedef unsigned int npy_uint;
typedef unsigned long npy_ulong;

/* These are for completeness */
typedef float npy_float;
typedef double npy_double;
typedef short npy_short;
typedef int npy_int;
typedef long npy_long;

typedef struct { float real, imag; } npy_cfloat;
typedef struct { double real, imag; } npy_cdouble;
typedef struct {npy_longdouble real, imag;} npy_clongdouble;

/*
 * numarray-style bit-width typedefs
 */
#define NPY_MAX_INT8 127
#define NPY_MIN_INT8 -128
#define NPY_MAX_UINT8 255
#define NPY_MAX_INT16 32767
#define NPY_MIN_INT16 -32768
#define NPY_MAX_UINT16 65535
#define NPY_MAX_INT32 2147483647
#define NPY_MIN_INT32 (-NPY_MAX_INT32 - 1)
#define NPY_MAX_UINT32 4294967295U
#define NPY_MAX_INT64 NPY_LONGLONG_SUFFIX(9223372036854775807)
#define NPY_MIN_INT64 (-NPY_MAX_INT64 - NPY_LONGLONG_SUFFIX(1))
#define NPY_MAX_UINT64 NPY_ULONGLONG_SUFFIX(18446744073709551615)
#define NPY_MAX_INT128 NPY_LONGLONG_SUFFIX(85070591730234615865843651857942052864)
#define NPY_MIN_INT128 (-NPY_MAX_INT128 - NPY_LONGLONG_SUFFIX(1))
#define NPY_MAX_UINT128 NPY_ULONGLONG_SUFFIX(170141183460469231731687303715884105728)
#define NPY_MAX_INT256 NPY_LONGLONG_SUFFIX(57896044618658097711785492504343953926634992332820282019728792003956564819967)
#define NPY_MIN_INT256 (-NPY_MAX_INT256 - NPY_LONGLONG_SUFFIX(1))
#define NPY_MAX_UINT256 NPY_ULONGLONG_SUFFIX(115792089237316195423570985008687907853269984665640564039457584007913129639935)

        /* Need to find the number of bits for each type and
           make definitions accordingly.

           C states that sizeof(char) == 1 by definition

           So, just using the sizeof keyword won't help.

           It also looks like Python itself uses sizeof(char) quite a
           bit, which by definition should be 1 all the time.

           Idea: Make Use of CHAR_BIT which should tell us how many
           BITS per CHARACTER
        */

        /* Include platform definitions -- These are in the C89/90 standard */
#include <limits.h>
#define NPY_MAX_BYTE SCHAR_MAX
#define NPY_MIN_BYTE SCHAR_MIN
#define NPY_MAX_UBYTE UCHAR_MAX
#define NPY_MAX_SHORT SHRT_MAX
#define NPY_MIN_SHORT SHRT_MIN
#define NPY_MAX_USHORT USHRT_MAX
#define NPY_MAX_INT   INT_MAX
#ifndef INT_MIN
#define INT_MIN (-INT_MAX - 1)
#endif
#define NPY_MIN_INT   INT_MIN
#define NPY_MAX_UINT  UINT_MAX
#define NPY_MAX_LONG  LONG_MAX
#define NPY_MIN_LONG  LONG_MIN
#define NPY_MAX_ULONG  ULONG_MAX

#define NPY_BITSOF_BOOL (sizeof(npy_bool)*CHAR_BIT)
#define NPY_BITSOF_CHAR CHAR_BIT
#define NPY_BITSOF_SHORT (NPY_SIZEOF_SHORT * CHAR_BIT)
#define NPY_BITSOF_INT (NPY_SIZEOF_INT * CHAR_BIT)
#define NPY_BITSOF_LONG (NPY_SIZEOF_LONG * CHAR_BIT)
#define NPY_BITSOF_LONGLONG (NPY_SIZEOF_LONGLONG * CHAR_BIT)
#define NPY_BITSOF_FLOAT (NPY_SIZEOF_FLOAT * CHAR_BIT)
#define NPY_BITSOF_DOUBLE (NPY_SIZEOF_DOUBLE * CHAR_BIT)
#define NPY_BITSOF_LONGDOUBLE (NPY_SIZEOF_LONGDOUBLE * CHAR_BIT)

#if NPY_BITSOF_LONG == 8
#define NPY_INT8 NPY_LONG
#define NPY_UINT8 NPY_ULONG
        typedef long npy_int8;
        typedef unsigned long npy_uint8;
#define PyInt8ScalarObject PyLongScalarObject
#define PyInt8ArrType_Type PyLongArrType_Type
#define PyUInt8ScalarObject PyULongScalarObject
#define PyUInt8ArrType_Type PyULongArrType_Type
#define NPY_INT8_FMT NPY_LONG_FMT
#define NPY_UINT8_FMT NPY_ULONG_FMT
#elif NPY_BITSOF_LONG == 16
#define NPY_INT16 NPY_LONG
#define NPY_UINT16 NPY_ULONG
        typedef long npy_int16;
        typedef unsigned long npy_uint16;
#define PyInt16ScalarObject PyLongScalarObject
#define PyInt16ArrType_Type PyLongArrType_Type
#define PyUInt16ScalarObject PyULongScalarObject
#define PyUInt16ArrType_Type PyULongArrType_Type
#define NPY_INT16_FMT NPY_LONG_FMT
#define NPY_UINT16_FMT NPY_ULONG_FMT
#elif NPY_BITSOF_LONG == 32
#define NPY_INT32 NPY_LONG
#define NPY_UINT32 NPY_ULONG
        typedef long npy_int32;
        typedef unsigned long npy_uint32;
        typedef unsigned long npy_ucs4;
#define PyInt32ScalarObject PyLongScalarObject
#define PyInt32ArrType_Type PyLongArrType_Type
#define PyUInt32ScalarObject PyULongScalarObject
#define PyUInt32ArrType_Type PyULongArrType_Type
#define NPY_INT32_FMT NPY_LONG_FMT
#define NPY_UINT32_FMT NPY_ULONG_FMT
#elif NPY_BITSOF_LONG == 64
#define NPY_INT64 NPY_LONG
#define NPY_UINT64 NPY_ULONG
        typedef long npy_int64;
        typedef unsigned long npy_uint64;
#define PyInt64ScalarObject PyLongScalarObject
#define PyInt64ArrType_Type PyLongArrType_Type
#define PyUInt64ScalarObject PyULongScalarObject
#define PyUInt64ArrType_Type PyULongArrType_Type
#define NPY_INT64_FMT NPY_LONG_FMT
#define NPY_UINT64_FMT NPY_ULONG_FMT
#elif NPY_BITSOF_LONG == 128
#define NPY_INT128 NPY_LONG
#define NPY_UINT128 NPY_ULONG
        typedef long npy_int128;
        typedef unsigned long npy_uint128;
#define PyInt128ScalarObject PyLongScalarObject
#define PyInt128ArrType_Type PyLongArrType_Type
#define PyUInt128ScalarObject PyULongScalarObject
#define PyUInt128ArrType_Type PyULongArrType_Type
#define NPY_INT128_FMT NPY_LONG_FMT
#define NPY_UINT128_FMT NPY_ULONG_FMT
#endif

#if NPY_BITSOF_LONGLONG == 8
#  ifndef NPY_INT8
#    define NPY_INT8 NPY_LONGLONG
#    define NPY_UINT8 NPY_ULONGLONG
        typedef npy_longlong npy_int8;
        typedef npy_ulonglong npy_uint8;
#    define PyInt8ScalarObject PyLongLongScalarObject
#    define PyInt8ArrType_Type PyLongLongArrType_Type
#    define PyUInt8ScalarObject PyULongLongScalarObject
#    define PyUInt8ArrType_Type PyULongLongArrType_Type
#define NPY_INT8_FMT NPY_LONGLONG_FMT
#define NPY_UINT8_FMT NPY_ULONGLONG_FMT
#  endif
#  define NPY_MAX_LONGLONG NPY_MAX_INT8
#  define NPY_MIN_LONGLONG NPY_MIN_INT8
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT8
#elif NPY_BITSOF_LONGLONG == 16
#  ifndef NPY_INT16
#    define NPY_INT16 NPY_LONGLONG
#    define NPY_UINT16 NPY_ULONGLONG
        typedef npy_longlong npy_int16;
        typedef npy_ulonglong npy_uint16;
#    define PyInt16ScalarObject PyLongLongScalarObject
#    define PyInt16ArrType_Type PyLongLongArrType_Type
#    define PyUInt16ScalarObject PyULongLongScalarObject
#    define PyUInt16ArrType_Type PyULongLongArrType_Type
#define NPY_INT16_FMT NPY_LONGLONG_FMT
#define NPY_UINT16_FMT NPY_ULONGLONG_FMT
#  endif
#  define NPY_MAX_LONGLONG NPY_MAX_INT16
#  define NPY_MIN_LONGLONG NPY_MIN_INT16
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT16
#elif NPY_BITSOF_LONGLONG == 32
#  ifndef NPY_INT32
#    define NPY_INT32 NPY_LONGLONG
#    define NPY_UINT32 NPY_ULONGLONG
        typedef npy_longlong npy_int32;
        typedef npy_ulonglong npy_uint32;
        typedef npy_ulonglong npy_ucs4;
#    define PyInt32ScalarObject PyLongLongScalarObject
#    define PyInt32ArrType_Type PyLongLongArrType_Type
#    define PyUInt32ScalarObject PyULongLongScalarObject
#    define PyUInt32ArrType_Type PyULongLongArrType_Type
#define NPY_INT32_FMT NPY_LONGLONG_FMT
#define NPY_UINT32_FMT NPY_ULONGLONG_FMT
#  endif
#  define NPY_MAX_LONGLONG NPY_MAX_INT32
#  define NPY_MIN_LONGLONG NPY_MIN_INT32
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT32
#elif NPY_BITSOF_LONGLONG == 64
#  ifndef NPY_INT64
#    define NPY_INT64 NPY_LONGLONG
#    define NPY_UINT64 NPY_ULONGLONG
        typedef npy_longlong npy_int64;
        typedef npy_ulonglong npy_uint64;
#    define PyInt64ScalarObject PyLongLongScalarObject
#    define PyInt64ArrType_Type PyLongLongArrType_Type
#    define PyUInt64ScalarObject PyULongLongScalarObject
#    define PyUInt64ArrType_Type PyULongLongArrType_Type
#define NPY_INT64_FMT NPY_LONGLONG_FMT
#define NPY_UINT64_FMT NPY_ULONGLONG_FMT
#  endif
#  define NPY_MAX_LONGLONG NPY_MAX_INT64
#  define NPY_MIN_LONGLONG NPY_MIN_INT64
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT64
#elif NPY_BITSOF_LONGLONG == 128
#  ifndef NPY_INT128
#    define NPY_INT128 NPY_LONGLONG
#    define NPY_UINT128 NPY_ULONGLONG
        typedef npy_longlong npy_int128;
        typedef npy_ulonglong npy_uint128;
#    define PyInt128ScalarObject PyLongLongScalarObject
#    define PyInt128ArrType_Type PyLongLongArrType_Type
#    define PyUInt128ScalarObject PyULongLongScalarObject
#    define PyUInt128ArrType_Type PyULongLongArrType_Type
#define NPY_INT128_FMT NPY_LONGLONG_FMT
#define NPY_UINT128_FMT NPY_ULONGLONG_FMT
#  endif
#  define NPY_MAX_LONGLONG NPY_MAX_INT128
#  define NPY_MIN_LONGLONG NPY_MIN_INT128
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT128
#elif NPY_BITSOF_LONGLONG == 256
#  define NPY_INT256 NPY_LONGLONG
#  define NPY_UINT256 NPY_ULONGLONG
        typedef npy_longlong npy_int256;
        typedef npy_ulonglong npy_uint256;
#  define PyInt256ScalarObject PyLongLongScalarObject
#  define PyInt256ArrType_Type PyLongLongArrType_Type
#  define PyUInt256ScalarObject PyULongLongScalarObject
#  define PyUInt256ArrType_Type PyULongLongArrType_Type
#define NPY_INT256_FMT NPY_LONGLONG_FMT
#define NPY_UINT256_FMT NPY_ULONGLONG_FMT
#  define NPY_MAX_LONGLONG NPY_MAX_INT256
#  define NPY_MIN_LONGLONG NPY_MIN_INT256
#  define NPY_MAX_ULONGLONG NPY_MAX_UINT256
#endif

#if NPY_BITSOF_INT == 8
#ifndef NPY_INT8
#define NPY_INT8 NPY_INT
#define NPY_UINT8 NPY_UINT
        typedef int npy_int8;
        typedef unsigned int npy_uint8;
#    define PyInt8ScalarObject PyIntScalarObject
#    define PyInt8ArrType_Type PyIntArrType_Type
#    define PyUInt8ScalarObject PyUIntScalarObject
#    define PyUInt8ArrType_Type PyUIntArrType_Type
#define NPY_INT8_FMT NPY_INT_FMT
#define NPY_UINT8_FMT NPY_UINT_FMT
#endif
#elif NPY_BITSOF_INT == 16
#ifndef NPY_INT16
#define NPY_INT16 NPY_INT
#define NPY_UINT16 NPY_UINT
        typedef int npy_int16;
        typedef unsigned int npy_uint16;
#    define PyInt16ScalarObject PyIntScalarObject
#    define PyInt16ArrType_Type PyIntArrType_Type
#    define PyUInt16ScalarObject PyIntUScalarObject
#    define PyUInt16ArrType_Type PyIntUArrType_Type
#define NPY_INT16_FMT NPY_INT_FMT
#define NPY_UINT16_FMT NPY_UINT_FMT
#endif
#elif NPY_BITSOF_INT == 32
#ifndef NPY_INT32
#define NPY_INT32 NPY_INT
#define NPY_UINT32 NPY_UINT
        typedef int npy_int32;
        typedef unsigned int npy_uint32;
        typedef unsigned int npy_ucs4;
#    define PyInt32ScalarObject PyIntScalarObject
#    define PyInt32ArrType_Type PyIntArrType_Type
#    define PyUInt32ScalarObject PyUIntScalarObject
#    define PyUInt32ArrType_Type PyUIntArrType_Type
#define NPY_INT32_FMT NPY_INT_FMT
#define NPY_UINT32_FMT NPY_UINT_FMT
#endif
#elif NPY_BITSOF_INT == 64
#ifndef NPY_INT64
#define NPY_INT64 NPY_INT
#define NPY_UINT64 NPY_UINT
        typedef int npy_int64;
        typedef unsigned int npy_uint64;
#    define PyInt64ScalarObject PyIntScalarObject
#    define PyInt64ArrType_Type PyIntArrType_Type
#    define PyUInt64ScalarObject PyUIntScalarObject
#    define PyUInt64ArrType_Type PyUIntArrType_Type
#define NPY_INT64_FMT NPY_INT_FMT
#define NPY_UINT64_FMT NPY_UINT_FMT
#endif
#elif NPY_BITSOF_INT == 128
#ifndef NPY_INT128
#define NPY_INT128 NPY_INT
#define NPY_UINT128 NPY_UINT
        typedef int npy_int128;
        typedef unsigned int npy_uint128;
#    define PyInt128ScalarObject PyIntScalarObject
#    define PyInt128ArrType_Type PyIntArrType_Type
#    define PyUInt128ScalarObject PyUIntScalarObject
#    define PyUInt128ArrType_Type PyUIntArrType_Type
#define NPY_INT128_FMT NPY_INT_FMT
#define NPY_UINT128_FMT NPY_UINT_FMT
#endif
#endif

#if NPY_BITSOF_SHORT == 8
#ifndef NPY_INT8
#define NPY_INT8 NPY_SHORT
#define NPY_UINT8 NPY_USHORT
        typedef short npy_int8;
        typedef unsigned short npy_uint8;
#    define PyInt8ScalarObject PyShortScalarObject
#    define PyInt8ArrType_Type PyShortArrType_Type
#    define PyUInt8ScalarObject PyUShortScalarObject
#    define PyUInt8ArrType_Type PyUShortArrType_Type
#define NPY_INT8_FMT NPY_SHORT_FMT
#define NPY_UINT8_FMT NPY_USHORT_FMT
#endif
#elif NPY_BITSOF_SHORT == 16
#ifndef NPY_INT16
#define NPY_INT16 NPY_SHORT
#define NPY_UINT16 NPY_USHORT
        typedef short npy_int16;
        typedef unsigned short npy_uint16;
#    define PyInt16ScalarObject PyShortScalarObject
#    define PyInt16ArrType_Type PyShortArrType_Type
#    define PyUInt16ScalarObject PyUShortScalarObject
#    define PyUInt16ArrType_Type PyUShortArrType_Type
#define NPY_INT16_FMT NPY_SHORT_FMT
#define NPY_UINT16_FMT NPY_USHORT_FMT
#endif
#elif NPY_BITSOF_SHORT == 32
#ifndef NPY_INT32
#define NPY_INT32 NPY_SHORT
#define NPY_UINT32 NPY_USHORT
        typedef short npy_int32;
        typedef unsigned short npy_uint32;
        typedef unsigned short npy_ucs4;
#    define PyInt32ScalarObject PyShortScalarObject
#    define PyInt32ArrType_Type PyShortArrType_Type
#    define PyUInt32ScalarObject PyUShortScalarObject
#    define PyUInt32ArrType_Type PyUShortArrType_Type
#define NPY_INT32_FMT NPY_SHORT_FMT
#define NPY_UINT32_FMT NPY_USHORT_FMT
#endif
#elif NPY_BITSOF_SHORT == 64
#ifndef NPY_INT64
#define NPY_INT64 NPY_SHORT
#define NPY_UINT64 NPY_USHORT
        typedef short npy_int64;
        typedef unsigned short npy_uint64;
#    define PyInt64ScalarObject PyShortScalarObject
#    define PyInt64ArrType_Type PyShortArrType_Type
#    define PyUInt64ScalarObject PyUShortScalarObject
#    define PyUInt64ArrType_Type PyUShortArrType_Type
#define NPY_INT64_FMT NPY_SHORT_FMT
#define NPY_UINT64_FMT NPY_USHORT_FMT
#endif
#elif NPY_BITSOF_SHORT == 128
#ifndef NPY_INT128
#define NPY_INT128 NPY_SHORT
#define NPY_UINT128 NPY_USHORT
        typedef short npy_int128;
        typedef unsigned short npy_uint128;
#    define PyInt128ScalarObject PyShortScalarObject
#    define PyInt128ArrType_Type PyShortArrType_Type
#    define PyUInt128ScalarObject PyUShortScalarObject
#    define PyUInt128ArrType_Type PyUShortArrType_Type
#define NPY_INT128_FMT NPY_SHORT_FMT
#define NPY_UINT128_FMT NPY_USHORT_FMT
#endif
#endif


#if NPY_BITSOF_CHAR == 8
#ifndef NPY_INT8
#define NPY_INT8 NPY_BYTE
#define NPY_UINT8 NPY_UBYTE
        typedef signed char npy_int8;
        typedef unsigned char npy_uint8;
#    define PyInt8ScalarObject PyByteScalarObject
#    define PyInt8ArrType_Type PyByteArrType_Type
#    define PyUInt8ScalarObject PyUByteScalarObject
#    define PyUInt8ArrType_Type PyUByteArrType_Type
#define NPY_INT8_FMT NPY_BYTE_FMT
#define NPY_UINT8_FMT NPY_UBYTE_FMT
#endif
#elif NPY_BITSOF_CHAR == 16
#ifndef NPY_INT16
#define NPY_INT16 NPY_BYTE
#define NPY_UINT16 NPY_UBYTE
        typedef signed char npy_int16;
        typedef unsigned char npy_uint16;
#    define PyInt16ScalarObject PyByteScalarObject
#    define PyInt16ArrType_Type PyByteArrType_Type
#    define PyUInt16ScalarObject PyUByteScalarObject
#    define PyUInt16ArrType_Type PyUByteArrType_Type
#define NPY_INT16_FMT NPY_BYTE_FMT
#define NPY_UINT16_FMT NPY_UBYTE_FMT
#endif
#elif NPY_BITSOF_CHAR == 32
#ifndef NPY_INT32
#define NPY_INT32 NPY_BYTE
#define NPY_UINT32 NPY_UBYTE
        typedef signed char npy_int32;
        typedef unsigned char npy_uint32;
        typedef unsigned char npy_ucs4;
#    define PyInt32ScalarObject PyByteScalarObject
#    define PyInt32ArrType_Type PyByteArrType_Type
#    define PyUInt32ScalarObject PyUByteScalarObject
#    define PyUInt32ArrType_Type PyUByteArrType_Type
#define NPY_INT32_FMT NPY_BYTE_FMT
#define NPY_UINT32_FMT NPY_UBYTE_FMT
#endif
#elif NPY_BITSOF_CHAR == 64
#ifndef NPY_INT64
#define NPY_INT64 NPY_BYTE
#define NPY_UINT64 NPY_UBYTE
        typedef signed char npy_int64;
        typedef unsigned char npy_uint64;
#    define PyInt64ScalarObject PyByteScalarObject
#    define PyInt64ArrType_Type PyByteArrType_Type
#    define PyUInt64ScalarObject PyUByteScalarObject
#    define PyUInt64ArrType_Type PyUByteArrType_Type
#define NPY_INT64_FMT NPY_BYTE_FMT
#define NPY_UINT64_FMT NPY_UBYTE_FMT
#endif
#elif NPY_BITSOF_CHAR == 128
#ifndef NPY_INT128
#define NPY_INT128 NPY_BYTE
#define NPY_UINT128 NPY_UBYTE
        typedef signed char npy_int128;
        typedef unsigned char npy_uint128;
#    define PyInt128ScalarObject PyByteScalarObject
#    define PyInt128ArrType_Type PyByteArrType_Type
#    define PyUInt128ScalarObject PyUByteScalarObject
#    define PyUInt128ArrType_Type PyUByteArrType_Type
#define NPY_INT128_FMT NPY_BYTE_FMT
#define NPY_UINT128_FMT NPY_UBYTE_FMT
#endif
#endif



#if NPY_BITSOF_DOUBLE == 16
#ifndef NPY_FLOAT16
#define NPY_FLOAT16 NPY_DOUBLE
#define NPY_COMPLEX32 NPY_CDOUBLE
        typedef  double npy_float16;
        typedef npy_cdouble npy_complex32;
#    define PyFloat16ScalarObject PyDoubleScalarObject
#    define PyComplex32ScalarObject PyCDoubleScalarObject
#    define PyFloat16ArrType_Type PyDoubleArrType_Type
#    define PyComplex32ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT16_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX32_FMT NPY_CDOUBLE_FMT
#endif
#elif NPY_BITSOF_DOUBLE == 32
#ifndef NPY_FLOAT32
#define NPY_FLOAT32 NPY_DOUBLE
#define NPY_COMPLEX64 NPY_CDOUBLE
        typedef double npy_float32;
        typedef npy_cdouble npy_complex64;
#    define PyFloat32ScalarObject PyDoubleScalarObject
#    define PyComplex64ScalarObject PyCDoubleScalarObject
#    define PyFloat32ArrType_Type PyDoubleArrType_Type
#    define PyComplex64ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT32_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX64_FMT NPY_CDOUBLE_FMT
#endif
#elif NPY_BITSOF_DOUBLE == 64
#ifndef NPY_FLOAT64
#define NPY_FLOAT64 NPY_DOUBLE
#define NPY_COMPLEX128 NPY_CDOUBLE
        typedef double npy_float64;
        typedef npy_cdouble npy_complex128;
#    define PyFloat64ScalarObject PyDoubleScalarObject
#    define PyComplex128ScalarObject PyCDoubleScalarObject
#    define PyFloat64ArrType_Type PyDoubleArrType_Type
#    define PyComplex128ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT64_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX128_FMT NPY_CDOUBLE_FMT
#endif
#elif NPY_BITSOF_DOUBLE == 80
#ifndef NPY_FLOAT80
#define NPY_FLOAT80 NPY_DOUBLE
#define NPY_COMPLEX160 NPY_CDOUBLE
        typedef double npy_float80;
        typedef npy_cdouble npy_complex160;
#    define PyFloat80ScalarObject PyDoubleScalarObject
#    define PyComplex160ScalarObject PyCDoubleScalarObject
#    define PyFloat80ArrType_Type PyDoubleArrType_Type
#    define PyComplex160ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT80_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX160_FMT NPY_CDOUBLE_FMT
#endif
#elif NPY_BITSOF_DOUBLE == 96
#ifndef NPY_FLOAT96
#define NPY_FLOAT96 NPY_DOUBLE
#define NPY_COMPLEX192 NPY_CDOUBLE
        typedef double npy_float96;
        typedef npy_cdouble npy_complex192;
#    define PyFloat96ScalarObject PyDoubleScalarObject
#    define PyComplex192ScalarObject PyCDoubleScalarObject
#    define PyFloat96ArrType_Type PyDoubleArrType_Type
#    define PyComplex192ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT96_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX192_FMT NPY_CDOUBLE_FMT
#endif
#elif NPY_BITSOF_DOUBLE == 128
#ifndef NPY_FLOAT128
#define NPY_FLOAT128 NPY_DOUBLE
#define NPY_COMPLEX256 NPY_CDOUBLE
        typedef double npy_float128;
        typedef npy_cdouble npy_complex256;
#    define PyFloat128ScalarObject PyDoubleScalarObject
#    define PyComplex256ScalarObject PyCDoubleScalarObject
#    define PyFloat128ArrType_Type PyDoubleArrType_Type
#    define PyComplex256ArrType_Type PyCDoubleArrType_Type
#define NPY_FLOAT128_FMT NPY_DOUBLE_FMT
#define NPY_COMPLEX256_FMT NPY_CDOUBLE_FMT
#endif
#endif



#if NPY_BITSOF_FLOAT == 16
#ifndef NPY_FLOAT16
#define NPY_FLOAT16 NPY_FLOAT
#define NPY_COMPLEX32 NPY_CFLOAT
        typedef float npy_float16;
        typedef npy_cfloat npy_complex32;
#    define PyFloat16ScalarObject PyFloatScalarObject
#    define PyComplex32ScalarObject PyCFloatScalarObject
#    define PyFloat16ArrType_Type PyFloatArrType_Type
#    define PyComplex32ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT16_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX32_FMT NPY_CFLOAT_FMT
#endif
#elif NPY_BITSOF_FLOAT == 32
#ifndef NPY_FLOAT32
#define NPY_FLOAT32 NPY_FLOAT
#define NPY_COMPLEX64 NPY_CFLOAT
        typedef float npy_float32;
        typedef npy_cfloat npy_complex64;
#    define PyFloat32ScalarObject PyFloatScalarObject
#    define PyComplex64ScalarObject PyCFloatScalarObject
#    define PyFloat32ArrType_Type PyFloatArrType_Type
#    define PyComplex64ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT32_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX64_FMT NPY_CFLOAT_FMT
#endif
#elif NPY_BITSOF_FLOAT == 64
#ifndef NPY_FLOAT64
#define NPY_FLOAT64 NPY_FLOAT
#define NPY_COMPLEX128 NPY_CFLOAT
        typedef float npy_float64;
        typedef npy_cfloat npy_complex128;
#    define PyFloat64ScalarObject PyFloatScalarObject
#    define PyComplex128ScalarObject PyCFloatScalarObject
#    define PyFloat64ArrType_Type PyFloatArrType_Type
#    define PyComplex128ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT64_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX128_FMT NPY_CFLOAT_FMT
#endif
#elif NPY_BITSOF_FLOAT == 80
#ifndef NPY_FLOAT80
#define NPY_FLOAT80 NPY_FLOAT
#define NPY_COMPLEX160 NPY_CFLOAT
        typedef float npy_float80;
        typedef npy_cfloat npy_complex160;
#    define PyFloat80ScalarObject PyFloatScalarObject
#    define PyComplex160ScalarObject PyCFloatScalarObject
#    define PyFloat80ArrType_Type PyFloatArrType_Type
#    define PyComplex160ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT80_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX160_FMT NPY_CFLOAT_FMT
#endif
#elif NPY_BITSOF_FLOAT == 96
#ifndef NPY_FLOAT96
#define NPY_FLOAT96 NPY_FLOAT
#define NPY_COMPLEX192 NPY_CFLOAT
        typedef float npy_float96;
        typedef npy_cfloat npy_complex192;
#    define PyFloat96ScalarObject PyFloatScalarObject
#    define PyComplex192ScalarObject PyCFloatScalarObject
#    define PyFloat96ArrType_Type PyFloatArrType_Type
#    define PyComplex192ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT96_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX192_FMT NPY_CFLOAT_FMT
#endif
#elif NPY_BITSOF_FLOAT == 128
#ifndef NPY_FLOAT128
#define NPY_FLOAT128 NPY_FLOAT
#define NPY_COMPLEX256 NPY_CFLOAT
        typedef float npy_float128;
        typedef npy_cfloat npy_complex256;
#    define PyFloat128ScalarObject PyFloatScalarObject
#    define PyComplex256ScalarObject PyCFloatScalarObject
#    define PyFloat128ArrType_Type PyFloatArrType_Type
#    define PyComplex256ArrType_Type PyCFloatArrType_Type
#define NPY_FLOAT128_FMT NPY_FLOAT_FMT
#define NPY_COMPLEX256_FMT NPY_CFLOAT_FMT
#endif
#endif


#if NPY_BITSOF_LONGDOUBLE == 16
#ifndef NPY_FLOAT16
#define NPY_FLOAT16 NPY_LONGDOUBLE
#define NPY_COMPLEX32 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float16;
        typedef npy_clongdouble npy_complex32;
#    define PyFloat16ScalarObject PyLongDoubleScalarObject
#    define PyComplex32ScalarObject PyCLongDoubleScalarObject
#    define PyFloat16ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex32ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT16_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX32_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 32
#ifndef NPY_FLOAT32
#define NPY_FLOAT32 NPY_LONGDOUBLE
#define NPY_COMPLEX64 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float32;
        typedef npy_clongdouble npy_complex64;
#    define PyFloat32ScalarObject PyLongDoubleScalarObject
#    define PyComplex64ScalarObject PyCLongDoubleScalarObject
#    define PyFloat32ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex64ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT32_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX64_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 64
#ifndef NPY_FLOAT64
#define NPY_FLOAT64 NPY_LONGDOUBLE
#define NPY_COMPLEX128 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float64;
        typedef npy_clongdouble npy_complex128;
#    define PyFloat64ScalarObject PyLongDoubleScalarObject
#    define PyComplex128ScalarObject PyCLongDoubleScalarObject
#    define PyFloat64ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex128ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT64_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX128_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 80
#ifndef NPY_FLOAT80
#define NPY_FLOAT80 NPY_LONGDOUBLE
#define NPY_COMPLEX160 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float80;
        typedef npy_clongdouble npy_complex160;
#    define PyFloat80ScalarObject PyLongDoubleScalarObject
#    define PyComplex160ScalarObject PyCLongDoubleScalarObject
#    define PyFloat80ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex160ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT80_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX160_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 96
#ifndef NPY_FLOAT96
#define NPY_FLOAT96 NPY_LONGDOUBLE
#define NPY_COMPLEX192 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float96;
        typedef npy_clongdouble npy_complex192;
#    define PyFloat96ScalarObject PyLongDoubleScalarObject
#    define PyComplex192ScalarObject PyCLongDoubleScalarObject
#    define PyFloat96ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex192ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT96_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX192_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 128
#ifndef NPY_FLOAT128
#define NPY_FLOAT128 NPY_LONGDOUBLE
#define NPY_COMPLEX256 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float128;
        typedef npy_clongdouble npy_complex256;
#    define PyFloat128ScalarObject PyLongDoubleScalarObject
#    define PyComplex256ScalarObject PyCLongDoubleScalarObject
#    define PyFloat128ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex256ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT128_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX256_FMT NPY_CLONGDOUBLE_FMT
#endif
#elif NPY_BITSOF_LONGDOUBLE == 256
#define NPY_FLOAT256 NPY_LONGDOUBLE
#define NPY_COMPLEX512 NPY_CLONGDOUBLE
        typedef npy_longdouble npy_float256;
        typedef npy_clongdouble npy_complex512;
#    define PyFloat256ScalarObject PyLongDoubleScalarObject
#    define PyComplex512ScalarObject PyCLongDoubleScalarObject
#    define PyFloat256ArrType_Type PyLongDoubleArrType_Type
#    define PyComplex512ArrType_Type PyCLongDoubleArrType_Type
#define NPY_FLOAT256_FMT NPY_LONGDOUBLE_FMT
#define NPY_COMPLEX512_FMT NPY_CLONGDOUBLE_FMT
#endif

/* End of typedefs for numarray style bit-width names */

#endif

