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

    //--------------------------------------------------------------------------------
    // memory ptr
    //--------------------------------------------------------------------------------

    template<typename TYPE> struct ExPtr {
        // pointer
        TYPE* ptr;

        // dimension
        int dim;

        // dimension size
        int dsize[SP_DIMMAX];
    };


    //--------------------------------------------------------------------------------
    // vector
    //--------------------------------------------------------------------------------


    class Vec2 {
    public:
        SP_REAL x, y;
        Vec2() {
            x = static_cast<SP_REAL>(0.0);
            y = static_cast<SP_REAL>(0.0);
        }

        Vec2(const double x, const double y) {
            this->x = static_cast<SP_REAL>(x);
            this->y = static_cast<SP_REAL>(y);
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend Vec2 operator + (const Vec2& vec0, const Vec2& vec1) {
            return Vec2(vec0.x + vec1.x, vec0.y + vec1.y);
        }

        void operator += (const Vec2& vec) {
            x += vec.x;
            y += vec.y;
        }

        Vec2 operator + () const {
            return *this;
        }

        friend Vec2 operator - (const Vec2& vec0, const Vec2& vec1) {
            return Vec2(vec0.x - vec1.x, vec0.y - vec1.y);
        }

        void operator -= (const Vec2& vec) {
            x -= vec.x;
            y -= vec.y;
        }

        Vec2 operator - () const {
            return Vec2(-x, -y);
        }

        friend Vec2 operator * (const Vec2& vec, const double val) {
            return Vec2(vec.x * val, vec.y * val);
        }

        friend Vec2 operator * (const double val, const Vec2& vec) {
            return Vec2(vec.x * val, vec.y * val);
        }

        void operator *= (const double val) {
            x *= val;
            y *= val;
        }

        friend Vec2 operator / (const Vec2& vec, const double val) {
            SP_ASSERT(val < -SP_SMALL || val >+SP_SMALL);
            return Vec2(vec.x / val, vec.y / val);
        }

        void operator /= (const double val) {
            SP_ASSERT(val < -SP_SMALL || val >+SP_SMALL);
            x /= val;
            y /= val;
        }

        //friend bool operator == (const Vec2& v0, const Vec2& v1) {
        //    return cmp(v0, v1) == true;
        //}

        //friend bool operator != (const Vec2& v0, const Vec2& v1) {
        //    return cmp(v0, v1) == false;
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // dot production
        SP_REAL dot(const Vec2& vec) const {
            return x * vec.x + y * vec.y;
        }

        SP_REAL cross(const Vec2& vec) const {
            return x * vec.y - y * vec.x;
        }

        // length
        SP_REAL length() const {
            return sqrt(x * x + y * y);
        }

        Vec2 unit() const {
            const double l = length();
            return (l > SP_SMALL) ? *this / l : Vec2(0.0, 0.0);
        }

        // round
        friend Vec2 round(const Vec2 vec) {
            return Vec2(round(vec.x), round(vec.y));
        }

        static bool cmp(const Vec2& v0, const Vec2& v1, const double t = 1.0e-6) {
            return ::fabs(v0.x - v1.x) < t && ::fabs(v0.y - v1.y) < t;
        }

        static Vec2 mul(const SP_REAL* mat, const int rows, const int cols, const Vec2& vec) {
            Vec2 dst = Vec2(0.0, 0.0);
            if (rows == 2 && cols == 2) {
                dst.x = static_cast<SP_REAL>(mat[0 * 2 + 0] * vec.x + mat[0 * 2 + 1] * vec.y);
                dst.y = static_cast<SP_REAL>(mat[1 * 2 + 0] * vec.x + mat[1 * 2 + 1] * vec.y);
            }
            if (rows == 2 && cols == 3) {
                dst.x = static_cast<SP_REAL>(mat[0 * 3 + 0] * vec.x + mat[0 * 3 + 1] * vec.y + mat[0 * 3 + 2]);
                dst.y = static_cast<SP_REAL>(mat[1 * 3 + 0] * vec.x + mat[1 * 3 + 1] * vec.y + mat[1 * 3 + 2]);
            }
            if (rows == 3 && cols == 3) {
                const double scale = mat[2 * 3 + 0] * vec.x + mat[2 * 3 + 1] * vec.y + mat[2 * 3 + 2];
                dst.x = static_cast<SP_REAL>((mat[0 * 3 + 0] * vec.x + mat[0 * 3 + 1] * vec.y + mat[0 * 3 + 2]) / scale);
                dst.y = static_cast<SP_REAL>((mat[1 * 3 + 0] * vec.x + mat[1 * 3 + 1] * vec.y + mat[1 * 3 + 2]) / scale);
            }
            if (rows == 3 && cols == 4) {
                dst.x = static_cast<SP_REAL>(mat[0 * 4 + 0] * vec.x + mat[0 * 4 + 1] * vec.y + mat[0 * 4 + 2] * 0.0 + mat[0 * 4 + 3]);
                dst.y = static_cast<SP_REAL>(mat[1 * 4 + 0] * vec.x + mat[1 * 4 + 1] * vec.y + mat[1 * 4 + 2] * 0.0 + mat[1 * 4 + 3]);
            }
            if (rows == 4 && cols == 4) {
                const double scale = mat[3 * 4 + 0] * vec.x + mat[3 * 4 + 1] * vec.y + mat[3 * 4 + 2] * 0.0 + mat[3 * 4 + 3];
                dst.x = static_cast<SP_REAL>((mat[0 * 4 + 0] * vec.x + mat[0 * 4 + 1] * vec.y + mat[0 * 4 + 2] * 0.0 + mat[0 * 4 + 3]) / scale);
                dst.y = static_cast<SP_REAL>((mat[1 * 4 + 0] * vec.x + mat[1 * 4 + 1] * vec.y + mat[1 * 4 + 2] * 0.0 + mat[1 * 4 + 3]) / scale);
            }
            return dst;
        }
    };

    class Vec3 {
    public:
        SP_REAL x, y, z;
        Vec3() {
            x = static_cast<SP_REAL>(0.0);
            y = static_cast<SP_REAL>(0.0);
            z = static_cast<SP_REAL>(0.0);
        }

        Vec3(const double x, const double y, const double z) {
            this->x = static_cast<SP_REAL>(x);
            this->y = static_cast<SP_REAL>(y);
            this->z = static_cast<SP_REAL>(z);
        }

        Vec3(const Vec2& vec, const double z) {
            this->x = static_cast<SP_REAL>(vec.x);
            this->y = static_cast<SP_REAL>(vec.y);
            this->z = static_cast<SP_REAL>(z);
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend Vec3 operator + (const Vec3& vec0, const Vec3& vec1) {
            return Vec3(vec0.x + vec1.x, vec0.y + vec1.y, vec0.z + vec1.z);
        }

        void operator += (const Vec3& vec) {
            x += vec.x;
            y += vec.y;
            z += vec.z;
        }

        Vec3 operator + () const {
            return *this;
        }

        friend Vec3 operator - (const Vec3& vec0, const Vec3& vec1) {
            return Vec3(vec0.x - vec1.x, vec0.y - vec1.y, vec0.z - vec1.z);
        }

        void operator -= (const Vec3& vec) {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
        }

        Vec3 operator - () const {
            return Vec3(-x, -y, -z);
        }

        friend Vec3 operator * (const Vec3& vec, const double val) {
            return Vec3(vec.x * val, vec.y * val, vec.z * val);
        }

        friend Vec3 operator * (const double val, const Vec3& vec) {
            return Vec3(vec.x * val, vec.y * val, vec.z * val);
        }

        void operator *= (const double val) {
            x *= val;
            y *= val;
            z *= val;
        }

        friend Vec3 operator / (const Vec3& vec, const double val) {
            SP_ASSERT(val < -SP_SMALL || val >+SP_SMALL);
            return Vec3(vec.x / val, vec.y / val, vec.z / val);
        }

        void operator /= (const double val) {
            SP_ASSERT(val < -SP_SMALL || val >+SP_SMALL);
            x /= val;
            y /= val;
            z /= val;
        }

        //friend bool operator == (const Vec3& v0, const Vec3& v1) {
        //    return cmp(v0, v1) == true;
        //}

        //friend bool operator != (const Vec3& v0, const Vec3& v1) {
        //    return cmp(v0, v1) == false;
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // dot production
        SP_REAL dot(const Vec3& vec) const {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        Vec3 cross(const Vec3& vec) const {
            return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
        }

        // length
        SP_REAL length() const {
            return sqrt(x * x + y * y + z * z);
        }

        Vec3 unit() const {
            const double l = length();
            return (l > SP_SMALL) ? *this / l : Vec3(0.0, 0.0, 0.0);
        }

        // round
        friend Vec3 round(const Vec3 vec) {
            return Vec3(round(vec.x), round(vec.y), round(vec.z));
        }

        static bool cmp(const Vec3& v0, const Vec3& v1, const double t = 1.0e-6) {
            return ::fabs(v0.x - v1.x) < t && ::fabs(v0.y - v1.y) < t && ::fabs(v0.z - v1.z) < t;
        }

        static Vec3 mul(const SP_REAL* mat, const int rows, const int cols, const Vec3& vec) {
            Vec3 dst = Vec3(0.0, 0.0, 0.0);
            if (rows == 3 && cols == 3) {
                dst.x = static_cast<SP_REAL>(mat[0 * 3 + 0] * vec.x + mat[0 * 3 + 1] * vec.y + mat[0 * 3 + 2] * vec.z);
                dst.y = static_cast<SP_REAL>(mat[1 * 3 + 0] * vec.x + mat[1 * 3 + 1] * vec.y + mat[1 * 3 + 2] * vec.z);
                dst.z = static_cast<SP_REAL>(mat[2 * 3 + 0] * vec.x + mat[2 * 3 + 1] * vec.y + mat[2 * 3 + 2] * vec.z);
            }
            if (rows == 3 && cols == 4) {
                dst.x = static_cast<SP_REAL>(mat[0 * 4 + 0] * vec.x + mat[0 * 4 + 1] * vec.y + mat[0 * 4 + 2] * vec.z + mat[0 * 4 + 3]);
                dst.y = static_cast<SP_REAL>(mat[1 * 4 + 0] * vec.x + mat[1 * 4 + 1] * vec.y + mat[1 * 4 + 2] * vec.z + mat[1 * 4 + 3]);
                dst.z = static_cast<SP_REAL>(mat[2 * 4 + 0] * vec.x + mat[2 * 4 + 1] * vec.y + mat[2 * 4 + 2] * vec.z + mat[2 * 4 + 3]);
            }
            if (rows == 4 && cols == 4) {
                const double scale = mat[3 * 4 + 0] * vec.x + mat[3 * 4 + 1] * vec.y + mat[3 * 4 + 2] * vec.z + mat[3 * 4 + 3];
                dst.x = static_cast<SP_REAL>((mat[0 * 4 + 0] * vec.x + mat[0 * 4 + 1] * vec.y + mat[0 * 4 + 2] * vec.z + mat[0 * 4 + 3]) / scale);
                dst.y = static_cast<SP_REAL>((mat[1 * 4 + 0] * vec.x + mat[1 * 4 + 1] * vec.y + mat[1 * 4 + 2] * vec.z + mat[1 * 4 + 3]) / scale);
                dst.z = static_cast<SP_REAL>((mat[2 * 4 + 0] * vec.x + mat[2 * 4 + 1] * vec.y + mat[2 * 4 + 2] * vec.z + mat[2 * 4 + 3]) / scale);
            }
            return dst;
        }
    };

    //--------------------------------------------------------------------------------
    // position and direction
    //--------------------------------------------------------------------------------

    class VecPD2 {
    public:
        Vec2 pos, drc;

        VecPD2() {
        }

        VecPD2(const Vec2& pos, const Vec2& drc) {
            this->pos = pos;
            this->drc = drc;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------
        //friend bool operator == (const VecPD2& vec0, const VecPD2& vec1) {
        //    return VecPD2::cmp(vec0, vec1) == true;
        //}

        //friend bool operator != (const VecPD2& vec0, const VecPD2& vec1) {
        //    return VecPD2::cmp(vec0, vec1) == false;
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // compare vec (position and normal)
        static bool cmp(const VecPD2& vec0, const VecPD2& vec1, const double t = 1.0e-6) {
            return Vec2::cmp(vec0.pos, vec1.pos, t) && Vec2::cmp(vec0.drc, vec1.drc, t);
        }
    };

    class VecPD3 {
    public:
        Vec3 pos, drc;

        VecPD3() {
        }

        VecPD3(const Vec3& pos, const Vec3& drc) {
            this->pos = pos;
            this->drc = drc;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------
        //friend bool operator == (const VecPD3& vec0, const VecPD3& vec1) {
        //    return VecPD3::cmp(vec0, vec1) == true;
        //}

        //friend bool operator != (const VecPD3& vec0, const VecPD3& vec1) {
        //    return VecPD3::cmp(vec0, vec1) == false;
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // compare vec (position and normal)
        static bool cmp(const VecPD3& vec0, const VecPD3& vec1, const double t = 1.0e-6) {
            return Vec3::cmp(vec0.pos, vec1.pos, t) && Vec3::cmp(vec0.drc, vec1.drc, t);
        }
    };

    //--------------------------------------------------------------------------------
    // line
    //--------------------------------------------------------------------------------

    class Line2 {
    public:
        Vec2 pos[2];
        Line2() {
        }
        Line2(const Vec2& vec0, const Vec2& vec1) {
            pos[0] = vec0;
            pos[1] = vec1;
        }

        friend Line2 operator + (const Line2& line, const Vec2& vec) {
            return Line2(line.pos[0] + vec, line.pos[1] + vec);
        }
        friend Line2 operator - (const Line2& line, const Vec2& vec) {
            return Line2(line.pos[0] - vec, line.pos[1] - vec);
        }
        void operator += (const Vec2& vec) {
            pos[0] += vec;
            pos[1] += vec;
        }
        void operator -= (const Vec2& vec) {
            pos[0] -= vec;
            pos[1] -= vec;
        }

        // length
        SP_REAL length() const {
            return (pos[0] - pos[1]).length();
        }

        // length
        SP_REAL distance(const Vec2& vec) const {
            double ret = 0.0;

            const Vec2 lvec = pos[1] - pos[0];
            const double len = lvec.length();

            if (len < SP_SMALL) {
                ret = (vec - (pos[0] + pos[1]) * 0.5).length();
            }
            else {
                const double s = lvec.dot(vec - pos[0]) / len;
                if (s < 0.0) {
                    ret = (vec - pos[0]).length();
                }
                else if (s > len) {
                    ret = (vec - pos[1]).length();
                }
                else {
                    ret = (vec - (pos[0] + lvec * s / len)).length();
                }
            }
            return ret;
        }
    };

    class Line3 {
    public:
        Vec3 pos[2];

        Line3() {
        }
        Line3(const Vec3& vec0, const Vec3& vec1) {
            pos[0] = vec0;
            pos[1] = vec1;
        }

        friend Line3 operator + (const Line3& line, const Vec3& vec) {
            return Line3(line.pos[0] + vec, line.pos[1] + vec);
        }
        friend Line3 operator - (const Line3& line, const Vec3& vec) {
            return Line3(line.pos[0] - vec, line.pos[1] - vec);
        }
        void operator += (const Vec3& vec) {
            pos[0] += vec;
            pos[1] += vec;
        }
        void operator -= (const Vec3& vec) {
            pos[0] -= vec;
            pos[1] -= vec;
        }

        // length
        SP_REAL length() const {
            return (pos[0] - pos[1]).length();
        }

        // length
        SP_REAL distance(const Vec3& vec) const {
            double ret = 0.0;

            const Vec3 lvec = pos[1] - pos[0];
            const double len = lvec.length();

            if (len < SP_SMALL) {
                ret = (vec - (pos[0] + pos[1]) * 0.5).length();
            }
            else {
                const double s = lvec.dot(vec - pos[0]) / len;
                if (s < 0.0) {
                    ret = (vec - pos[0]).length();
                }
                else if (s > len) {
                    ret = (vec - pos[1]).length();
                }
                else {
                    ret = (vec - (pos[0] + lvec * s / len)).length();
                }
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------------
    // triangle mesh
    //--------------------------------------------------------------------------------

    class Mesh2 {
    public:
        Vec2 pos[3];

        Mesh2() {
        }

        Mesh2(const Vec2& vec0, const Vec2& vec1, const Vec2& vec2) {
            pos[0] = vec0;
            pos[1] = vec1;
            pos[2] = vec2;
        }

        friend Mesh2 operator + (const Mesh2& mesh, const Vec2 vec) {
            return Mesh2(mesh.pos[0] + vec, mesh.pos[1] + vec, mesh.pos[2] + vec);
        }
        friend Mesh2 operator - (const Mesh2& mesh, const Vec2 vec) {
            return Mesh2(mesh.pos[0] - vec, mesh.pos[1] - vec, mesh.pos[2] - vec);
        }
        friend Mesh2 operator * (const Mesh2& mesh, const double val) {
            return Mesh2(mesh.pos[0] * val, mesh.pos[1] * val, mesh.pos[2] * val);
        }

        void operator += (const Vec2& vec) {
            pos[0] += vec;
            pos[1] += vec;
            pos[2] += vec;
        }
        void operator -= (const Vec2& vec) {
            pos[0] -= vec;
            pos[1] -= vec;
            pos[2] -= vec;
        }
        void operator *= (const double val) {
            pos[0] *= val;
            pos[1] *= val;
            pos[2] *= val;
        }

        // get center vector
        Vec2 center() const {
            return (pos[0] + pos[1] + pos[2]) / 3.0;
        }
    };


    class Mesh3 {
    public:
        Vec3 pos[3];

        Mesh3() {
        }

        Mesh3(const Vec3& vec0, const Vec3& vec1, const Vec3& vec2) {
            pos[0] = vec0;
            pos[1] = vec1;
            pos[2] = vec2;
        }

        friend Mesh3 operator + (const Mesh3& mesh, const Vec3 vec) {
            return Mesh3(mesh.pos[0] + vec, mesh.pos[1] + vec, mesh.pos[2] + vec);
        }
        friend Mesh3 operator - (const Mesh3& mesh, const Vec3 vec) {
            return Mesh3(mesh.pos[0] - vec, mesh.pos[1] - vec, mesh.pos[2] - vec);
        }
        friend Mesh3 operator * (const Mesh3& mesh, const double val) {
            return Mesh3(mesh.pos[0] * val, mesh.pos[1] * val, mesh.pos[2] * val);
        }

        void operator += (const Vec3& vec) {
            pos[0] += vec;
            pos[1] += vec;
            pos[2] += vec;
        }

        void operator -= (const Vec3& vec) {
            pos[0] -= vec;
            pos[1] -= vec;
            pos[2] -= vec;
        }

        void operator *= (const double val) {
            pos[0] *= val;
            pos[1] *= val;
            pos[2] *= val;
        }

        // get normal vector
        Vec3 normal() const {
            return ((pos[1] - pos[0]).cross(pos[2] - pos[0])).unit();
        }

        // get center vector
        Vec3 center() const {
            return (pos[0] + pos[1] + pos[2]) / 3.0;
        }
    };



    //--------------------------------------------------------------------------------
    // rectangle
    //--------------------------------------------------------------------------------

    class Rect2 {
    public:
        // dimension base
        int dbase[2];

        // dimension size
        int dsize[2];

        Rect2(const int dbase0, const int dbase1, const int dsize0, const int dsize1) {
            this->dbase[0] = dbase0;
            this->dbase[1] = dbase1;
            this->dsize[0] = dsize0;
            this->dsize[1] = dsize1;
        }

        Rect2() : Rect2(0, 0, 0, 0) {
        }

        Rect2(const int* dbase, const int* dsize) : Rect2(dbase[0], dbase[1], dsize[0], dsize[1]) {
        }

        Rect2(const int* dsize) : Rect2(0, 0, dsize[0], dsize[1]) {
        }

        Rect2(const Vec2& vec) : Rect2(round(vec.x), round(vec.y), 1, 1) {
        }


        //--------------------------------------------------------------------------------
        // check in rect
        //--------------------------------------------------------------------------------

        template<typename TYPE>
        bool contains(const TYPE* d) const {
            for (int i = 0; i < 2; i++) {
                if (d[i] < static_cast<TYPE>(this->dbase[i])) return false;
                if (d[i] > static_cast<TYPE>(this->dbase[i] + this->dsize[i] - 1)) return false;
            }
            return true;
        }

        bool contains(const Rect2& rect) const {
            for (int i = 0; i < 2; i++) {
                if (rect.dbase[i] < this->dbase[i]) return false;
                if (rect.dbase[i] + rect.dsize[i] > this->dbase[i] + this->dsize[i]) return false;
            }
            return true;
        }


        bool contains(const double d0, const double d1) const {
            const double d[] = { d0, d1 };
            return contains(d);
        }


        bool contains(const Vec2& vec) const {
            const double d[] = { vec.x, vec.y };
            return contains(d);
        }


        //--------------------------------------------------------------------------------
        // rect util
        //--------------------------------------------------------------------------------

        friend Rect2 operator & (const Rect2& rect0, const Rect2& rect1) {
            int dbase[2] = { 0 }, dsize[2] = { 0 };
            for (int i = 0; i < 2; i++) {
                dbase[i] = max(rect0.dbase[i], rect1.dbase[i]);
                dsize[i] = max(0, min(rect0.dbase[i] + rect0.dsize[i], rect1.dbase[i] + rect1.dsize[i]) - dbase[i]);
            }
            return Rect2(dbase, dsize);
        }


        friend Rect2 operator | (const Rect2& rect0, const Rect2& rect1) {
            const bool b0 = (rect0.dsize[0] * rect0.dsize[1] > 0);
            const bool b1 = (rect1.dsize[0] * rect1.dsize[1] > 0);
            if (b0 && b1) {
                int dbase[2] = { 0 }, dsize[2] = { 0 };
                for (int i = 0; i < 2; i++) {
                    dbase[i] = min(rect0.dbase[i], rect1.dbase[i]);
                    dsize[i] = max(rect0.dbase[i] + rect0.dsize[i], rect1.dbase[i] + rect1.dsize[i]) - dbase[i];
                    dsize[i] = max(0, dsize[i]);
                }
                return Rect2(dbase, dsize);
            }
            else {
                return (b0) ? rect0 : rect1;
            }
        }


        friend Rect2 operator + (const Rect2& rect, const int val) { return rect.extend(+val); }
        friend void operator += (Rect2& rect, const int val) { rect = rect.extend(+val); }
        friend Rect2 operator - (const Rect2& rect, const int val) { return rect.extend(-val); }
        friend void operator -= (Rect2& rect, const int val) { rect = rect.extend(-val); }

        Rect2 extend(const int val) const {
            int dbase[2] = { 0 }, dsize[2] = { 0 };
            for (int i = 0; i < 2; i++) {
                const int t = max(val, -this->dsize[i] / 2);
                dbase[i] = this->dbase[i] - t;
                dsize[i] = this->dsize[i] + 2 * t;
            }
            return Rect2(dbase, dsize);
        }

        // get center vector
        Vec2 center() const {
            Vec2 vec;
            vec.x = this->dbase[0] + (this->dsize[0] - 1) / 2.0;
            vec.y = this->dbase[1] + (this->dsize[1] - 1) / 2.0;
            return vec;
        }
    };

    class Rect3 {
    public:
        // dimension base
        int dbase[3];

        // dimension size
        int dsize[3];

        Rect3(const int dbase0, const int dbase1, const int dbase2, const int dsize0, const int dsize1, const int dsize2) {
            this->dbase[0] = dbase0;
            this->dbase[1] = dbase1;
            this->dbase[2] = dbase2;
            this->dsize[0] = dsize0;
            this->dsize[1] = dsize1;
            this->dsize[2] = dsize2;
        }
        Rect3() : Rect3(0, 0, 0, 0, 0, 0) {
        }
        Rect3(const int* dbase, const int* dsize) : Rect3(dbase[0], dbase[1], dbase[2], dsize[0], dsize[1], dsize[2]) {
        }

        Rect3(const int* dsize) : Rect3(0, 0, 0, dsize[0], dsize[1], dsize[2]) {
        }

        Rect3(const Vec3& vec) : Rect3(round(vec.x), round(vec.y), round(vec.z), 1, 1, 1) {
        }


        //--------------------------------------------------------------------------------
        // check in rect
        //--------------------------------------------------------------------------------

        template<typename TYPE>
        bool contains(const TYPE* d) const {
            for (int i = 0; i < 3; i++) {
                if (d[i] < static_cast<TYPE>(this->dbase[i])) return false;
                if (d[i] > static_cast<TYPE>(this->dbase[i] + this->dsize[i] - 1)) return false;
            }
            return true;
        }

        bool contains(const Rect3& rect) const {
            for (int i = 0; i < 3; i++) {
                if (rect.dbase[i] < this->dbase[i]) return false;
                if (rect.dbase[i] + rect.dsize[i] > this->dbase[i] + this->dsize[i]) return false;
            }
            return true;
        }


        bool contains(const double d0, const double d1, const double d2) const {
            const double d[] = { d0, d1, d2 };
            return contains(d);
        }


        bool contains(const Vec3& vec) const {
            const double d[] = { vec.x, vec.y, vec.z };
            return contains(d);
        }


        //--------------------------------------------------------------------------------
        // rect util
        //--------------------------------------------------------------------------------

        friend Rect3 operator & (const Rect3& rect0, const Rect3& rect1) {
            int dbase[3] = { 0 }, dsize[3] = { 0 };
            for (int i = 0; i < 3; i++) {
                dbase[i] = max(rect0.dbase[i], rect1.dbase[i]);
                dsize[i] = max(0, min(rect0.dbase[i] + rect0.dsize[i], rect1.dbase[i] + rect1.dsize[i]) - dbase[i]);
            }
            return Rect3(dbase, dsize);
        }


        friend Rect3 operator | (const Rect3& rect0, const Rect3& rect1) {
            const bool b0 = (rect0.dsize[0] * rect0.dsize[1] * rect0.dsize[2] > 0);
            const bool b1 = (rect1.dsize[0] * rect1.dsize[1] * rect1.dsize[2] > 0);
            if (b0 && b1) {
                int dbase[3] = { 0 }, dsize[3] = { 0 };
                for (int i = 0; i < 3; i++) {
                    dbase[i] = min(rect0.dbase[i], rect1.dbase[i]);
                    dsize[i] = max(rect0.dbase[i] + rect0.dsize[i], rect1.dbase[i] + rect1.dsize[i]) - dbase[i];
                    dsize[i] = max(0, dsize[i]);
                }
                return Rect3(dbase, dsize);
            }
            else {
                return (b0) ? rect0 : rect1;
            }
        }


        friend Rect3 operator + (const Rect3& rect, const int val) { return rect.extend(+val); }
        friend void operator += (Rect3& rect, const int val) { rect = rect.extend(+val); }
        friend Rect3 operator - (const Rect3& rect, const int val) { return rect.extend(-val); }
        friend void operator -= (Rect3& rect, const int val) { rect = rect.extend(-val); }

        Rect3 extend(const int val) const {
            int dbase[3] = { 0 }, dsize[3] = { 0 };
            for (int i = 0; i < 3; i++) {
                const int t = max(val, -this->dsize[i] / 2);
                dbase[i] = this->dbase[i] - t;
                dsize[i] = this->dsize[i] + 2 * t;
            }
            return Rect3(dbase, dsize);
        }

        // get center vector
        Vec3 center() const {
            Vec3 vec;
            vec.x = this->dbase[0] + (this->dsize[0] - 1) / 2.0;
            vec.y = this->dbase[1] + (this->dsize[1] - 1) / 2.0;
            vec.z = this->dbase[2] + (this->dsize[2] - 1) / 2.0;
            return vec;
        }
    };

    //--------------------------------------------------------------------------------
    // bounding box
    //--------------------------------------------------------------------------------

    class Box2 {
    private:
        bool valid;

    public:
        Vec2 pos[2];

        Box2() {
            valid = false;
        }

        Box2(const Box2& box) {
            valid = box.valid;
            pos[0] = box.pos[0];
            pos[1] = box.pos[1];
        }

        Box2(const Vec2& vec0, const Vec2& vec1) {
            valid = true;
            pos[0] = vec0;
            pos[1] = vec1;
        }

        Box2(const Vec2& vec) {
            valid = true;
            pos[0] = vec;
            pos[1] = vec;
        }

        Box2(const Mesh2& mesh) {
            *this = Box2(mesh.pos[0]) + Box2(mesh.pos[1]) + Box2(mesh.pos[2]);
        }

        friend Box2 operator + (const Box2& box0, const Box2& box1) {
            Box2 dst = box0;

            if (box0.valid == false) {
                dst = box1;
            }
            else if (box1.valid == false) {
                dst = box0;
            }
            else {
                dst.pos[0].x = min(box0.pos[0].x, box1.pos[0].x);
                dst.pos[1].x = max(box0.pos[1].x, box1.pos[1].x);
                dst.pos[0].y = min(box0.pos[0].y, box1.pos[0].y);
                dst.pos[1].y = max(box0.pos[1].y, box1.pos[1].y);
            }
            return dst;
        }

        SP_REAL area() const {
            const Vec2 d = pos[1] - pos[0];
            return d.x * d.y;
        }

        Vec2 center() const {
            return (pos[0] + pos[1]) * 0.5;
        }
    };

    class Box3 {
    private:
        bool valid;

    public:
        Vec3 pos[2];

        Box3() {
            valid = false;
        }

        Box3(const Box3& box) {
            valid = box.valid;
            pos[0] = box.pos[0];
            pos[1] = box.pos[1];
        }

        Box3(const Vec3& vec0, const Vec3& vec1) {
            valid = true;
            pos[0] = vec0;
            pos[1] = vec1;
        }

        Box3(const Vec3& vec) {
            valid = true;
            pos[0] = vec;
            pos[1] = vec;
        }

        Box3(const Mesh3& mesh) {
            *this = Box3(mesh.pos[0]) + Box3(mesh.pos[1]) + Box3(mesh.pos[2]);
        }

        friend Box3 operator + (const Box3& box0, const Box3& box1) {
            Box3 dst = box0;

            if (box0.valid == false) {
                dst = box1;
            }
            else if (box1.valid == false) {
                dst = box0;
            }
            else {
                dst.pos[0].x = min(box0.pos[0].x, box1.pos[0].x);
                dst.pos[1].x = max(box0.pos[1].x, box1.pos[1].x);
                dst.pos[0].y = min(box0.pos[0].y, box1.pos[0].y);
                dst.pos[1].y = max(box0.pos[1].y, box1.pos[1].y);
                dst.pos[0].z = min(box0.pos[0].z, box1.pos[0].z);
                dst.pos[1].z = max(box0.pos[1].z, box1.pos[1].z);
            }
            return dst;
        }

        SP_REAL area() const {
            const Vec3 d = pos[1] - pos[0];
            return d.x * d.y * d.z;
        }

        Vec3 center() const {
            return (pos[0] + pos[1]) * 0.5;
        }
    };


    //--------------------------------------------------------------------------------
    // 3d transform
    //--------------------------------------------------------------------------------

    class Rot {
    public:
        // quaternion
        SP_REAL qx, qy, qz, qw;

        Rot(const SP_REAL qx, const SP_REAL qy, const SP_REAL qz, const SP_REAL qw) {
            initialize(qx, qy, qz, qw);
        }

        Rot() {
            initialize(0.0, 0.0, 0.0, 1.0);
        }

        Rot(const SP_REAL* mat, const int rows, const int cols) {
            initialize(mat, rows, cols);
        }

        Rot(const Vec3& vec) {
            const SP_REAL angle = vec.length();
            if (angle > SP_SMALL) {
                const Vec3 nrm = vec.unit();

                const SP_REAL s = sin(angle * 0.5);
                const SP_REAL c = cos(angle * 0.5);
                initialize(s * nrm.x, s * nrm.y, s * nrm.z, c);
            }
            else {
                initialize(0.0, 0.0, 0.0, 1.0);
            }
        }

        Rot inverse() const {
            return Rot(-this->qx, -this->qy, -this->qz, this->qw);
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend Rot operator * (const Rot& rot0, const Rot& rot1) {
            const SP_REAL qx = static_cast<SP_REAL>((rot0.qw * rot1.qx) + (rot0.qx * rot1.qw) + (rot0.qy * rot1.qz) - (rot0.qz * rot1.qy));
            const SP_REAL qy = static_cast<SP_REAL>((rot0.qw * rot1.qy) + (rot0.qy * rot1.qw) + (rot0.qz * rot1.qx) - (rot0.qx * rot1.qz));
            const SP_REAL qz = static_cast<SP_REAL>((rot0.qw * rot1.qz) + (rot0.qz * rot1.qw) + (rot0.qx * rot1.qy) - (rot0.qy * rot1.qx));
            const SP_REAL qw = static_cast<SP_REAL>((rot0.qw * rot1.qw) - (rot0.qx * rot1.qx) - (rot0.qy * rot1.qy) - (rot0.qz * rot1.qz));

            return Rot(qx, qy, qz, qw);
        }

        //friend bool operator == (const Rot& rot0, const Rot& rot1) {
        //    return cmp(rot0, rot1) == true;
        //}

        //friend bool operator != (const Rot& rot0, const Rot& rot1) {
        //    return cmp(rot0, rot1) == false;
        //}

        //friend Vec3 operator * (const Rot& rot, const Vec3& vec) {
        //    return Rot::mul(rot, vec);
        //}

        //friend Vec3 operator * (const Rot& rot, const Vec2& vec) {
        //    return Rot::mul(rot, Vec3(vec.x, vec.y, 0.0));
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static Rot x(const double angle) {
            return Rot(Vec3(1.0, 0.0, 0.0) * angle);
        }

        static Rot y(const double angle) {
            return Rot(Vec3(0.0, 1.0, 0.0) * angle);
        }

        static Rot z(const double angle) {
            return Rot(Vec3(0.0, 0.0, 1.0) * angle);
        }

        static Rot axis(const Vec3& x, const Vec3& y, const Vec3& z) {
            const Vec3 nx = x.unit();
            const Vec3 ny = y.unit();
            const Vec3 nz = z.unit();
            SP_REAL mat[3 * 3];
            mat[0 * 3 + 0] = nx.x; mat[0 * 3 + 1] = ny.x; mat[0 * 3 + 2] = nz.x;
            mat[1 * 3 + 0] = nx.y; mat[1 * 3 + 1] = ny.y; mat[1 * 3 + 2] = nz.y;
            mat[2 * 3 + 0] = nx.z; mat[2 * 3 + 1] = ny.z; mat[2 * 3 + 2] = nz.z;
            return Rot(mat, 3, 3);
        }

        static bool cmp(const Rot& v0, const Rot& v1, const double t = 1.0e-6) {
            const double s0 = (v0.qw > 0.0) ? +1.0 : -1.0;
            const double s1 = (v1.qw > 0.0) ? +1.0 : -1.0;
            return ::fabs(v0.qx * s0 - v1.qx * s1) < t
                && ::fabs(v0.qy * s0 - v1.qy * s1) < t
                && ::fabs(v0.qz * s0 - v1.qz * s1) < t
                && ::fabs(v0.qw * s0 - v1.qw * s1) < t;
        }


    private:

        void initialize(const SP_REAL qx, const SP_REAL qy, const SP_REAL qz, const SP_REAL qw) {
            this->qx = qx;
            this->qy = qy;
            this->qz = qz;
            this->qw = qw;
            normalize();
        }

        void initialize(const SP_REAL* mat, const int rows, const int cols) {
            this->qx = sqrt(max(0.0, 1 + mat[0 * cols + 0] - mat[1 * cols + 1] - mat[2 * cols + 2])) / 2;
            this->qy = sqrt(max(0.0, 1 - mat[0 * cols + 0] + mat[1 * cols + 1] - mat[2 * cols + 2])) / 2;
            this->qz = sqrt(max(0.0, 1 - mat[0 * cols + 0] - mat[1 * cols + 1] + mat[2 * cols + 2])) / 2;
            this->qw = sqrt(max(0.0, 1 + mat[0 * cols + 0] + mat[1 * cols + 1] + mat[2 * cols + 2])) / 2;

            this->qx *= sign(this->qx * (mat[2 * cols + 1] - mat[1 * cols + 2]));
            this->qy *= sign(this->qy * (mat[0 * cols + 2] - mat[2 * cols + 0]));
            this->qz *= sign(this->qz * (mat[1 * cols + 0] - mat[0 * cols + 1]));
            normalize();
        }

        void normalize() {
            const double div = sqrt(this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw);
            if (div > SP_SMALL) {
                const double s = (sign(this->qw) >= 0.0) ? +1 : -1;

                this->qx = this->qx / div * s;
                this->qy = this->qy / div * s;
                this->qz = this->qz / div * s;
                this->qw = this->qw / div * s;
            }
            else {
                this->qx = 0.0;
                this->qy = 0.0;
                this->qz = 0.0;
                this->qw = 1.0;
            }
        }

        static Vec3 mul(const Rot& rot, const Vec3& vec) {
            SP_REAL mat[3 * 3];
            Rot::mat(mat, 3, 3, rot);
            return Vec3::mul(mat, 3, 3, vec);
        }

        static void mat(SP_REAL* dst, const int rows, const int cols, const Rot& rot) {
            {
                const double qx2 = rot.qx * rot.qx;
                const double qy2 = rot.qy * rot.qy;
                const double qz2 = rot.qz * rot.qz;
                const double qw2 = rot.qw * rot.qw;

                dst[0 * cols + 0] = static_cast<SP_REAL>(qw2 + qx2 - qy2 - qz2);
                dst[1 * cols + 1] = static_cast<SP_REAL>(qw2 - qx2 + qy2 - qz2);
                dst[2 * cols + 2] = static_cast<SP_REAL>(qw2 - qx2 - qy2 + qz2);
            }
            {
                const double qxy = rot.qx * rot.qy;
                const double qzw = rot.qz * rot.qw;
                dst[0 * cols + 1] = static_cast<SP_REAL>(2 * (qxy - qzw));
                dst[1 * cols + 0] = static_cast<SP_REAL>(2 * (qxy + qzw));

                const double qxz = rot.qx * rot.qz;
                const double qyw = rot.qy * rot.qw;
                dst[0 * cols + 2] = static_cast<SP_REAL>(2 * (qxz + qyw));
                dst[2 * cols + 0] = static_cast<SP_REAL>(2 * (qxz - qyw));

                const double qyz = rot.qy * rot.qz;
                const double qxw = rot.qx * rot.qw;
                dst[1 * cols + 2] = static_cast<SP_REAL>(2 * (qyz - qxw));
                dst[2 * cols + 1] = static_cast<SP_REAL>(2 * (qyz + qxw));
            }
        }
    };

    class Pose {
    public:
        // rotation
        Rot rot;

        // position
        Vec3 pos;

        Pose(const Rot& rot, const Vec3& pos) {
            this->rot = rot;
            this->pos = pos;
        }

        Pose(const Pose& pose) {
            this->rot = pose.rot;
            this->pos = pose.pos;
        }

        Pose() {
            this->rot = Rot();
            this->pos = Vec3(0.0, 0.0, 0.0);
        }

        Pose(const Rot& rot) {
            this->rot = rot;
            this->pos = Vec3(0.0, 0.0, 0.0);
        }

        Pose(const Vec3& pos) {
            this->rot = Rot();
            this->pos = pos;
        }

        Pose(const SP_REAL* mat, const int rows, const int cols) {
            initialize(mat, rows, cols);
        }

        //Pose inverse() const {
        //    Pose dst;
        //    dst.rot = this->rot.inverse();
        //    dst.pos = dst.rot * this->pos * -1.0;

        //    return dst;
        //}

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        //friend bool operator == (const Pose& v0, const Pose& v1) {
        //    return cmp(v0, v1) == true;
        //}

        //friend bool operator != (const Pose& v0, const Pose& v1) {
        //    return cmp(v0, v1) == false;
        //}

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Pose& v0, const Pose& v1, const double t = 1.0e-6) {
            return Rot::cmp(v0.rot, v1.rot, t) && Vec3::cmp(v0.pos, v1.pos, t);
        }

    private:


        void initialize(const SP_REAL* mat, const int rows, const int cols) {
            this->rot = Rot(mat, rows, cols);
            this->pos = Vec3(mat[0 * cols + 3], mat[1 * cols + 3], mat[2 * cols + 3]);
        }
    };


    //--------------------------------------------------------------------------------
    // camera parameter
    //--------------------------------------------------------------------------------

    enum CamParam_Type {
        CamParam_Pers = 0,
        CamParam_Orth = 1,
        CamParam_Fish = 2
    };

    struct CamParam {
        CamParam_Type type;
        int dsize[2];
        SP_REAL fx, fy;
        SP_REAL cx, cy;
        SP_REAL k1, k2, k3, k4;
        SP_REAL p1, p2;
    };


    //--------------------------------------------------------------------------------
    // color
    //--------------------------------------------------------------------------------

    struct Col3 {
        u08 r, g, b;
    };

    struct Col4 : public Col3 {
        u08 a;
    };

    struct Col3f {
        float r, g, b;
    };

    struct Col4f : public Col3f {
        float a;
    };


    //--------------------------------------------------------------------------------
    // material
    //--------------------------------------------------------------------------------

    struct Material {
        Col4f col;

        // transmittance : ray = tr * max(tr, rf) / (tr + rf)
        float tr;

        // refractive index 
        float ri;

        // reflectivity : ray = rf * max(tr, rf) / (tr + rf)
        float rf;

        // emission
        float em;

        // extinction
        float ex;
    };


    //--------------------------------------------------------------------------------
    // byte order
    //--------------------------------------------------------------------------------

    enum ByteOrder { BigEndian, LittleEndian };

}


//--------------------------------------------------------------------------------
// cast
//--------------------------------------------------------------------------------

namespace sp {

    //--------------------------------------------------------------------------------
    // basic type
    //--------------------------------------------------------------------------------

    template<typename TYPE> SP_GENFUNC void _cast(char& dst, const TYPE& src) {
        dst = static_cast<char>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(short& dst, const TYPE& src) {
        dst = static_cast<short>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(int& dst, const TYPE& src) {
        dst = static_cast<int>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(float& dst, const TYPE& src) {
        dst = static_cast<float>(src);
    }

    template<typename TYPE> SP_GENFUNC void _cast(double& dst, const TYPE& src) {
        dst = static_cast<double>(src);
    }

    //--------------------------------------------------------------------------------
    // byte
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(u08& dst, const int& src) {
        dst = static_cast<u08>((src < 0) ? 0 : (src > SP_BYTEMAX) ? SP_BYTEMAX : src);
    }

    SP_GENFUNC void _cast(u08& dst, const double& src) {
        dst = static_cast<u08>((src < 0) ? 0 : (src > SP_BYTEMAX) ? SP_BYTEMAX : src + 0.5);
    }

    SP_GENFUNC void _cast(u08& dst, const u08& src) {
        dst = src;
    }

    SP_GENFUNC void _cast(u08& dst, const Col3& src) {
        dst = static_cast<u08>(0.299 * src.r + 0.587 * src.g + 0.114 * src.b + 0.5);
    }

    SP_GENFUNC void _cast(u08& dst, const Col4& src) {
        dst = static_cast<u08>(0.299 * src.r + 0.587 * src.g + 0.114 * src.b + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 3
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col3& dst, const u08& src) {
        dst.r = src;
        dst.g = src;
        dst.b = src;
    }

    SP_GENFUNC void _cast(Col3& dst, const Col3& src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
    }

    SP_GENFUNC void _cast(Col3& dst, const Col4& src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
    }

    SP_GENFUNC void _cast(Col3& dst, const Vec3& src) {
        dst.r = static_cast<u08>(((src.x < 0.0) ? 0.0 : ((src.x > 1.0) ? 1.0 : src.x)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<u08>(((src.y < 0.0) ? 0.0 : ((src.y > 1.0) ? 1.0 : src.y)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<u08>(((src.z < 0.0) ? 0.0 : ((src.z > 1.0) ? 1.0 : src.z)) * SP_BYTEMAX + 0.5);
    }

    SP_GENFUNC void _cast(Col3& dst, const Col3f& src) {
        dst.r = static_cast<u08>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<u08>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<u08>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
    }

    SP_GENFUNC void _cast(Col3& dst, const Col4f& src) {
        dst.r = static_cast<u08>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<u08>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<u08>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 4
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col4& dst, const u08& src) {
        dst.r = src;
        dst.g = src;
        dst.b = src;
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4& dst, const Col3& src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4& dst, const Col4& src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = src.a;
    }

    SP_GENFUNC void _cast(Col4& dst, const Col3f& src) {
        dst.r = static_cast<u08>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<u08>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<u08>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4& dst, const Col4f& src) {
        dst.r = static_cast<u08>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<u08>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<u08>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
        dst.a = static_cast<u08>(((src.a < 0.0) ? 0.0 : ((src.a > 1.0) ? 1.0 : src.a)) * SP_BYTEMAX + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 3f
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col3f& dst, const Col3& src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col3f& dst, const Col4& src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
    }

    //--------------------------------------------------------------------------------
    // color 4f
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col4f& dst, const Col3& src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
        dst.a = 1.0;
    }

    SP_GENFUNC void _cast(Col4f& dst, const Col4& src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
        dst.a = static_cast<float>(src.a) / SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4f& dst, const Col3f& src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = 1.0f;
    }

    //--------------------------------------------------------------------------------
    // template cast
    //--------------------------------------------------------------------------------

    template<typename DST, typename SRC> SP_GENFUNC DST cast(const SRC& src) {
        DST dst;
        _cast(dst, src);
        return dst;
    }
}


//--------------------------------------------------------------------------------
// compare
//--------------------------------------------------------------------------------

namespace sp {

    // compare double
    SP_GENFUNC bool cmp(const double v0, const double v1, const double t = 1.0e-6) {
        return ((v0 - v1) < +t && (v0 - v1) > -t) ? true : false;
    }

    // compare rect
    SP_GENFUNC bool cmp(const Rect2& v0, const Rect2& v1) {
        return (v0.dbase[0] == v1.dbase[0]) && (v0.dsize[0] == v1.dsize[0]) && (v0.dbase[1] == v1.dbase[1]) && (v0.dsize[1] == v1.dsize[1]);
    }
    // compare rect
    SP_GENFUNC bool cmp(const Rect3& v0, const Rect3& v1) {
        return (v0.dbase[0] == v1.dbase[0]) && (v0.dsize[0] == v1.dsize[0]) && (v0.dbase[1] == v1.dbase[1]) && (v0.dsize[1] == v1.dsize[1]) && (v0.dbase[2] == v1.dbase[2]) && (v0.dsize[2] == v1.dsize[2]);
    }

    // compare vec
    SP_GENFUNC bool cmp(const Vec2& v0, const Vec2& v1, const double t = 1.0e-6) {
        return cmp(v0.x, v1.x, t) && cmp(v0.y, v1.y, t);
    }
    // compare vec
    SP_GENFUNC bool cmp(const Vec3& v0, const Vec3& v1, const double t = 1.0e-6) {
        return cmp(v0.x, v1.x, t) && cmp(v0.y, v1.y, t) & cmp(v0.z, v1.z, t);
    }

    // compare vec (position and normal)
    SP_GENFUNC bool cmp(const VecPD2& v0, const VecPD2& v1, const double t = 1.0e-6) {
        return cmp(v0.pos, v1.pos, t) && cmp(v0.drc, v1.drc, t);
    }
    // compare vec (position and normal)
    SP_GENFUNC bool cmp(const VecPD3& v0, const VecPD3& v1, const double t = 1.0e-6) {
        return cmp(v0.pos, v1.pos, t) && cmp(v0.drc, v1.drc, t);
    }

    // compare line
    SP_GENFUNC bool cmp(const Line2& v0, const Line2& v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) && cmp(v0.pos[1], v1.pos[1], t);
    }
    // compare line
    SP_GENFUNC bool cmp(const Line3& v0, const Line3& v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) && cmp(v0.pos[1], v1.pos[1], t);
    }

    // compare mesh
    SP_GENFUNC bool cmp(const Mesh3& v0, const Mesh3& v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) && cmp(v0.pos[1], v1.pos[1], t) && cmp(v0.pos[2], v1.pos[2], t);
    }

    // compare rotation
    SP_GENFUNC bool cmp(const Rot& v0, const Rot& v1, const double t = 1.0e-6) {
        const double s0 = (v0.qw > 0.0) ? +1.0 : -1.0;
        const double s1 = (v1.qw > 0.0) ? +1.0 : -1.0;
        return cmp(v0.qx * s0, v1.qx * s1, t) && cmp(v0.qy * s0, v1.qy * s1, t) && cmp(v0.qz * s0, v1.qz * s1, t) && cmp(v0.qw * s0, v1.qw * s1, t);
    }

    // compare pose
    SP_GENFUNC bool cmp(const Pose& v0, const Pose& v1, const double t = 1.0e-6) {
        return cmp(v0.rot, v1.rot, t) && cmp(v0.pos, v1.pos, t);
    }

    // compare color
    SP_GENFUNC bool cmp(const Col3& v0, const Col3& v1) {
        return (v0.r == v1.r) && (v0.g == v1.g) && (v0.b == v1.b);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col4& v0, const Col4& v1) {
        return (v0.r == v1.r) && (v0.g == v1.g) && (v0.b == v1.b) && (v0.a == v1.a);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col3f& v0, const Col3f& v1, const double t = 1.0e-6) {
        return cmp(v0.r, v1.r, t) && cmp(v0.g, v1.g, t) && cmp(v0.b, v1.b, t);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col4f& v0, const Col4f& v1, const double t = 1.0e-6) {
        return cmp(v0.r, v1.r, t) && cmp(v0.g, v1.g, t) && cmp(v0.b, v1.b, t) && cmp(v0.a, v1.a, t);
    }

    // compare material
    SP_GENFUNC bool cmp(const Material& v0, const Material& v1, const double t = 1.0e-6) {
        return cmp(v0.col, v1.col) && cmp(v0.rf, v1.rf, t) && cmp(v0.ri, v1.ri, t) && cmp(v0.tr, v1.tr, t) && cmp(v0.ex, v1.ex, t) && cmp(v0.em, v1.em, t);
    }

    // compare camera
    SP_GENFUNC bool cmp(const CamParam& v0, const CamParam& v1) {
        return (v0.type == v1.type) && (v0.dsize[0] == v1.dsize[0]) && (v0.dsize[1] == v1.dsize[1]) &&
            (v0.fx == v1.fx) && (v0.fy == v1.fy) && (v0.cx == v1.cx) && (v0.cy == v1.cy) && (v0.k1 == v1.k1) && (v0.k2 == v1.k2) && (v0.k3 == v1.k3) && (v0.p1 == v1.p1) && (v0.p2 == v1.p2);
    }

    // compare memory
    template<typename TYPE>
    SP_CPUFUNC bool cmp(const TYPE* mem0, const TYPE* mem1, const int size) {
        const int s = sizeof(TYPE) * size;
        const char* p0 = (char*)mem0;
        const char* p1 = (char*)mem1;

        for (int i = 0; i < s; i++) {
            if (p0[i] != p1[i]) return false;
        }
        return true;
    }

#define SP_CMP_OPERATOR(TYPE)\
    SP_GENFUNC bool operator == (const TYPE &v0, const TYPE &v1) { return  cmp(v0, v1); } \
    SP_GENFUNC bool operator != (const TYPE &v0, const TYPE &v1) { return !cmp(v0, v1); }

    SP_CMP_OPERATOR(Rect2);
    SP_CMP_OPERATOR(Rect3);
    SP_CMP_OPERATOR(Vec2);
    SP_CMP_OPERATOR(Vec3);
    SP_CMP_OPERATOR(VecPD2);
    SP_CMP_OPERATOR(VecPD3);
    SP_CMP_OPERATOR(Line2);
    SP_CMP_OPERATOR(Line3);
    SP_CMP_OPERATOR(Mesh3);
    SP_CMP_OPERATOR(Rot);
    SP_CMP_OPERATOR(Pose);
    SP_CMP_OPERATOR(Col3);
    SP_CMP_OPERATOR(Col4);
    SP_CMP_OPERATOR(Col3f);
    SP_CMP_OPERATOR(Col4f);
    SP_CMP_OPERATOR(Material);
    SP_CMP_OPERATOR(CamParam);

}

#endif