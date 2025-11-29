//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_COM_H__
#define __SP_COM_H__

#include <math.h>


//--------------------------------------------------------------------------------
// system
//--------------------------------------------------------------------------------

#ifndef SP_USE_OMP
#define SP_USE_OMP 1
#endif

#ifndef SP_USE_CONSOLE
#define SP_USE_CONSOLE 1
#endif

#ifndef SP_USE_DEBUG
#define SP_USE_DEBUG 0
#endif


//--------------------------------------------------------------------------------
// assert
//--------------------------------------------------------------------------------

#ifndef SP_USE_ASSERT
#define SP_USE_ASSERT 1
#endif

#if SP_USE_ASSERT
#include<assert.h>
#define SP_ASSERT(EXP) if (!(EXP)) { assert((EXP)); }
#elif
#define SP_ASSERT(EXP) (EXP)
#endif


//--------------------------------------------------------------------------------
// calling convention
//--------------------------------------------------------------------------------

// cpu function
#ifndef SP_CPUFUNC
#define SP_CPUFUNC static
#endif

// cpu/gpu function
#ifndef SP_GENFUNC
#define SP_GENFUNC static
#endif


//--------------------------------------------------------------------------------
// real number type
//--------------------------------------------------------------------------------

#ifndef SP_REAL
#define SP_REAL double
#endif


//--------------------------------------------------------------------------------
// const value
//--------------------------------------------------------------------------------

// pi
#define SP_PI static_cast<SP_REAL>(3.14159265358979323846)

// limit value regarded as zero
#define SP_SMALL    static_cast<SP_REAL>(1.0e-20)

// limit value regarded as infinity
#define SP_INFINITY static_cast<SP_REAL>(1.0e+20)

// maximal value (unsigned char)
#define SP_BYTEMAX 255

// maximal value (int)
#define SP_INTMAX ((int)(((unsigned)-1)/2))

// maximal string
#define SP_STRMAX 512

// maximal dimension
#define SP_DIMMAX 3


//--------------------------------------------------------------------------------
// struct
//--------------------------------------------------------------------------------

namespace sp {

    //--------------------------------------------------------------------------------
    // type
    //--------------------------------------------------------------------------------

    typedef char           s08;
    typedef unsigned char  u08;
    typedef short          s16;
    typedef unsigned short u16;
    typedef int            s32;
    typedef unsigned int   u32;
    typedef float          f32;
    typedef double         f64;

    //--------------------------------------------------------------------------------
    // basic
    //--------------------------------------------------------------------------------

    // get round (ex. 1.5 -> 2)
    SP_GENFUNC int round(const double x) { return static_cast<int>((x > 0) ? (x + 0.5) : (x - 0.5)); }

    // get ceil (ex. 1.5 -> 2)
    SP_GENFUNC int ceil(const double x) { return static_cast<int>((x > 0) ? (x + 1.0) : (x - 1.0)); }

    // get floor (ex. 1.5 -> 1)
    SP_GENFUNC int floor(const double x) { return static_cast<int>(x); }

    // get sign (+1 or -1)
    template<typename TYPE> SP_GENFUNC int sign(const TYPE x) { return (x > 0) - (x < 0); }

    // swap
    template<typename TYPE> SP_GENFUNC void swap(TYPE& a, TYPE& b) { const TYPE tmp = a; a = b; b = tmp; }

    // get max value
    SP_GENFUNC const SP_REAL max(const double a, const double b) { return static_cast<SP_REAL>((a > b) ? a : b); }

    // get min value
    SP_GENFUNC const SP_REAL min(const double a, const double b) { return static_cast<SP_REAL>((a < b) ? a : b); }

    // get clampped value
    SP_GENFUNC const int clamp(const int v, const int minv, const int maxv) { return (v > maxv) ? maxv : ((v < minv) ? minv : v); }

    // get clampped value
    SP_GENFUNC const SP_REAL clamp(const double v, const double minv, const double maxv) { return static_cast<SP_REAL>((v > maxv) ? maxv : ((v < minv) ? minv : v)); }

}

#endif