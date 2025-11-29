//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_POSE_H__
#define __SP_POSE_H__

#include "spcore/spcom.h"
#include "spcore/spvec.h"


namespace sp {


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

        friend bool operator == (const Rot& rot0, const Rot& rot1) {
            return cmp(rot0, rot1) == true;
        }

        friend bool operator != (const Rot& rot0, const Rot& rot1) {
            return cmp(rot0, rot1) == false;
        }

        friend Vec3 operator * (const Rot& rot, const Vec3& vec) {
            return Rot::mul(rot, vec);
        }

        friend Vec3 operator * (const Rot& rot, const Vec2& vec) {
            return Rot::mul(rot, Vec3(vec.x, vec.y, 0.0));
        }

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

        Pose inverse() const {
            Pose dst;
            dst.rot = this->rot.inverse();
            dst.pos = dst.rot * this->pos * -1.0;

            return dst;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        //friend Vec3 operator * (const Pose& pose, const Vec3& vec) {
        //    return Pose::mul(pose, vec);
        //}

        //friend Vec3 operator * (const Pose& pose, const Vec2& vec) { return Pose::mul(pose, vec); }

        ////SP_GENFUNC VecPD3 operator * (const Pose& pose, const VecPD3& vec) { return mulPose(pose, vec); }
        ////SP_GENFUNC Line3 operator * (const Pose& pose, const Line3& line) { return mulPose(pose, line); }
        ////SP_GENFUNC Mesh3 operator * (const Pose& pose, const Mesh3& mesh) { return mulPose(pose, mesh); }

        //friend Pose operator * (const Pose& pose0, const Pose& pose1) { return Pose::mul(pose0, pose1); }
        //friend void operator *= (Pose& pose0, const Pose& pose1) { pose0 = Pose::mul(pose0, pose1); }

        //friend Pose operator * (const Rot& rot, const Pose& pose) { return Pose::mul(Pose(rot), pose); }
        //friend Pose operator * (const Pose& pose, const Rot& rot) { return Pose::mul(pose, Pose(rot)); }
        //friend void operator *= (Pose& pose, const Rot& rot) { pose = Pose::mul(pose, Pose(rot)); }

        friend bool operator == (const Pose& v0, const Pose& v1) {
            return cmp(v0, v1) == true;
        }

        friend bool operator != (const Pose& v0, const Pose& v1) {
            return cmp(v0, v1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static Pose mul(const Pose& pose0, const Pose& pose1) {
            Pose dst;
            dst.rot = pose0.rot * pose1.rot;
            dst.pos = Rot::mul(pose0.rot, pose1.pos) + pose0.pos;
            return dst;
        }

        static Vec3 mul(const Pose& pose, const Vec3& vec) {
            SP_REAL mat[3 * 4];
            Pose::mat(mat, 3, 4, pose);
            return Vec3::mul(mat, 3, 4, vec);
        }

        static Vec3 mul(const Pose& pose, const Vec2& vec) {
            return mul(pose, Vec3(vec.x, vec.y, 0.0));
        }

        //static VecPD3 mul(const Pose& pose, const VecPD3& vec) {
        //    SP_REAL mat[3 * 4];
        //    Pose::mat(mat, 3, 4, pose);

        //    return Vec3::mul(mat, 3, 4, vec);
        //}

        //SP_GENFUNC Line3 mulPose(const Pose& pose, const Line3& line) {
        //    SP_REAL poseMat[3 * 4];
        //    getMat(poseMat, 3, 4, pose);

        //    return mulMat(poseMat, 3, 4, line);
        //}

        //SP_GENFUNC Mesh3 mulPose(const Pose& pose, const Mesh3& mesh) {
        //    SP_REAL poseMat[3 * 4];
        //    getMat(poseMat, 3, 4, pose);

        //    return mulMat(poseMat, 3, 4, mesh);
        //}


        static bool cmp(const Pose& v0, const Pose& v1, const double t = 1.0e-6) {
            return Rot::cmp(v0.rot, v1.rot, t) && Vec3::cmp(v0.pos, v1.pos, t);
        }

        static void mat(SP_REAL* dst, const int rows, const int cols, const Pose& pose) {
            Rot::mat(dst, rows, cols, pose.rot);
            dst[0 * cols + 3] = pose.pos.x;
            dst[1 * cols + 3] = pose.pos.y;
            dst[2 * cols + 3] = pose.pos.z;
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

    class CamParam {
    public:
        CamParam_Type type;
        int dsize[2];
        SP_REAL fx, fy;
        SP_REAL cx, cy;
        SP_REAL k1, k2, k3, k4;
        SP_REAL p1, p2;

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const CamParam& cam0, const CamParam& cam1) {
            return CamParam::cmp(cam0, cam1) == true;
        }

        friend bool operator != (const CamParam& cam0, const CamParam& cam1) {
            return CamParam::cmp(cam0, cam1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const CamParam& cam0, const CamParam& cam1) {
            return (cam0.type == cam1.type) && (cam0.dsize[0] == cam1.dsize[0]) && (cam0.dsize[1] == cam1.dsize[1]) &&
                (cam0.fx == cam1.fx) && (cam0.fy == cam1.fy) && (cam0.cx == cam1.cx) && (cam0.cy == cam1.cy) && (cam0.k1 == cam1.k1) && (cam0.k2 == cam1.k2) && (cam0.k3 == cam1.k3) && (cam0.p1 == cam1.p1) && (cam0.p2 == cam1.p2);
        }
    };


    //--------------------------------------------------------------------------------
    // color
    //--------------------------------------------------------------------------------

    class Col3 {
    public:
        u08 r, g, b;

        Col3() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }

        Col3(const u08 r, const u08 g, const u08 b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const Col3& col0, const Col3& col1) {
            return Col3::cmp(col0, col1) == true;
        }

        friend bool operator != (const Col3& col0, const Col3& col1) {
            return Col3::cmp(col0, col1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Col3& col0, const Col3& col1) {
            return (col0.r == col1.r) && (col0.g == col1.g) && (col0.b == col1.b);
        }
    };

    class Col4 : public Col3 {
    public:
        u08 a;

        Col4() : Col3() {
            this->a = 0;
        }

        Col4(const u08 r, const u08 g, const u08 b, const u08 a) : Col3(a, g, b) {
            this->a = a;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const Col4& col0, const Col4& col1) {
            return Col4::cmp(col0, col1) == true;
        }

        friend bool operator != (const Col4& col0, const Col4& col1) {
            return Col4::cmp(col0, col1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Col4& col0, const Col4& col1) {
            return (col0.r == col1.r) && (col0.g == col1.g) && (col0.b == col1.b) && (col0.a == col1.a);
        }
    };

    class Col3f {
    public:
        float r, g, b;

        Col3f() {
            this->r = 0.0f;
            this->g = 0.0f;
            this->b = 0.0f;
        }

        Col3f(const float r, const float g, const float b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const Col3f& col0, const Col3f& col1) {
            return Col3f::cmp(col0, col1) == true;
        }

        friend bool operator != (const Col3f& col0, const Col3f& col1) {
            return Col3f::cmp(col0, col1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Col3f& col0, const Col3f& col1, const double t = 1.0e-6) {
            return ::fabs(col0.r - col1.r) < t
                && ::fabs(col0.g - col1.g) < t
                && ::fabs(col0.b - col1.b) < t;
        }
    };

    class Col4f : public Col3f {
    public:
        float a;

        Col4f() : Col3f() {
            this->a = 0.0f;
        }

        Col4f(const float r, const float g, const float b, const float a) : Col3f(r, g, b) {
            this->a = a;
        }

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const Col4f& col0, const Col4f& col1) {
            return Col4f::cmp(col0, col1) == true;
        }

        friend bool operator != (const Col4f& col0, const Col4f& col1) {
            return Col4f::cmp(col0, col1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Col4f& col0, const Col4f& col1, const double t = 1.0e-6) {
            return ::fabs(col0.r - col1.r) < t
                && ::fabs(col0.g - col1.g) < t
                && ::fabs(col0.b - col1.b) < t
                && ::fabs(col0.a - col1.a) < t;
        }

        static Col4f cast(const Col3& src) {
            Col4f dst;
            dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
            dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
            dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
            dst.a = 1.0;
            return dst;
        }

        static Col4f cast(const Col4& src) {
            Col4f dst;
            dst.r = static_cast<float>(src.r) / SP_BYTEMAX;
            dst.g = static_cast<float>(src.g) / SP_BYTEMAX;
            dst.b = static_cast<float>(src.b) / SP_BYTEMAX;
            dst.a = static_cast<float>(src.a) / SP_BYTEMAX;
            return dst;
        }

        static Col4f cast(const Col3f& src) {
            Col4f dst;
            dst.r = src.r;
            dst.g = src.g;
            dst.b = src.b;
            dst.a = 1.0f;
            return dst;
        }
    };


    //--------------------------------------------------------------------------------
    // material
    //--------------------------------------------------------------------------------

    class Material {
    public:
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

        //--------------------------------------------------------------------------------
        // operator
        //--------------------------------------------------------------------------------

        friend bool operator == (const Material& material0, const Material& material1) {
            return Material::cmp(material0, material1) == true;
        }

        friend bool operator != (const Material& material0, const Material& material1) {
            return Material::cmp(material0, material1) == false;
        }

        //--------------------------------------------------------------------------------
        // util
        //--------------------------------------------------------------------------------

        static bool cmp(const Material& v0, const Material& v1, const double t = 1.0e-6) {
            return Col4f::cmp(v0.col, v1.col)
                && ::fabs(v0.rf - v1.rf) < t
                && ::fabs(v0.ri - v1.ri) < t
                && ::fabs(v0.tr - v1.tr) < t
                && ::fabs(v0.ex - v1.ex) < t
                && ::fabs(v0.em - v1.em) < t;
        }

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
    // template cast
    //--------------------------------------------------------------------------------

    template<typename DST, typename SRC> SP_GENFUNC DST cast(const SRC& src) {
        DST dst;
        _cast(dst, src);
        return dst;
    }


}



#endif