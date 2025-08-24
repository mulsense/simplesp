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

namespace sp{

    //--------------------------------------------------------------------------------
    // type
    //--------------------------------------------------------------------------------
    
    typedef unsigned char Byte;

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

    // get clone
    template<typename TYPE> SP_GENFUNC TYPE clone(const TYPE& src) { TYPE dst = src; return dst; }

    // get max value
    SP_GENFUNC const int max(const int a, const int b) { return (a > b) ? a : b; }

    // get min value
    SP_GENFUNC const int min(const int a, const int b) { return (a < b) ? a : b; }

    // get max value
    SP_GENFUNC const SP_REAL max(const double a, const double b) { return static_cast<SP_REAL>((a > b) ? a : b); }

    // get min value
    SP_GENFUNC const SP_REAL min(const double a, const double b) { return static_cast<SP_REAL>((a < b) ? a : b); }

    // get limit value
    SP_GENFUNC const int lim(const int v, const int minv, const int maxv) { return (v > maxv) ? maxv : ((v < minv) ? minv : v); }

    // get limit value
    SP_GENFUNC const SP_REAL lim(const double v, const double minv, const double maxv) { return static_cast<SP_REAL>((v > maxv) ? maxv : ((v < minv) ? minv : v)); }

    //--------------------------------------------------------------------------------
    // complex
    //--------------------------------------------------------------------------------

    struct Cmp {
        SP_REAL re, im;
    };

    //--------------------------------------------------------------------------------
    // memory ptr
    //--------------------------------------------------------------------------------

    template<typename TYPE> struct ExPtr{
        // pointer
        TYPE *ptr;

        // dimension
        int dim;

        // dimension size
        int dsize[SP_DIMMAX];
    };

    //--------------------------------------------------------------------------------
    // rectangle
    //--------------------------------------------------------------------------------

    struct Rect2 {
        // dimension base
        int dbase[2];

        // dimension size
        int dsize[2];
    };

    struct Rect3 {
        // dimension base
        int dbase[3];

        // dimension size
        int dsize[3];
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

        // dot production
        SP_REAL dot(const Vec2& vec) const {
            return x * vec.x + y * vec.y;
        }

        SP_REAL cross(const Vec2& vec) const {
            return x * vec.y - y * vec.x;
        }

        SP_REAL sq() const {
            return x * x + y * y;
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

        // dot production
        SP_REAL dot(const Vec3& vec) const {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        Vec3 cross(const Vec3& vec) const {
            return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
        }

        SP_REAL sq() const {
            return x * x + y * y + z * z;
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
    };

    //--------------------------------------------------------------------------------
    // position and direction
    //--------------------------------------------------------------------------------
    
    struct VecPD2{
        Vec2 pos, drc;
    };

    struct VecPD3{
        Vec3 pos, drc;
    };

    //--------------------------------------------------------------------------------
    // line
    //--------------------------------------------------------------------------------

    struct Line2 {
        Vec2 pos[2];
    };

    struct Line3 {
        Vec3 pos[2];
    };

    //--------------------------------------------------------------------------------
    // triangle mesh
    //--------------------------------------------------------------------------------
    
    struct Mesh2{
        Vec2 pos[3];
    };

    struct Mesh3 {
        Vec3 pos[3];
    };

    //--------------------------------------------------------------------------------
    // bounding box
    //--------------------------------------------------------------------------------

    struct Box2 {
        Vec2 pos[2];
    };

    struct Box3 {
        Vec3 pos[2];
    };


    //--------------------------------------------------------------------------------
    // 3d transform
    //--------------------------------------------------------------------------------

    struct Rot{
        // quaternion
        SP_REAL qx, qy, qz, qw;
    };

    struct Pose{
        // rotation
        Rot rot;

        // position
        Vec3 pos;
    };


    //--------------------------------------------------------------------------------
    // camera parameter
    //--------------------------------------------------------------------------------

    enum CamParam_Type {
        CamParam_Pers = 0,
        CamParam_Orth = 1,
        CamParam_Fish = 2
    };

    struct CamParam{
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

    struct Col3{
        Byte r, g, b;
    };

    struct Col4 : public Col3{
        Byte a;
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
  
    template<typename TYPE> SP_GENFUNC void _cast(char &dst, const TYPE &src) {
        dst = static_cast<char>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(short &dst, const TYPE &src) {
        dst = static_cast<short>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(int &dst, const TYPE &src) {
        dst = static_cast<int>(src + 0.5 - (src < 0));
    }

    template<typename TYPE> SP_GENFUNC void _cast(float &dst, const TYPE &src) {
        dst = static_cast<float>(src);
    }

    template<typename TYPE> SP_GENFUNC void _cast(double &dst, const TYPE &src) {
        dst = static_cast<double>(src);
    }

    //--------------------------------------------------------------------------------
    // byte
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Byte &dst, const int &src) {
        dst = static_cast<Byte>((src < 0) ? 0 : (src > SP_BYTEMAX) ? SP_BYTEMAX : src);
    }

    SP_GENFUNC void _cast(Byte &dst, const double &src) {
        dst = static_cast<Byte>((src < 0) ? 0 : (src > SP_BYTEMAX) ? SP_BYTEMAX : src + 0.5);
    }

    SP_GENFUNC void _cast(Byte &dst, const Byte &src) {
        dst = src;
    }

    SP_GENFUNC void _cast(Byte &dst, const Col3 &src) {
        dst = static_cast<Byte>(0.299 * src.r + 0.587 * src.g + 0.114 * src.b + 0.5);
    }
 
    SP_GENFUNC void _cast(Byte &dst, const Col4 &src) {
        dst = static_cast<Byte>(0.299 * src.r + 0.587 * src.g + 0.114 * src.b + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 3
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col3 &dst, const Byte &src) {
        dst.r = src;
        dst.g = src;
        dst.b = src;
    }

    SP_GENFUNC void _cast(Col3 &dst, const Col3 &src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
    }

    SP_GENFUNC void _cast(Col3 &dst, const Col4 &src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
    }

    SP_GENFUNC void _cast(Col3 &dst, const Vec3 &src) {
        dst.r = static_cast<Byte>(((src.x < 0.0) ? 0.0 : ((src.x > 1.0) ? 1.0 : src.x)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<Byte>(((src.y < 0.0) ? 0.0 : ((src.y > 1.0) ? 1.0 : src.y)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<Byte>(((src.z < 0.0) ? 0.0 : ((src.z > 1.0) ? 1.0 : src.z)) * SP_BYTEMAX + 0.5);
    }

    SP_GENFUNC void _cast(Col3 &dst, const Col3f &src) {
        dst.r = static_cast<Byte>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<Byte>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<Byte>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
    }

    SP_GENFUNC void _cast(Col3 &dst, const Col4f &src) {
        dst.r = static_cast<Byte>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<Byte>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<Byte>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 4
    //--------------------------------------------------------------------------------
  
    SP_GENFUNC void _cast(Col4 &dst, const Byte &src) {
        dst.r = src;
        dst.g = src;
        dst.b = src;
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4 &dst, const Col3 &src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4 &dst, const Col4 &src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = src.a;
    }

    SP_GENFUNC void _cast(Col4 &dst, const Col3f &src) {
        dst.r = static_cast<Byte>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<Byte>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<Byte>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
        dst.a = SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4 &dst, const Col4f &src) {
        dst.r = static_cast<Byte>(((src.r < 0.0) ? 0.0 : ((src.r > 1.0) ? 1.0 : src.r)) * SP_BYTEMAX + 0.5);
        dst.g = static_cast<Byte>(((src.g < 0.0) ? 0.0 : ((src.g > 1.0) ? 1.0 : src.g)) * SP_BYTEMAX + 0.5);
        dst.b = static_cast<Byte>(((src.b < 0.0) ? 0.0 : ((src.b > 1.0) ? 1.0 : src.b)) * SP_BYTEMAX + 0.5);
        dst.a = static_cast<Byte>(((src.a < 0.0) ? 0.0 : ((src.a > 1.0) ? 1.0 : src.a)) * SP_BYTEMAX + 0.5);
    }

    //--------------------------------------------------------------------------------
    // color 3f
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col3f &dst, const Col3 &src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col3f &dst, const Col4 &src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
    }

    //--------------------------------------------------------------------------------
    // color 4f
    //--------------------------------------------------------------------------------

    SP_GENFUNC void _cast(Col4f &dst, const Col3 &src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
        dst.a = 1.0;
    }

    SP_GENFUNC void _cast(Col4f &dst, const Col4 &src) {
        dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
        dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
        dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
        dst.a = static_cast<float>(src.a) / SP_BYTEMAX;
    }

    SP_GENFUNC void _cast(Col4f &dst, const Col3f &src) {
        dst.r = src.r;
        dst.g = src.g;
        dst.b = src.b;
        dst.a = 1.0f;
    }

    //--------------------------------------------------------------------------------
    // template cast
    //--------------------------------------------------------------------------------

    template<typename DST, typename SRC> SP_GENFUNC DST cast(const SRC &src) {
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

    // compare complex
    SP_GENFUNC bool cmp(const Cmp &v0, const Cmp &v1, const double t = 1.0e-6) {
        return cmp(v0.re, v1.re, t) & cmp(v0.im, v1.im, t);
    }

    // compare rect
    SP_GENFUNC bool cmp(const Rect2 &v0, const Rect2 &v1) {
        return (v0.dbase[0] == v1.dbase[0]) & (v0.dsize[0] == v1.dsize[0]) & (v0.dbase[1] == v1.dbase[1]) & (v0.dsize[1] == v1.dsize[1]);
    }
    // compare rect
    SP_GENFUNC bool cmp(const Rect3 &v0, const Rect3 &v1) {
        return (v0.dbase[0] == v1.dbase[0]) & (v0.dsize[0] == v1.dsize[0]) & (v0.dbase[1] == v1.dbase[1]) & (v0.dsize[1] == v1.dsize[1]) & (v0.dbase[2] == v1.dbase[2]) & (v0.dsize[2] == v1.dsize[2]);
    }

    // compare vec
    SP_GENFUNC bool cmp(const Vec2 &v0, const Vec2 &v1, const double t = 1.0e-6) {
        return cmp(v0.x, v1.x, t) & cmp(v0.y, v1.y, t);
    }
    // compare vec
    SP_GENFUNC bool cmp(const Vec3 &v0, const Vec3 &v1, const double t = 1.0e-6) {
        return cmp(v0.x, v1.x, t) & cmp(v0.y, v1.y, t) & cmp(v0.z, v1.z, t);
    }

    // compare vec (position and normal)
    SP_GENFUNC bool cmp(const VecPD2 &v0, const VecPD2 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos, v1.pos, t) & cmp(v0.drc, v1.drc, t);
    }
    // compare vec (position and normal)
    SP_GENFUNC bool cmp(const VecPD3 &v0, const VecPD3 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos, v1.pos, t) & cmp(v0.drc, v1.drc, t);
    }
    
    // compare line
    SP_GENFUNC bool cmp(const Line2 &v0, const Line2 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) & cmp(v0.pos[1], v1.pos[1], t);
    }
    // compare line
    SP_GENFUNC bool cmp(const Line3 &v0, const Line3 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) & cmp(v0.pos[1], v1.pos[1], t);
    }

    // compare mesh
    SP_GENFUNC bool cmp(const Mesh2 &v0, const Mesh2 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) & cmp(v0.pos[1], v1.pos[1], t) & cmp(v0.pos[2], v1.pos[2], t);
    }
    // compare mesh
    SP_GENFUNC bool cmp(const Mesh3 &v0, const Mesh3 &v1, const double t = 1.0e-6) {
        return cmp(v0.pos[0], v1.pos[0], t) & cmp(v0.pos[1], v1.pos[1], t) & cmp(v0.pos[2], v1.pos[2], t);
    }

    // compare rotation
    SP_GENFUNC bool cmp(const Rot &v0, const Rot &v1, const double t = 1.0e-6) {
        const double s0 = (v0.qw > 0.0) ? +1.0 : -1.0;
        const double s1 = (v1.qw > 0.0) ? +1.0 : -1.0;
        return cmp(v0.qx * s0, v1.qx * s1, t) & cmp(v0.qy * s0, v1.qy * s1, t) & cmp(v0.qz * s0, v1.qz * s1, t) & cmp(v0.qw * s0, v1.qw * s1, t);
    }

    // compare pose
    SP_GENFUNC bool cmp(const Pose &v0, const Pose &v1, const double t = 1.0e-6) {
        return cmp(v0.rot, v1.rot, t) & cmp(v0.pos, v1.pos, t);
    }

    // compare color
    SP_GENFUNC bool cmp(const Col3 &v0, const Col3 &v1) {
        return (v0.r == v1.r) & (v0.g == v1.g) & (v0.b == v1.b);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col4 &v0, const Col4 &v1) {
        return (v0.r == v1.r) & (v0.g == v1.g) & (v0.b == v1.b) & (v0.a == v1.a);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col3f &v0, const Col3f &v1, const double t = 1.0e-6) {
        return cmp(v0.r, v1.r, t) & cmp(v0.g, v1.g, t) & cmp(v0.b, v1.b, t);
    }
    // compare color
    SP_GENFUNC bool cmp(const Col4f &v0, const Col4f &v1, const double t = 1.0e-6) {
        return cmp(v0.r, v1.r, t) & cmp(v0.g, v1.g, t) & cmp(v0.b, v1.b, t) & cmp(v0.a, v1.a, t);
    }

    // compare material
    SP_GENFUNC bool cmp(const Material &v0, const Material &v1, const double t = 1.0e-6) {
        return cmp(v0.col, v1.col) & cmp(v0.rf, v1.rf, t) & cmp(v0.ri, v1.ri, t) & cmp(v0.tr, v1.tr, t) & cmp(v0.ex, v1.ex, t) & cmp(v0.em, v1.em, t);
    }

    // compare camera
    SP_GENFUNC bool cmp(const CamParam &v0, const CamParam &v1) {
        return (v0.type == v1.type) & (v0.dsize[0] == v1.dsize[0]) & (v0.dsize[1] == v1.dsize[1]) &
            (v0.fx == v1.fx) & (v0.fy == v1.fy) & (v0.cx == v1.cx) & (v0.cy == v1.cy) & (v0.k1 == v1.k1) & (v0.k2 == v1.k2) & (v0.k3 == v1.k3) & (v0.p1 == v1.p1) & (v0.p2 == v1.p2);
    }

    // compare memory
    template<typename TYPE>
    SP_CPUFUNC bool cmp(const TYPE *mem0, const TYPE *mem1, const int size) {
        const int s = sizeof(TYPE) * size;
        const char *p0 = (char*)mem0;
        const char *p1 = (char*)mem1;

        for (int i = 0; i < s; i++) {
            if (p0[i] != p1[i]) return false;
        }
        return true;
    }

#define SP_CMP_OPERATOR(TYPE)\
    SP_GENFUNC bool operator == (const TYPE &v0, const TYPE &v1) { return  cmp(v0, v1); } \
    SP_GENFUNC bool operator != (const TYPE &v0, const TYPE &v1) { return !cmp(v0, v1); }

    SP_CMP_OPERATOR(Cmp);
    SP_CMP_OPERATOR(Rect2);
    SP_CMP_OPERATOR(Rect3);
    SP_CMP_OPERATOR(Vec2);
    SP_CMP_OPERATOR(Vec3);
    SP_CMP_OPERATOR(VecPD2);
    SP_CMP_OPERATOR(VecPD3);
    SP_CMP_OPERATOR(Line2);
    SP_CMP_OPERATOR(Line3);
    SP_CMP_OPERATOR(Mesh2);
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