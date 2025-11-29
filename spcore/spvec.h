//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_VEC_H__
#define __SP_VEC_H__

#include "spcore/spcom.h"


namespace sp {

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
            SP_ASSERT(val < -SP_SMALL || val > +SP_SMALL);
            return Vec2(vec.x / val, vec.y / val);
        }

        void operator /= (const double val) {
            SP_ASSERT(val < -SP_SMALL || val > +SP_SMALL);
            x /= val;
            y /= val;
        }

        friend bool operator == (const Vec2& v0, const Vec2& v1) {
            return cmp(v0, v1) == true;
        }

        friend bool operator != (const Vec2& v0, const Vec2& v1) {
            return cmp(v0, v1) == false;
        }

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

        SP_REAL length() const {
            return sqrt(x * x + y * y);
        }

        Vec2 unit() const {
            return (length() > SP_SMALL) ? *this / length() : Vec2(0.0, 0.0);
        }

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

        friend bool operator == (const Vec3& v0, const Vec3& v1) {
            return cmp(v0, v1) == true;
        }

        friend bool operator != (const Vec3& v0, const Vec3& v1) {
            return cmp(v0, v1) == false;
        }

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
        friend bool operator == (const VecPD2& vec0, const VecPD2& vec1) {
            return VecPD2::cmp(vec0, vec1) == true;
        }

        friend bool operator != (const VecPD2& vec0, const VecPD2& vec1) {
            return VecPD2::cmp(vec0, vec1) == false;
        }

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
        friend bool operator == (const VecPD3& vec0, const VecPD3& vec1) {
            return VecPD3::cmp(vec0, vec1) == true;
        }

        friend bool operator != (const VecPD3& vec0, const VecPD3& vec1) {
            return VecPD3::cmp(vec0, vec1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // compare vec (position and normal)
        static bool cmp(const VecPD3& vec0, const VecPD3& vec1, const double t = 1.0e-6) {
            return Vec3::cmp(vec0.pos, vec1.pos, t) && Vec3::cmp(vec0.drc, vec1.drc, t);
        }

        //static VecPD3 mul(const SP_REAL* mat, const int rows, const int cols, const VecPD3& vec) {
        //    VecPD3 dst;

        //    dst.pos = Vec3::mul(mat, rows, cols, vec.pos);

        //    SP_REAL rot[3 * 3] = { 0 };
        //    {
        //        for (int r = 0; r < 3; r++) {
        //            for (int c = 0; c < 3; c++) {
        //                rot[r * 3 + c] = mat[r * cols + c];
        //            }
        //        }
        //    }
        //    if (rows == 4 && cols == 4) {
        //        const SP_REAL pos[3] = { dst.pos.x, dst.pos.y, dst.pos.z };
        //        for (int r = 0; r < 3; r++) {
        //            for (int c = 0; c < 3; c++) {
        //                rot[r * 3 + c] -= mat[3 * cols + c] * pos[r];
        //            }
        //        }
        //    }

        //    dst.drc = Vec3::mul(rot, 3, 3, vec.drc).unit();

        //    return dst;
        //}
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

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

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

        friend bool operator == (const Line2& line0, const Line2& line1) {
            return cmp(line0, line1) == true;
        }

        friend bool operator != (const Line2& line0, const Line2& line1) {
            return cmp(line0, line1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

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

        static bool cmp(const Line2& line0, const Line2& line1, const double t = 1.0e-6) {
            return Vec2::cmp(line0.pos[0], line1.pos[0], t)
                && Vec2::cmp(line0.pos[1], line1.pos[1], t);
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

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

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

        friend bool operator == (const Line3& line0, const Line3& line1) {
            return cmp(line0, line1) == true;
        }

        friend bool operator != (const Line3& line0, const Line3& line1) {
            return cmp(line0, line1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

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

        static bool cmp(const Line3& line0, const Line3& line1, const double t = 1.0e-6) {
            return Vec3::cmp(line0.pos[0], line1.pos[0], t)
                && Vec3::cmp(line0.pos[1], line1.pos[1], t);
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

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

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

        friend bool operator == (const Mesh3& mesh0, const Mesh3& mesh1) {
            return Mesh3::cmp(mesh0, mesh1) == true;
        }

        friend bool operator != (const Mesh3& mesh0, const Mesh3& mesh1) {
            return Mesh3::cmp(mesh0, mesh1) == false;
        }
        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        // get normal vector
        Vec3 normal() const {
            return ((pos[1] - pos[0]).cross(pos[2] - pos[0])).unit();
        }

        // get center vector
        Vec3 center() const {
            return (pos[0] + pos[1] + pos[2]) / 3.0;
        }

        static bool cmp(const Mesh3& mesh0, const Mesh3& mesh1, const double t = 1.0e-6) {
            return Vec3::cmp(mesh0.pos[0], mesh1.pos[0], t)
                && Vec3::cmp(mesh0.pos[1], mesh1.pos[1], t)
                && Vec3::cmp(mesh0.pos[2], mesh1.pos[2], t);
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
        // operator
        //--------------------------------------------------------------------------------

        friend Rect2 operator + (const Rect2& rect, const int val) { return rect.extend(+val); }
        friend void operator += (Rect2& rect, const int val) { rect = rect.extend(+val); }
        friend Rect2 operator - (const Rect2& rect, const int val) { return rect.extend(-val); }
        friend void operator -= (Rect2& rect, const int val) { rect = rect.extend(-val); }


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

        friend bool operator == (const Rect2& rect0, const Rect2& rect1) {
            return cmp(rect0, rect1) == true;
        }

        friend bool operator != (const Rect2& rect0, const Rect2& rect1) {
            return cmp(rect0, rect1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
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

        // compare rect
        static bool cmp(const Rect2& rect0, const Rect2& rect1) {
            return (rect0.dbase[0] == rect1.dbase[0]) && (rect0.dsize[0] == rect1.dsize[0])
                && (rect0.dbase[1] == rect1.dbase[1]) && (rect0.dsize[1] == rect1.dsize[1]);
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
        // operator
        //--------------------------------------------------------------------------------


        friend Rect3 operator + (const Rect3& rect, const int val) { return rect.extend(+val); }
        friend void operator += (Rect3& rect, const int val) { rect = rect.extend(+val); }
        friend Rect3 operator - (const Rect3& rect, const int val) { return rect.extend(-val); }
        friend void operator -= (Rect3& rect, const int val) { rect = rect.extend(-val); }

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

        friend bool operator == (const Rect3& rect0, const Rect3& rect1) {
            return cmp(rect0, rect1) == true;
        }

        friend bool operator != (const Rect3& rect0, const Rect3& rect1) {
            return cmp(rect0, rect1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
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

        // compare rect
        static bool cmp(const Rect3& rect0, const Rect3& rect1) {
            return (rect0.dbase[0] == rect1.dbase[0]) && (rect0.dsize[0] == rect1.dsize[0])
                && (rect0.dbase[1] == rect1.dbase[1]) && (rect0.dsize[1] == rect1.dsize[1])
                && (rect0.dbase[2] == rect1.dbase[2]) && (rect0.dsize[2] == rect1.dsize[2]);
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

}


#endif