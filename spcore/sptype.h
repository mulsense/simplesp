//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_TYPE_H__
#define __SP_TYPE_H__

#include "spcore/spcom.h"
#include "spcore/spbase.h"
#include "spcore/spmath.h"

//--------------------------------------------------------------------------------
// vector
//--------------------------------------------------------------------------------

namespace sp {

    
    //--------------------------------------------------------------------------------
    // vector util
    //--------------------------------------------------------------------------------

    // projection vec3 to vec2
    SP_GENFUNC Vec2 prjVec(const Vec3 &vec, const bool pers = true) {
        return (pers == true) ? Vec2(vec.x, vec.y) / vec.z : Vec2(vec.x, vec.y);
    }
    // projection vec2 to vec3
    SP_GENFUNC Vec3 prjVec(const Vec2 &vec, const double z, const bool pers = true) {
        return (pers == true) ? Vec3(vec.x, vec.y, 1.0) * z : Vec3(vec.x, vec.y, z);
    }


    //--------------------------------------------------------------------------------
    // matrix * vector
    //--------------------------------------------------------------------------------

    SP_GENFUNC Vec2 mulMat(const SP_REAL *mat, const int rows, const int cols, const Vec2 &vec) {
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

    SP_GENFUNC Vec3 mulMat(const SP_REAL *mat, const int rows, const int cols, const Vec3 &vec) {
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



    //--------------------------------------------------------------------------------
    // matrix * vector pn
    //--------------------------------------------------------------------------------

    SP_GENFUNC VecPD2 mulMat(const SP_REAL *mat, const int rows, const int cols, const VecPD2 &vec) {
        VecPD2 dst;

        dst.pos = mulMat(mat, rows, cols, vec.pos);

        SP_REAL rot[2 * 2] = { 0 };
        {
            for (int r = 0; r < 2; r++) {
                for (int c = 0; c < 2; c++) {
                    rot[r * 2 + c] = mat[r * cols + c];
                }
            }
        }
        if (rows == 3 && cols == 3) {
            const SP_REAL pos[2] = { dst.pos.x, dst.pos.y };
            for (int r = 0; r < 2; r++) {
                for (int c = 0; c < 2; c++) {
                    rot[r * 2 + c] -= mat[2 * cols + c] * pos[r];
                }
            }
        }

        dst.drc = mulMat(rot, 2, 2, vec.drc).unit();

        return dst;
    }

    SP_GENFUNC VecPD3 mulMat(const SP_REAL *mat, const int rows, const int cols, const VecPD3 &vec) {
        VecPD3 dst;

        dst.pos = mulMat(mat, rows, cols, vec.pos);

        SP_REAL rot[3 * 3] = { 0 };
        {
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    rot[r * 3 + c] = mat[r * cols + c];
                }
            }
        }
        if (rows == 4 && cols == 4) {
            const SP_REAL pos[3] = { dst.pos.x, dst.pos.y, dst.pos.z };
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    rot[r * 3 + c] -= mat[3 * cols + c] * pos[r];
                }
            }
        }

        dst.drc = mulMat(rot, 3, 3, vec.drc).unit();

        return dst;
    }


    //--------------------------------------------------------------------------------
    // line util
    //--------------------------------------------------------------------------------
    
    // projection vec3 to vec2
    SP_GENFUNC Line2 prjVec(const Line3 &line, const bool pers = true) {
        return Line2(prjVec(line.pos[0], pers), prjVec(line.pos[1], pers));
    }


    //--------------------------------------------------------------------------------
    // matrix * line
    //--------------------------------------------------------------------------------

    SP_GENFUNC Line2 mulMat(const SP_REAL *mat, const int rows, const int cols, const Line2 &line) {
        Line2 dst;
        dst.pos[0] = mulMat(mat, rows, cols, line.pos[0]);
        dst.pos[1] = mulMat(mat, rows, cols, line.pos[1]);
        return dst;
    }

    SP_GENFUNC Line3 mulMat(const SP_REAL *mat, const int rows, const int cols, const Line3 &line) {
        Line3 dst;
        dst.pos[0] = mulMat(mat, rows, cols, line.pos[0]);
        dst.pos[1] = mulMat(mat, rows, cols, line.pos[1]);
        return dst;
    }



    //--------------------------------------------------------------------------------
    // matrix * mesh
    //--------------------------------------------------------------------------------

    SP_GENFUNC Mesh3 mulMat(const SP_REAL *mat, const int rows, const int cols, const Mesh3 &mesh) {
        Mesh3 dst;
        dst.pos[0] = mulMat(mat, rows, cols, mesh.pos[0]);
        dst.pos[1] = mulMat(mat, rows, cols, mesh.pos[1]);
        dst.pos[2] = mulMat(mat, rows, cols, mesh.pos[2]);
        return dst;
    }


    
    //--------------------------------------------------------------------------------
    // geodesic dorm
    //--------------------------------------------------------------------------------

    // geodesic mesh num
    SP_GENFUNC int getGeodesicMeshNum(const int level) {
        int ret = 20;
        for (int i = 0; i < level; i++) {
            ret *= 4;
        }
        return ret;
    }

    SP_GENFUNC Mesh3 getGeodesicMesh(const int level, const int id) {
        Mesh3 model[20];

        // init regular geodesic dorm (vertex num 12)
        {
            int cnt;

            Vec3 p[12];
            const double u = (1.0 + sqrt(5.0)) / 2.0;
            // vertex (0, ±1, ±u), (±u, 0, ±1), (±1, ±u, 0)

            cnt = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        const double s[2] = { -1.0, +1.0 };
                        const double v[3] = { 0.0, s[j], s[k] * u };
                        p[cnt++] = Vec3(v[(i + 0) % 3], v[(i + 1) % 3], v[(i + 2) % 3]);
                    }
                }
            }

            cnt = 0;
            for (int i = 0; i < 12; i++) {
                for (int j = i + 1; j < 12; j++) {
                    for (int k = j + 1; k < 12; k++) {
                        if (cmp(p[i].dot(p[j]), u, 0.001) == false) continue;
                        if (cmp(p[j].dot(p[k]), u, 0.001) == false) continue;
                        if (cmp(p[k].dot(p[i]), u, 0.001) == false) continue;
                        model[cnt++] = Mesh3(p[i].unit(), p[j].unit(), p[k].unit());
                    }
                }
            }

            for (int i = 0; i < 20; i++) {
                Mesh3 &m = model[i];
                if (m.normal().dot(m.center()) < 0.0) {
                    swap(m.pos[1], m.pos[2]);
                }
            }
        }

        // div regular geodesic dorm
        int num = getGeodesicMeshNum(level) / 20;
        int tmp = id;

        Mesh3 dst = model[tmp / num];
        for (int d = 0; d < level; d++) {
            const Vec3 p0 = (dst.pos[0] + dst.pos[1]).unit();
            const Vec3 p1 = (dst.pos[1] + dst.pos[2]).unit();
            const Vec3 p2 = (dst.pos[2] + dst.pos[0]).unit();

            Mesh3 mesh[4];
            mesh[0] = Mesh3(p0, p1, p2);
            mesh[1] = Mesh3(dst.pos[0], p0, p2);
            mesh[2] = Mesh3(dst.pos[1], p1, p0);
            mesh[3] = Mesh3(dst.pos[2], p2, p1);

            tmp %= num;
            num /= 4;
            dst = mesh[tmp / num];
        }
        return dst;
    }
}


//--------------------------------------------------------------------------------
// color
//--------------------------------------------------------------------------------

namespace sp {
   
    // get color
    SP_GENFUNC Col3 getCol3(const u08 r, const u08 g, const u08 b) {
        Col3 dst;
        dst.r = r;
        dst.g = g;
        dst.b = b;
        return dst;
    }

    // get color
    SP_GENFUNC Col4 getCol4(const u08 r, const u08 g, const u08 b, const u08 a) {
        Col4 dst;
        dst.r = r;
        dst.g = g;
        dst.b = b;
        dst.a = a;
        return dst;
    }

    // get color
    SP_GENFUNC Col4 getCol4(const Col3 &col, const u08 a) {
        return getCol4(col.r, col.g, col.b, a);
    }

    // get color
    SP_GENFUNC Col3f getCol3f(const double r, const double g, const double b) {
        Col3f dst;
        dst.r = static_cast<float>(r);
        dst.g = static_cast<float>(g);
        dst.b = static_cast<float>(b);
        return dst;
    }

    // get color
    SP_GENFUNC Col4f getCol4f(const double r, const double g, const double b, const double a) {
        Col4f dst;
        dst.r = static_cast<float>(r);
        dst.g = static_cast<float>(g);
        dst.b = static_cast<float>(b);
        dst.a = static_cast<float>(a);
        return dst;
    }

    // get color
    SP_GENFUNC Col4f getCol4f(const Col3f &col, const double a) {
        return getCol4f(col.r, col.g, col.b, a);
    }

    //--------------------------------------------------------------------------------
    // color operator
    //--------------------------------------------------------------------------------

    SP_GENFUNC Col3f addCol(const Col3f &col0, const Col3f &col1) { return getCol3f(col0.r + col1.r, col0.g + col1.g, col0.b + col1.b); }
    SP_GENFUNC Col4f addCol(const Col4f &col0, const Col4f &col1) { return getCol4f(col0.r + col1.r, col0.g + col1.g, col0.b + col1.b, col0.a + col1.a); }

    SP_GENFUNC Col3f subCol(const Col3f &col0, const Col3f &col1) { return getCol3f(col0.r - col1.r, col0.g - col1.g, col0.b - col1.b); }
    SP_GENFUNC Col4f subCol(const Col4f &col0, const Col4f &col1) { return getCol4f(col0.r - col1.r, col0.g - col1.g, col0.b - col1.b, col0.a - col1.a); }

    SP_GENFUNC Col3f mulCol(const Col3f &col, const double val) { return getCol3f(col.r * val, col.g * val, col.b * val); }
    SP_GENFUNC Col4f mulCol(const Col4f &col, const double val) { return getCol4f(col.r * val, col.g * val, col.b * val, col.a * val); }

    SP_GENFUNC Col3f divCol(const Col3f &col, const double val) { SP_ASSERT(fabs(val) > SP_SMALL); return getCol3f(col.r / val, col.g / val, col.b / val); }
    SP_GENFUNC Col4f divCol(const Col4f &col, const double val) { SP_ASSERT(fabs(val) > SP_SMALL); return getCol4f(col.r / val, col.g / val, col.b / val, col.a / val); }

    SP_GENFUNC Col3f operator + (const Col3f &col0, const Col3f &col1) { return addCol(col0, col1); }
    SP_GENFUNC Col4f operator + (const Col4f &col0, const Col4f &col1) { return addCol(col0, col1); }
    SP_GENFUNC void operator += (Col3f &col0, const Col3f &col1) { col0 = addCol(col0, col1); }
    SP_GENFUNC void operator += (Col4f &col0, const Col4f &col1) { col0 = addCol(col0, col1); }

    SP_GENFUNC Col3f operator * (const Col3f &col, const double val) { return mulCol(col, val); }
    SP_GENFUNC Col4f operator * (const Col4f &col, const double val) { return mulCol(col, val); }
    SP_GENFUNC void operator *= (Col3f &col, const double val) { col = mulCol(col, val); }
    SP_GENFUNC void operator *= (Col4f &col, const double val) { col = mulCol(col, val); }

    SP_GENFUNC Col3f operator / (const Col3f &col, const double val) { return divCol(col, val); }
    SP_GENFUNC Col4f operator / (const Col4f &col, const double val) { return divCol(col, val); }
    SP_GENFUNC void operator /= (Col3f &col, const double val) { col = divCol(col, val); }
    SP_GENFUNC void operator /= (Col4f &col, const double val) { col = divCol(col, val); }

    //--------------------------------------------------------------------------------
    // color space
    //--------------------------------------------------------------------------------

    // convert phase to col3(rainbow), phase = [0, 1]
    SP_GENFUNC void cnvPhaseToCol(Col3 &col, const double phase) {
        const double p = max(0.0, min(phase, 1.0));

        col.r = static_cast<u08>(255 * (sin(1.5 * SP_PI * p + SP_PI * (9.0 / 4.0)) + 1.0) / 2.0);
        col.g = static_cast<u08>(255 * (sin(1.5 * SP_PI * p + SP_PI * (7.0 / 4.0)) + 1.0) / 2.0);
        col.b = static_cast<u08>(255 * (sin(1.5 * SP_PI * p + SP_PI * (5.0 / 4.0)) + 1.0) / 2.0);
    }

    // convert hsv to col3, hsv = Vec3(h = [0, 2 * PI], s = [0, 1], v = [0, 1])
    SP_GENFUNC void cnvHSVToCol(Col3 &col, const Vec3 &hsv) {
        const double h = hsv.x;
        const double s = hsv.y;
        const double v = hsv.z;

        const double r = (h < 0 || h >= 2 * SP_PI) ? 0 : h;
        const double D = (r * 180.0 / SP_PI) / 60.0;

        const double f = D - floor(D);
        const u08 uv = static_cast<u08>(v * 255.0 + 0.5);
        const u08 ua = static_cast<u08>(v * 255.0 * (1.0 - s) + 0.5);
        const u08 ub = static_cast<u08>(v * 255.0 * (1.0 - s * f) + 0.5);
        const u08 uc = static_cast<u08>(v * 255.0 * (1.0 - s * (1.0 - f)) + 0.5);

        switch (floor(D) % 6) {
        case 0: col = getCol3(uv, uc, ua); break;
        case 1: col = getCol3(ub, uv, ua); break;
        case 2: col = getCol3(ua, uv, uc); break;
        case 3: col = getCol3(ua, ub, uv); break;
        case 4: col = getCol3(uc, ua, uv); break;
        case 5: col = getCol3(uv, ua, ub); break;
        }
    }

    // convert col3 to hsv, hsv = Vec3(h = [0, 2 * PI], s = [0, 1], v = [0, 1])
    SP_GENFUNC void cnvColToHSV(Vec3 &hsv, const Col3 &col) {
        const double maxv = max(col.r, max(col.g, col.b));
        const double minv = min(col.r, min(col.g, col.b));
        const double subv = maxv - minv;

        double h, s, v;
        {
            h = 0.0;
            v = maxv / 255.0;
            s = subv / max(maxv, 1.0);
        }
        if (subv == 0.0) {
            h = 0.0;
        }
        else if (col.r == maxv) {
            h = (col.g - col.b) / subv + 0.0;
        }
        else if (col.g == maxv) {
            h = (col.b - col.r) / subv + 2.0;
        }
        else if (col.b == maxv) {
            h = (col.r - col.g) / subv + 4.0;
        }
        h *= SP_PI / 3.0;
        if (h < 0.0) {
            h += 2 * SP_PI;
        }

        hsv = Vec3(h, s, v);
    }


    SP_GENFUNC void cnvXYZToLab(Vec3 &lab, const Vec3 &xyz) {

        const Vec3 w = Vec3(0.95047, 1.00000, 1.0883); // D65

        auto f = [](const double v)-> SP_REAL {
            return static_cast<SP_REAL>((v > 0.008856) ? pow(v, 1.0 / 3.0) : (7.787 * v) + (16.0 / 116.0));
        };

        Vec3 val;
        val.x = f(xyz.x / w.x);
        val.y = f(xyz.y / w.y);
        val.z = f(xyz.z / w.z);

        const double l = (116.0 * val.y) - 16.0;
        const double a = 500.0 * (val.x - val.y);
        const double b = 200.0 * (val.y - val.z);

        lab = Vec3(l, a, b);
    }


    SP_GENFUNC void cnvLabToXYZ(Vec3 &xyz, const Vec3 &lab) {

        const Vec3 w = Vec3(0.95047, 1.00000, 1.0883); // D65

        auto f = [](const SP_REAL v)-> SP_REAL {
            return static_cast<SP_REAL>((v > 0.206897) ? pow(v, 3.0) : 0.001107 * (116.0 * v - 16.0));
        };

        Vec3 val;
        val.y = static_cast<SP_REAL>((lab.x + 16.0) / 116.0);
        val.x = static_cast<SP_REAL>(val.y + lab.y / 500.0);
        val.z = static_cast<SP_REAL>(val.y - lab.z / 200.0);

        xyz.x = f(val.x) * w.x;
        xyz.y = f(val.y) * w.y;
        xyz.z = f(val.z) * w.z;
    }

    SP_GENFUNC void cnvColToXYZ(Vec3 &xyz, const Col3 &col) {
        auto f = [](const double v)-> SP_REAL {
            return static_cast<SP_REAL>((v > 0.040450) ? pow((v + 0.055) / 1.055, 2.4) : v / 12.92);
        };

        Vec3 val;
        val.x = f(col.r / 255.0);
        val.y = f(col.g / 255.0);
        val.z = f(col.b / 255.0);

        // D65
        xyz.x = static_cast<SP_REAL>(+0.412391 * val.x + 0.357584 * val.y + 0.180481 * val.z);
        xyz.y = static_cast<SP_REAL>(+0.212639 * val.x + 0.715169 * val.y + 0.072192 * val.z);
        xyz.z = static_cast<SP_REAL>(+0.019331 * val.x + 0.119195 * val.y + 0.950532 * val.z);
    }

    SP_GENFUNC void cnvXYZToCol(Col3 &col, const Vec3 &xyz) {
        auto f = [](const double v)-> SP_REAL {
            return (v > 0.0031308) ? 1.055 * pow(v, 1.0 / 2.4) - 0.055 : 12.92 * v;
        };

        Vec3 val;

        // D65
        val.x = static_cast<SP_REAL>(+3.240970 * xyz.x - 1.537383 * xyz.y - 0.498611 * xyz.z);
        val.y = static_cast<SP_REAL>(-0.969244 * xyz.x + 1.875968 * xyz.y + 0.041555 * xyz.z);
        val.z = static_cast<SP_REAL>(+0.055630 * xyz.x - 0.203977 * xyz.y + 1.056972 * xyz.z);

        val.x = min(1.0, f(val.x));
        val.y = min(1.0, f(val.y));
        val.z = min(1.0, f(val.z));

        col = cast<Col3>(val);
    }

    SP_GENFUNC void cnvColToLab(Vec3 &lab, const Col3 &col) {
        Vec3 xyz;
        cnvColToXYZ(xyz, col);
        cnvXYZToLab(lab, xyz);
    }

    SP_GENFUNC void cnvLabToCol(Col3 &col, const Vec3 &lab) {
        Vec3 xyz;
        cnvLabToXYZ(xyz, lab);
        cnvXYZToCol(col, xyz);
    }

    //--------------------------------------------------------------------------------
    // color util
    //--------------------------------------------------------------------------------
        // color id
    SP_GENFUNC Col3 getCol3FromId(const int id) {
        Col3 col = getCol3(0, 0, 0);
        col.r = (id) % 256;
        col.g = (id / (256)) % 256;
        col.b = (id / (256 * 256)) % 256;
        return col;
    }

    // color id
    SP_GENFUNC int getIdFromCol3(const Col3& col) {
        int id = 0;
        id += col.r;
        id += col.g * (256);
        id += col.b * (256 * 256);
        if (id == 256 * 256 * 256 - 1) id = -1;
        return id;
    }
    SP_GENFUNC u08 blendCol(const u08 col0, const double r0, const u08 col1, const double r1) {
        if (r0 + r1 == 0.0f) return 0;
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        u08 col;
        col = static_cast<u08>((col0 * r0 + col1 * r1) / (r0 + r1) + 0.5);
        return col;
    }

    SP_GENFUNC SP_REAL blendCol(const SP_REAL col0, const double r0, const SP_REAL col1, const double r1) {
        if (r0 + r1 == 0.0f) return 0.0f;
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        SP_REAL col;
        col = static_cast<SP_REAL>((col0 * r0 + col1 * r1) / (r0 + r1));
        return col;
    }

    SP_GENFUNC Col3 blendCol(const Col3 &col0, const double r0, const Col3 &col1, const double r1) {
        if (r0 + r1 == 0.0f) return getCol3(0, 0, 0);
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        Col3 col;
        col.r = static_cast<u08>((col0.r * r0 + col1.r * r1) / (r0 + r1) + 0.5);
        col.g = static_cast<u08>((col0.g * r0 + col1.g * r1) / (r0 + r1) + 0.5);
        col.b = static_cast<u08>((col0.b * r0 + col1.b * r1) / (r0 + r1) + 0.5);
        return col;
    }

    SP_GENFUNC Col4 blendCol(const Col4 &col0, const double r0, const Col4 &col1, const double r1) {
        if (r0 + r1 == 0.0) return getCol4(0, 0, 0, 0);
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        Col4 dst;
        const float t0 = col0.a * r0;
        const float t1 = col1.a * r1;

        if (t0 + t1 > 0.0f) {
            dst.r = static_cast<u08>((col0.r * t0 + col1.r * t1) / (t0 + t1) + 0.5);
            dst.g = static_cast<u08>((col0.g * t0 + col1.g * t1) / (t0 + t1) + 0.5);
            dst.b = static_cast<u08>((col0.b * t0 + col1.b * t1) / (t0 + t1) + 0.5);
            dst.a = static_cast<u08>((t0 + t1) / (r0 + r1) + 0.5);
        }
        else {
            dst.r = static_cast<u08>((col0.r * r0 + col1.r * r1) / (r0 + r1) + 0.5);
            dst.g = static_cast<u08>((col0.g * r0 + col1.g * r1) / (r0 + r1) + 0.5);
            dst.b = static_cast<u08>((col0.b * r0 + col1.b * r1) / (r0 + r1) + 0.5);
            dst.a = static_cast<u08>(0.0);
        }
        return dst;
    }

    SP_GENFUNC Col3f blendCol(const Col3f &col0, const double r0, const Col3f &col1, const double r1) {
        if (r0 + r1 == 0.0) return getCol4f(0.0, 0.0, 0.0, 0.0);
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        Col4f dst;
        dst.r = (col0.r * r0 + col1.r * r1) / (r0 + r1);
        dst.g = (col0.g * r0 + col1.g * r1) / (r0 + r1);
        dst.b = (col0.b * r0 + col1.b * r1) / (r0 + r1);
        return dst;
    }

    SP_GENFUNC Col4f blendCol(const Col4f &col0, const double r0, const Col4f &col1, const double r1) {
        if (r0 + r1 == 0.0) return getCol4f(0.0, 0.0, 0.0, 0.0);
        if (r0 == 0.0) return col1;
        if (r1 == 0.0) return col0;

        Col4f dst;
        const float t0 = col0.a * r0;
        const float t1 = col1.a * r1;

        if (t0 + t1 > 0.0f) {
            dst.r = (col0.r * t0 + col1.r * t1) / (t0 + t1);
            dst.g = (col0.g * t0 + col1.g * t1) / (t0 + t1);
            dst.b = (col0.b * t0 + col1.b * t1) / (t0 + t1);
            dst.a = (t0 + t1) / (r0 + r1);
        }
        else {
            dst.r = (col0.r * r0 + col1.r * r1) / (r0 + r1);
            dst.g = (col0.g * r0 + col1.g * r1) / (r0 + r1);
            dst.b = (col0.b * r0 + col1.b * r1) / (r0 + r1);
            dst.a = 0.0f;
        }
        return dst;
    }
    SP_GENFUNC Col3 revCol(const Col3& col) {
        Vec3 hsv;
        cnvColToHSV(hsv, col);
        hsv.z = static_cast<SP_REAL>((hsv.z > 0.5) ? hsv.z - 0.5 : hsv.z + 0.5);

        Col3 tmp;
        cnvHSVToCol(tmp, hsv);

        return tmp;
    }

    SP_GENFUNC Col4 revCol(const Col4& col) {
        Col3 c3 = revCol(getCol3(col.r, col.g, col.b));
        return getCol4(c3.r, c3.g, c3.b, col.a);
    }
    // standord color i (0-12) v (0-7)
    SP_GENFUNC Col3 stdcol(const int i, const int v) {
        Col3 col;
        if (i == 0) {
            const float vlist[] = { 1.00f, 0.90f, 0.80f, 0.70f, 0.60f, 0.50f, 0.40f, 0.30f };
            cnvHSVToCol(col, sp::Vec3(0.0f, 0.0f, vlist[v]));
        }
        else {
            const float h = (i - 1) * 2.0f * SP_PI / 12.0f;
            const float slist[] = { 0.16f, 0.26f, 0.38f, 0.50f, 0.62f, 0.74f, 0.86f, 0.98f };
            const float vlist[] = { 0.98f, 0.95f, 0.91f, 0.86f, 0.80f, 0.73f, 0.65f, 0.56f };
            cnvHSVToCol(col, sp::Vec3(h, slist[v], vlist[v]));
        }
        return col;
    }

}

//--------------------------------------------------------------------------------
// camera parameter
//--------------------------------------------------------------------------------

namespace sp {

    SP_GENFUNC CamParam getCamParam(const int dsize0, const int dsize1, const double fx, const double fy, const double cx, const double cy) {
        CamParam dst;
        dst.type = CamParam_Pers;
        dst.dsize[0] = dsize0;
        dst.dsize[1] = dsize1;

        dst.fx = static_cast<SP_REAL>(fx);
        dst.fy = static_cast<SP_REAL>(fy);
        dst.cx = static_cast<SP_REAL>(cx);
        dst.cy = static_cast<SP_REAL>(cy);

        dst.k1 = static_cast<SP_REAL>(0.0);
        dst.k2 = static_cast<SP_REAL>(0.0);
        dst.k3 = static_cast<SP_REAL>(0.0);
        dst.k4 = static_cast<SP_REAL>(0.0);
        dst.p1 = static_cast<SP_REAL>(0.0);
        dst.p2 = static_cast<SP_REAL>(0.0);
        return dst;
    }

    SP_GENFUNC CamParam getCamParam(const int dsize0, const int dsize1, const double fx, const double fy) {
        return getCamParam(dsize0, dsize1, fx, fy, (dsize0 - 1) * 0.5, (dsize1 - 1) * 0.5);
    }

    SP_GENFUNC CamParam getCamParam(const int dsize0, const int dsize1) {
        // groundless camera parameter, but in many cases empirically, no big difference
        const SP_REAL f = 0.8 * (dsize0 + dsize1);
        return getCamParam(dsize0, dsize1, f, f);
    }

    SP_GENFUNC CamParam getCamParam(const int *dsize, const double fx, const double fy, const double cx, const double cy) {
        return getCamParam(dsize[0], dsize[1], fx, fy, cx, cy);
    }

    SP_GENFUNC CamParam getCamParam(const int *dsize, const double fx, const double fy) {
        return getCamParam(dsize[0], dsize[1], fx, fy);
    }

    SP_GENFUNC CamParam getCamParam(const int *dsize) {
        return getCamParam(dsize[0], dsize[1]);
    }

    SP_GENFUNC void getMat(SP_REAL *dst, const int rows, const int cols, const CamParam &cam) {
        dst[0 * cols + 0] = cam.fx;
        dst[0 * cols + 1] = 0.0;
        dst[0 * cols + 2] = cam.cx;

        dst[1 * cols + 0] = 0.0;
        dst[1 * cols + 1] = cam.fy;
        dst[1 * cols + 2] = cam.cy;

        dst[2 * cols + 0] = 0.0;
        dst[2 * cols + 1] = 0.0;
        dst[2 * cols + 2] = 1.0;
    }


    //--------------------------------------------------------------------------------
    // jacob
    //--------------------------------------------------------------------------------

    SP_GENFUNC void jacobCamToPix(SP_REAL *jacob, const CamParam &cam, const Vec2 &npx) {
        const double x2 = npx.x * npx.x;
        const double y2 = npx.y * npx.y;
        const double xy = npx.x * npx.y;

        const double r2 = x2 + y2;
        const double r4 = r2 * r2;
        const double r6 = r4 * r2;

        const double k = 1.0 + cam.k1 * r2 + cam.k2 * r4 + cam.k3 * r6;

        Vec2 dist;
        dist.x = static_cast<SP_REAL>(npx.x * k + cam.p1 * (2.0 * xy) + cam.p2 * (2.0 * x2 + r2));
        dist.y = static_cast<SP_REAL>(npx.y * k + cam.p1 * (2.0 * y2 + r2) + cam.p2 * (2.0 * xy));

        jacob[0 * 9 + 0] = static_cast<SP_REAL>(dist.x);
        jacob[0 * 9 + 1] = static_cast<SP_REAL>(0.0);
        jacob[0 * 9 + 2] = static_cast<SP_REAL>(1.0);
        jacob[0 * 9 + 3] = static_cast<SP_REAL>(0.0);
        jacob[0 * 9 + 4] = static_cast<SP_REAL>(cam.fx * (npx.x * r2));
        jacob[0 * 9 + 5] = static_cast<SP_REAL>(cam.fx * (npx.x * r4));
        jacob[0 * 9 + 6] = static_cast<SP_REAL>(cam.fx * (npx.x * r6));
        jacob[0 * 9 + 7] = static_cast<SP_REAL>(cam.fx * (2.0 * xy));
        jacob[0 * 9 + 8] = static_cast<SP_REAL>(cam.fx * (2.0 * x2 + r2));

        jacob[1 * 9 + 0] = static_cast<SP_REAL>(0.0);
        jacob[1 * 9 + 1] = static_cast<SP_REAL>(dist.y);
        jacob[1 * 9 + 2] = static_cast<SP_REAL>(0.0);
        jacob[1 * 9 + 3] = static_cast<SP_REAL>(1.0);
        jacob[1 * 9 + 4] = static_cast<SP_REAL>(cam.fy * (npx.y * r2));
        jacob[1 * 9 + 5] = static_cast<SP_REAL>(cam.fy * (npx.y * r4));
        jacob[1 * 9 + 6] = static_cast<SP_REAL>(cam.fy * (npx.y * r6));
        jacob[1 * 9 + 7] = static_cast<SP_REAL>(cam.fy * (2.0 * y2 + r2));
        jacob[1 * 9 + 8] = static_cast<SP_REAL>(cam.fy * (2.0 * xy));
    }

    SP_GENFUNC void jacobNpxToDist(SP_REAL *jacob, const CamParam &cam, const Vec2 &npx) {
        const double x2 = npx.x * npx.x;
        const double y2 = npx.y * npx.y;
        const double xy = npx.x * npx.y;

        const double r2 = x2 + y2;
        const double r4 = r2 * r2;
        const double r6 = r4 * r2;

        const double k1 = 1.0 + cam.k1 * r2 + cam.k2 * r4 + cam.k3 * r6;
        const double k2 = 2.0 * cam.k1 + 4.0 * cam.k2 * r2 + 6.0 * cam.k3 * r4;

        jacob[0 * 2 + 0] = static_cast<SP_REAL>(x2 * k2 + k1 + 2.0 * cam.p1 * npx.y + 6.0 * cam.p2 * npx.x);
        jacob[1 * 2 + 1] = static_cast<SP_REAL>(y2 * k2 + k1 + 6.0 * cam.p1 * npx.y + 2.0 * cam.p2 * npx.x);

        jacob[0 * 2 + 1] = static_cast<SP_REAL>(xy * k2 + 2.0 * cam.p1 * npx.x + 2.0 * cam.p2 * npx.y);
        jacob[1 * 2 + 0] = static_cast<SP_REAL>(xy * k2 + 2.0 * cam.p1 * npx.x + 2.0 * cam.p2 * npx.y);
    }

    SP_GENFUNC void jacobNpxToPix(SP_REAL *jacob, const CamParam &cam, const Vec2 &npx) {

        SP_REAL jNpxToDist[2 * 2] = { 0 };
        jacobNpxToDist(jNpxToDist, cam, npx);

        SP_REAL jDistToPix[2 * 2] = { 0 };
        jDistToPix[0 * 2 + 0] = cam.fx;
        jDistToPix[1 * 2 + 1] = cam.fy;

        mulMat(jacob, 2, 2, jDistToPix, 2, 2, jNpxToDist, 2, 2);
    }


    //--------------------------------------------------------------------------------
    // camera util
    //--------------------------------------------------------------------------------

    SP_GENFUNC Vec2 mulCam(const CamParam &cam, const Vec2 &npx) {
        Vec2 pix;
        pix.x = npx.x * cam.fx + cam.cx;
        pix.y = npx.y * cam.fy + cam.cy;
        return pix;
    }

    SP_GENFUNC Line2 mulCam(const CamParam &cam, const Line2 &npxline) {
        Line2 pixline;
        pixline.pos[0] = mulCam(cam, npxline.pos[0]);
        pixline.pos[1] = mulCam(cam, npxline.pos[1]);
        return pixline;
    }

    SP_GENFUNC Vec2 invCam(const CamParam &cam, const Vec2 &pix) {
        Vec2 npx;
        npx.x = (pix.x - cam.cx) / cam.fx;
        npx.y = (pix.y - cam.cy) / cam.fy;
        return npx;
    }

    SP_GENFUNC Line2 invCam(const CamParam &cam, const Line2 &pixline) {
        Line2 npxline;
        npxline.pos[0] = invCam(cam, pixline.pos[0]);
        npxline.pos[1] = invCam(cam, pixline.pos[1]);
        return npxline;
    }

    // distiortion
    SP_GENFUNC Vec2 npxDist(const CamParam &cam, const Vec2 &npx) {
        const double x2 = npx.x * npx.x;
        const double y2 = npx.y * npx.y;
        const double xy = npx.x * npx.y;

        const double r2 = x2 + y2;
        const double r4 = r2 * r2;
        const double r6 = r4 * r2;

        const double k = 1.0 + cam.k1 * r2 + cam.k2 * r4 + cam.k3 * r6;

        Vec2 dist;
        dist.x = static_cast<SP_REAL>(npx.x * k + cam.p1 * (2.0 * xy) + cam.p2 * (2.0 * x2 + r2));
        dist.y = static_cast<SP_REAL>(npx.y * k + cam.p1 * (2.0 * y2 + r2) + cam.p2 * (2.0 * xy));

        return dist;
    }

    // distiortion
    SP_GENFUNC Vec2 pixDist(const CamParam &cam, const Vec2 &pix) {
        return mulCam(cam, npxDist(cam, invCam(cam, pix)));
    }

    // undistortion
    SP_GENFUNC Vec2 npxUndist(const CamParam &cam, const Vec2 &npx) {
        const int maxit = 10;

        Vec2 undist = npx;
        for (int it = 0; it < maxit; it++) {
            const Vec2 err = npx - npxDist(cam, undist);
            if (err.length() < SP_SMALL) break;

            SP_REAL J[2 * 2], inv[2 * 2];
            jacobNpxToDist(J, cam, undist);

            if (invMat22(inv, J) == false) break;

            undist += mulMat(inv, 2, 2, err);
        }

        return undist;
    }

    // undistortion
    SP_GENFUNC Vec2 pixUndist(const CamParam &cam, const Vec2 &pix) {
        return mulCam(cam, npxUndist(cam, invCam(cam, pix)));
    }

    // ideal to pix
    SP_GENFUNC Vec2 mulCamD(const CamParam &cam, const Vec2 &npx) {
        return mulCam(cam, npxDist(cam, npx));
    }

    // pix to ideal
    SP_GENFUNC Vec2 invCamD(const CamParam &cam, const Vec2 &pix) {
        return npxUndist(cam, invCam(cam, pix));
    }

}


//--------------------------------------------------------------------------------
// transform
//--------------------------------------------------------------------------------

namespace sp {

    //--------------------------------------------------------------------------------
    // rotation
    //--------------------------------------------------------------------------------

    SP_GENFUNC Rot nrmRot(const Rot &rot) {
        Rot dst;

        const double div = sqrt(rot.qx * rot.qx + rot.qy * rot.qy + rot.qz * rot.qz + rot.qw * rot.qw);
        if (div > SP_SMALL) {
            const double s = (sign(rot.qw) >= 0.0) ? +1 : -1;

            dst.qx = rot.qx / div * s;
            dst.qy = rot.qy / div * s;
            dst.qz = rot.qz / div * s;
            dst.qw = rot.qw / div * s;
        }
        else {
            dst.qx = 0.0;
            dst.qy = 0.0;
            dst.qz = 0.0;
            dst.qw = 1.0;
        }
        return dst;
    }

    SP_GENFUNC Rot getRot(const SP_REAL qx, const SP_REAL qy, const SP_REAL qz, const SP_REAL qw) {
        Rot dst;
        dst.qx = qx;
        dst.qy = qy;
        dst.qz = qz;
        dst.qw = qw;
        return nrmRot(dst);
    }

    SP_GENFUNC Rot getRot(const SP_REAL *mat, const int rows, const int cols) {
        Rot dst;
        dst.qx = sqrt(max(0.0, 1 + mat[0 * cols + 0] - mat[1 * cols + 1] - mat[2 * cols + 2])) / 2;
        dst.qy = sqrt(max(0.0, 1 - mat[0 * cols + 0] + mat[1 * cols + 1] - mat[2 * cols + 2])) / 2;
        dst.qz = sqrt(max(0.0, 1 - mat[0 * cols + 0] - mat[1 * cols + 1] + mat[2 * cols + 2])) / 2;
        dst.qw = sqrt(max(0.0, 1 + mat[0 * cols + 0] + mat[1 * cols + 1] + mat[2 * cols + 2])) / 2;

        dst.qx *= sign(dst.qx * (mat[2 * cols + 1] - mat[1 * cols + 2]));
        dst.qy *= sign(dst.qy * (mat[0 * cols + 2] - mat[2 * cols + 0]));
        dst.qz *= sign(dst.qz * (mat[1 * cols + 0] - mat[0 * cols + 1]));

        return nrmRot(dst);
    }

    SP_GENFUNC void getMat(SP_REAL *dst, const int rows, const int cols, const Rot &rot) {
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

    SP_GENFUNC Rot zeroRot() {
        return getRot(0.0, 0.0, 0.0, 1.0);
    }

    SP_GENFUNC Rot invRot(const Rot &rot) {
        return getRot(-rot.qx, -rot.qy, -rot.qz, rot.qw);
    }

    //--------------------------------------------------------------------------------
    // rotation operator
    //--------------------------------------------------------------------------------

    SP_GENFUNC Rot mulRot(const Rot &rot0, const Rot &rot1) {
        Rot dst;
        dst.qx = static_cast<SP_REAL>((rot0.qw * rot1.qx) + (rot0.qx * rot1.qw) + (rot0.qy * rot1.qz) - (rot0.qz * rot1.qy));
        dst.qy = static_cast<SP_REAL>((rot0.qw * rot1.qy) + (rot0.qy * rot1.qw) + (rot0.qz * rot1.qx) - (rot0.qx * rot1.qz));
        dst.qz = static_cast<SP_REAL>((rot0.qw * rot1.qz) + (rot0.qz * rot1.qw) + (rot0.qx * rot1.qy) - (rot0.qy * rot1.qx));

        dst.qw = static_cast<SP_REAL>((rot0.qw * rot1.qw) - (rot0.qx * rot1.qx) - (rot0.qy * rot1.qy) - (rot0.qz * rot1.qz));

        return nrmRot(dst);
    }

    SP_GENFUNC Vec3 mulRot(const Rot &rot, const Vec3 &vec) {
        SP_REAL rotMat[3 * 3];
        getMat(rotMat, 3, 3, rot);

        return mulMat(rotMat, 3, 3, vec);
    }

    SP_GENFUNC Vec3 mulRot(const Rot &rot, const Vec2 &vec) {
        return mulRot(rot, Vec3(vec.x, vec.y, 0.0));
    }

    SP_GENFUNC Vec3 operator * (const Rot &rot, const Vec3 &vec) { return mulRot(rot, vec); }
    SP_GENFUNC Vec3 operator * (const Rot &rot, const Vec2 &vec) { return mulRot(rot, vec); }

    SP_GENFUNC Rot operator * (const Rot &rot0, const Rot &rot1) { return mulRot(rot0, rot1); }
    SP_GENFUNC void operator *= (Rot &rot0, const Rot &rot1) { rot0 = mulRot(rot0, rot1); }

    //--------------------------------------------------------------------------------
    // rotation util
    //--------------------------------------------------------------------------------

    SP_GENFUNC void getMatAngleX(SP_REAL *dst, const int rows, const int cols, const SP_REAL angle) {
        dst[0 * cols + 0] = 1.0;
        dst[0 * cols + 1] = 0.0;
        dst[0 * cols + 2] = 0.0;

        dst[1 * cols + 0] = 0.0;
        dst[1 * cols + 1] = +cos(angle);
        dst[1 * cols + 2] = -sin(angle);

        dst[2 * cols + 0] = 0.0;
        dst[2 * cols + 1] = +sin(angle);
        dst[2 * cols + 2] = +cos(angle);
    }

    SP_GENFUNC void getMatAngleY(SP_REAL *dst, const int rows, const int cols, const SP_REAL angle) {
        dst[0 * cols + 0] = +cos(angle);
        dst[0 * cols + 1] = 0.0;
        dst[0 * cols + 2] = +sin(angle);

        dst[1 * cols + 0] = 0.0;
        dst[1 * cols + 1] = 1.0;
        dst[1 * cols + 2] = 0.0;

        dst[2 * cols + 0] = -sin(angle);
        dst[2 * cols + 1] = 0.0;
        dst[2 * cols + 2] = +cos(angle);
    }

    SP_GENFUNC void getMatAngleZ(SP_REAL *dst, const int rows, const int cols, const SP_REAL angle) {
        dst[0 * cols + 0] = +cos(angle);
        dst[0 * cols + 1] = -sin(angle);
        dst[0 * cols + 2] = 0.0;

        dst[1 * cols + 0] = +sin(angle);
        dst[1 * cols + 1] = +cos(angle);
        dst[1 * cols + 2] = 0.0;

        dst[2 * cols + 0] = 0.0;
        dst[2 * cols + 1] = 0.0;
        dst[2 * cols + 2] = 1.0;
    }

    SP_GENFUNC void getMatRodrigues(SP_REAL *dst, const int rows, const int cols, const Vec3 &vec) {
        const SP_REAL angle = vec.length();
        const Vec3 nrm = vec.unit();

        const SP_REAL c = cos(angle);
        const SP_REAL s = sin(angle);

        dst[0 * 3 + 0] = static_cast<SP_REAL>(nrm.x * nrm.x * (1.0 - c) + c);
        dst[0 * 3 + 1] = static_cast<SP_REAL>(nrm.x * nrm.y * (1.0 - c) - nrm.z * s);
        dst[0 * 3 + 2] = static_cast<SP_REAL>(nrm.x * nrm.z * (1.0 - c) + nrm.y * s);

        dst[1 * 3 + 0] = static_cast<SP_REAL>(nrm.y * nrm.x * (1.0 - c) + nrm.z * s);
        dst[1 * 3 + 1] = static_cast<SP_REAL>(nrm.y * nrm.y * (1.0 - c) + c);
        dst[1 * 3 + 2] = static_cast<SP_REAL>(nrm.y * nrm.z * (1.0 - c) - nrm.x * s);

        dst[2 * 3 + 0] = static_cast<SP_REAL>(nrm.z * nrm.x * (1.0 - c) - nrm.y * s);
        dst[2 * 3 + 1] = static_cast<SP_REAL>(nrm.z * nrm.y * (1.0 - c) + nrm.x * s);
        dst[2 * 3 + 2] = static_cast<SP_REAL>(nrm.z * nrm.z * (1.0 - c) + c);
    }

    SP_GENFUNC void getMatRodrigues(SP_REAL *dst, const int rows, const int cols, const Vec3 &vec, const SP_REAL angle) {
        getMatRodrigues(dst, rows, cols, vec.unit() * angle);
    }

    SP_GENFUNC Rot getRotAxis(const Vec3 &x, const Vec3 &y, const Vec3 &z) {
        const Vec3 nx = x.unit();
        const Vec3 ny = y.unit();
        const Vec3 nz = z.unit();
        SP_REAL mat[3 * 3];
        mat[0 * 3 + 0] = nx.x; mat[0 * 3 + 1] = ny.x; mat[0 * 3 + 2] = nz.x;
        mat[1 * 3 + 0] = nx.y; mat[1 * 3 + 1] = ny.y; mat[1 * 3 + 2] = nz.y;
        mat[2 * 3 + 0] = nx.z; mat[2 * 3 + 1] = ny.z; mat[2 * 3 + 2] = nz.z;

        return getRot(mat, 3, 3);
    }

    SP_GENFUNC Rot getRotAngle(const Vec3 &vec) {
        const SP_REAL angle = vec.length();
        if (angle > SP_SMALL) {
            const Vec3 nrm = vec.unit();

            const SP_REAL s = sin(angle * 0.5);
            const SP_REAL c = cos(angle * 0.5);
            return getRot(s * nrm.x, s * nrm.y, s * nrm.z, c);
        }
        else {
            return zeroRot();
        }
    }

    SP_GENFUNC Rot getRotAngle(const Vec3 &vec, const double angle) {
        return getRotAngle(vec.unit() * angle);
    }

    SP_GENFUNC Rot getRotAngleX(const double angle) {
        return getRotAngle(Vec3(1.0, 0.0, 0.0), angle);
    }

    SP_GENFUNC Rot getRotAngleY(const double angle) {
        return getRotAngle(Vec3(0.0, 1.0, 0.0), angle);
    }

    SP_GENFUNC Rot getRotAngleZ(const double angle) {
        return getRotAngle(Vec3(0.0, 0.0, 1.0), angle);
    }

    SP_GENFUNC Vec3 getAngle(const Rot &rot) {
        Vec3 vec = Vec3(0.0, 0.0, 0.0);

        if (cmp(rot, getRot(0.0, 0.0, 0.0, 1.0)) == false) {
            const SP_REAL angle = acos(rot.qw) * 2.0;

            if (cmp(angle, 0.0) == false) {
                const SP_REAL s = sin(angle * 0.5);
                vec.x = rot.qx / s * angle;
                vec.y = rot.qy / s * angle;
                vec.z = rot.qz / s * angle;
            }
        }
        return vec;
    }

    SP_GENFUNC Rot getRotDirection(const Vec3 &vec) {
        const Vec3 nrm = vec.unit();

        if (fabs(nrm.z) == 1.0) {
            const SP_REAL angle = (nrm.z > 0) ? 0.0 : SP_PI;
            return getRotAngleX(angle);
        }
        else {
            const Vec3 v0 = Vec3(0.0, 1.0, 0.0).cross(Vec3(nrm.x, nrm.y, 0.0));
            const SP_REAL a0 = acos(nrm.y / sqrt(nrm.x * nrm.x + nrm.y * nrm.y));
            const Rot rot0 = getRotAngle(v0, a0);

            const Vec3 v1 = Vec3(0.0, 0.0, 1.0).cross(nrm);
            const SP_REAL a1 = acos(nrm.z);
            const Rot rot1 = getRotAngle(v1, a1);

            return invRot(rot1 * rot0);
        }
    }

    // zyx eulter
    SP_GENFUNC Rot getRotEuler(const Vec3 &euler) {
        const Rot rotx = getRotAngleX(euler.x);
        const Rot roty = getRotAngleY(euler.y);
        const Rot rotz = getRotAngleZ(euler.z);
        return rotz * roty * rotx;
    }

    // zyx eulter
    SP_GENFUNC Vec3 getEuler(const SP_REAL *mat, const int rows, const int cols) {

        Vec3 euler;
        euler.y = asin(-mat[2 * 3 + 0]);

        if (fabs(euler.y) < SP_PI / 2.0) {
            euler.z = atan2(mat[1 * 3 + 0], mat[0 * 3 + 0]);
            euler.x = atan2(mat[2 * 3 + 1], mat[2 * 3 + 2]);
        }
        else {
            euler.z = atan2(-mat[1 * 3 + 2], mat[0 * 3 + 2]);
            euler.x = 0.0;
        }

        return euler;
    }

    // zyx eulter
    SP_GENFUNC Vec3 getEuler(const Rot &rot) {
        SP_REAL mat[3 * 3];
        getMat(mat, 3, 3, rot);

        return getEuler(mat, 3, 3);
    }


    // update
    SP_GENFUNC Rot updateRot(const Rot &rot, const SP_REAL *delta) {
        return getRotAngle(Vec3(delta[0], delta[1], delta[2])) * rot;
    }

    // angle
    SP_GENFUNC SP_REAL getAngle(const Rot &rot, const int axis) {
        SP_ASSERT(axis >= 0 && axis < 3);

        const Vec3 v0 = Vec3(axis == 0 ? 1.0 : 0.0, axis == 1 ? 1.0 : 0.0, axis == 2 ? 1.0 : 0.0);
        const Vec3 v1 = rot * v0;
        const SP_REAL angle = acos(v0.dot(v1));
        return angle;
    }

    // angle
    SP_GENFUNC SP_REAL getAngle(const Vec2 &vec0, const Vec2 &vec1) {
        double ret = 0.0;
        const double a = vec0.length();
        const double b = vec1.length();
        if (a > SP_SMALL && b > SP_SMALL) {
            ret = acos(vec0.dot(vec1) / (a * b));
        }
        return static_cast<SP_REAL>(ret);
    }

    // angle
    SP_GENFUNC SP_REAL getAngle(const Vec3 &vec0, const Vec3 &vec1) {
        double ret = 0.0;
        const double a = vec0.length();
        const double b = vec1.length();
        if (a > SP_SMALL && b > SP_SMALL) {
            ret = acos(vec0.dot(vec1) / (a * b));
        }
        return static_cast<SP_REAL>(ret);
    }

    // dif
    SP_GENFUNC SP_REAL difRot(const Rot &rot0, const Rot &rot1) {
        return getAngle(rot0 * invRot(rot1)).length();
    }

    // dif
    SP_GENFUNC SP_REAL difRot(const Rot &rot0, const Rot &rot1, const int axis) {
        return getAngle(rot0 * invRot(rot1), axis);
    }


    //--------------------------------------------------------------------------------
    // pose
    //--------------------------------------------------------------------------------

    SP_GENFUNC Pose getPose(const Rot &rot, const Vec3 &trn) {
        Pose dst;
        dst.rot = nrmRot(rot);
        dst.pos = trn;

        return dst;
    }

    SP_GENFUNC Pose getPose(const Rot &rot) {
        return getPose(rot, Vec3(0.0, 0.0, 0.0));
    }

    SP_GENFUNC Pose getPose(const Vec3 &trn) {
        return getPose(zeroRot(), trn);
    }

    SP_GENFUNC Pose getPose(const SP_REAL *mat, const int rows, const int cols) {
        Pose dst;
        if ((rows == 3 || rows == 4) && cols == 4) {
            dst.rot = getRot(mat, rows, cols);
            dst.pos = Vec3(mat[0 * cols + 3], mat[1 * cols + 3], mat[2 * cols + 3]);
        }
        if ((rows == 6 && cols == 1) || (rows == 1 && cols == 6)) {
            Vec3 euler = Vec3(mat[0], mat[1], mat[2]);
            dst.rot = getRotEuler(euler);
            dst.pos = Vec3(mat[3], mat[4], mat[5]);
        }
        return dst;
    }

    SP_GENFUNC void getMat(SP_REAL *dst, const int rows, const int cols, const Pose &pose) {
        if ((rows == 3 || rows == 4) && cols == 4) {
            eyeMat(dst, rows, cols);
            getMat(dst, rows, cols, pose.rot);

            dst[0 * cols + 3] = pose.pos.x;
            dst[1 * cols + 3] = pose.pos.y;
            dst[2 * cols + 3] = pose.pos.z;
        }
        if ((rows == 6 && cols == 1) || (rows == 1 && cols == 6)) {
            const Vec3 euler = getEuler(pose.rot);
            dst[0] = euler.x;
            dst[1] = euler.y;
            dst[2] = euler.z;
            dst[3] = pose.pos.x;
            dst[4] = pose.pos.y;
            dst[5] = pose.pos.z;
        }
    }

    SP_GENFUNC Pose zeroPose() {
        return getPose(zeroRot(), Vec3(0.0, 0.0, 0.0));
    }

    SP_GENFUNC Pose invPose(const Pose &pose) {
        Pose dst;
        dst.rot = invRot(pose.rot);
        dst.pos = mulRot(dst.rot, pose.pos) * -1.0;

        return dst;
    }

    //--------------------------------------------------------------------------------
    // pose operator
    //--------------------------------------------------------------------------------

    SP_GENFUNC Pose mulPose(const Pose &pose0, const Pose &pose1) {
        Pose dst;
        dst.rot = mulRot(pose0.rot, pose1.rot);
        dst.pos = mulRot(pose0.rot, pose1.pos) + pose0.pos;

        return dst;
    }

    SP_GENFUNC Vec3 mulPose(const Pose &pose, const Vec3 &vec) {
        SP_REAL poseMat[3 * 4];
        getMat(poseMat, 3, 4, pose);

        return mulMat(poseMat, 3, 4, vec);
    }

    SP_GENFUNC Vec3 mulPose(const Pose &pose, const Vec2 &vec) {
        return mulPose(pose, Vec3(vec.x, vec.y, 0.0));
    }

    SP_GENFUNC VecPD3 mulPose(const Pose &pose, const VecPD3 &vec) {
        SP_REAL poseMat[3 * 4];
        getMat(poseMat, 3, 4, pose);

        return mulMat(poseMat, 3, 4, vec);
    }

    SP_GENFUNC Line3 mulPose(const Pose &pose, const Line3 &line) {
        SP_REAL poseMat[3 * 4];
        getMat(poseMat, 3, 4, pose);

        return mulMat(poseMat, 3, 4, line);
    }

    SP_GENFUNC Mesh3 mulPose(const Pose &pose, const Mesh3 &mesh) {
        SP_REAL poseMat[3 * 4];
        getMat(poseMat, 3, 4, pose);

        return mulMat(poseMat, 3, 4, mesh);
    }

    SP_GENFUNC Vec3 operator * (const Pose &pose, const Vec3 &vec) { return mulPose(pose, vec); }
    SP_GENFUNC Vec3 operator * (const Pose &pose, const Vec2 &vec) { return mulPose(pose, vec); }
    
    SP_GENFUNC VecPD3 operator * (const Pose &pose, const VecPD3 &vec) { return mulPose(pose, vec); }
    SP_GENFUNC Line3 operator * (const Pose &pose, const Line3 &line) { return mulPose(pose, line); }
    SP_GENFUNC Mesh3 operator * (const Pose &pose, const Mesh3 &mesh) { return mulPose(pose, mesh); }

    SP_GENFUNC Pose operator * (const Pose &pose0, const Pose &pose1) { return mulPose(pose0, pose1); }
    SP_GENFUNC void operator *= (Pose &pose0, const Pose &pose1) { pose0 = mulPose(pose0, pose1); }

    SP_GENFUNC Pose operator * (const Rot &rot, const Pose &pose) { return mulPose(getPose(rot), pose); }
    SP_GENFUNC Pose operator * (const Pose &pose, const Rot &rot) { return mulPose(pose, getPose(rot)); }
    SP_GENFUNC void operator *= (Pose &pose, const Rot &rot) { pose = mulPose(pose, getPose(rot)); }

    //--------------------------------------------------------------------------------
    // pose util
    //--------------------------------------------------------------------------------

    SP_GENFUNC Pose getGeodesicPose(const int level, const int id, const double distance = 0.0) {
        const Vec3 v = getGeodesicMesh(level, id).center() * (-1.0);
        const Pose pose = getPose(getRotDirection(v), Vec3(0.0, 0.0, distance));
        return pose;
    }

    //--------------------------------------------------------------------------------
    // jacob
    //--------------------------------------------------------------------------------

    SP_GENFUNC void jacobPosToNpx(SP_REAL *jacob, const Vec3 &pos) {
        SP_REAL divz = (pos.z != 0) ? 1.0 / pos.z : 0.0;

        jacob[0 * 3 + 0] = divz; jacob[0 * 3 + 1] = 0.0; jacob[0 * 3 + 2] = -pos.x * divz * divz;
        jacob[1 * 3 + 0] = 0.0; jacob[1 * 3 + 1] = divz; jacob[1 * 3 + 2] = -pos.y * divz * divz;
    }

    SP_GENFUNC void jacobPosToPix(SP_REAL *jacob, const CamParam &cam, const Vec3 &pos) {

        SP_REAL jPosToNpz[2 * 3] = { 0 };
        jacobPosToNpx(jPosToNpz, pos);

        SP_REAL jNpxToPix[2 * 2];
        jacobNpxToPix(jNpxToPix, cam, prjVec(pos));

        mulMat(jacob, 2, 3, jNpxToPix, 2, 2, jPosToNpz, 2, 3);
    }

    SP_GENFUNC void jacobPoseToPos(SP_REAL *jacob, const Pose &pose, const Vec3 &pos) {
        SP_REAL rmat[3 * 3];
        getMat(rmat, 3, 3, pose.rot);
        const Vec3 v = mulMat(rmat, 3, 3, pos);
        jacob[0 * 6 + 0] = +0.0; jacob[0 * 6 + 1] = +v.z; jacob[0 * 6 + 2] = -v.y;
        jacob[1 * 6 + 0] = -v.z; jacob[1 * 6 + 1] = +0.0; jacob[1 * 6 + 2] = +v.x;
        jacob[2 * 6 + 0] = +v.y; jacob[2 * 6 + 1] = -v.x; jacob[2 * 6 + 2] = +0.0;

        jacob[0 * 6 + 3] = 1.0; jacob[0 * 6 + 4] = 0.0; jacob[0 * 6 + 5] = 0.0;
        jacob[1 * 6 + 3] = 0.0; jacob[1 * 6 + 4] = 1.0; jacob[1 * 6 + 5] = 0.0;
        jacob[2 * 6 + 3] = 0.0; jacob[2 * 6 + 4] = 0.0; jacob[2 * 6 + 5] = 1.0;

    }

    SP_GENFUNC void jacobPoseToNpx(SP_REAL *jacob, const Pose &pose, const Vec3 &pos) {
        SP_REAL pmat[3 * 4];
        getMat(pmat, 3, 4, pose);

        SP_REAL jPoseToPos[3 * 6] = { 0 };
        jacobPoseToPos(jPoseToPos, pose, pos);

        SP_REAL jPosToNpx[2 * 3] = { 0 };
        jacobPosToNpx(jPosToNpx, mulMat(pmat, 3, 4, pos));

        mulMat(jacob, 2, 6, jPosToNpx, 2, 3, jPoseToPos, 3, 6);
    }

    SP_GENFUNC void jacobPoseToPix(SP_REAL *jacob, const CamParam &cam, const Pose &pose, const Vec3 &pos) {
        SP_REAL pmat[3 * 4];
        getMat(pmat, 3, 4, pose);

        SP_REAL jPoseToPos[3 * 6] = { 0 };
        jacobPoseToPos(jPoseToPos, pose, pos);

        SP_REAL jPosToPix[2 * 3];
        jacobPosToPix(jPosToPix, cam, mulMat(pmat, 3, 4, pos));

        mulMat(jacob, 2, 6, jPosToPix, 2, 3, jPoseToPos, 3, 6);
    }

}


#endif