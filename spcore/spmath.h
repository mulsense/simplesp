//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_MATH_H__
#define __SP_MATH_H__

#include "spcore/spcom.h"
#include "spcore/spbase.h"

namespace sp {

    //--------------------------------------------------------------------------------
    // function
    //--------------------------------------------------------------------------------

    // gauss function
    SP_GENFUNC SP_REAL funcGauss(const double x, const double mean, const double sigma) {
        SP_REAL ret = 0.0;

        if (fabs(sigma) > 0) {
            ret = static_cast<SP_REAL>(exp(-(x - mean) * (x - mean) / (2 * sigma * sigma)) / (sqrt(2 * SP_PI) * sigma));
        }
        return ret;
    }

    // tukey function
    SP_GENFUNC SP_REAL funcTukey(const double x, const double t) {
        SP_REAL ret = 0.0;

        if (fabs(x) < t) {
            const double v = 1.0 - (x * x) / (t * t);
            ret = static_cast<SP_REAL>(v * v);
        }
        return ret;
    }

}

//--------------------------------------------------------------------------------
// mem
//--------------------------------------------------------------------------------

namespace sp {

    // set
    template <typename DST, typename SRC>
    SP_GENFUNC void setMem(DST *dst, const int size, const SRC *mem0) {
        for (int i = 0; i < size; i++) {
            dst[i] = mem0[i];
        }
    }

    // convert
    template <typename DST, typename SRC>
    SP_GENFUNC void cnvMem(DST *dst, const int size, const SRC *mem0, const SP_REAL scale = 1.0, const SP_REAL base = 0.0) {
        for (int i = 0; i < size; i++) {
            dst[i] = cast<DST>((mem0[i] - base) * scale);
        }
    }

    // set
    template <typename DST, typename ELEM>
    SP_GENFUNC void setElm(DST *dst, const int size, const ELEM &elm) {
        for (int i = 0; i < size; i++) {
            dst[i] = elm;
        }
    }

    //--------------------------------------------------------------------------------
    // mem operator
    //--------------------------------------------------------------------------------

    template <typename DST, typename SRC0, typename SRC1> SP_GENFUNC void addMem(DST *dst, const int size, const SRC0 *mem0, const SRC1 *mem1) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] + mem1[i]; } }
    template <typename DST, typename SRC0, typename SRC1> SP_GENFUNC void subMem(DST *dst, const int size, const SRC0 *mem0, const SRC1 *mem1) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] - mem1[i]; } }
    template <typename DST, typename SRC0, typename SRC1> SP_GENFUNC void mulMem(DST *dst, const int size, const SRC0 *mem0, const SRC1 *mem1) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] * mem1[i]; } }
    template <typename DST, typename SRC0, typename SRC1> SP_GENFUNC void divMem(DST *dst, const int size, const SRC0 *mem0, const SRC1 *mem1) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] / mem1[i]; } }

    template <typename DST, typename SRC, typename ELEM> SP_GENFUNC void addElm(DST *dst, const int size, const SRC *mem0, const ELEM &elm) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] + elm; } }
    template <typename DST, typename SRC, typename ELEM> SP_GENFUNC void subElm(DST *dst, const int size, const SRC *mem0, const ELEM &elm) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] - elm; } }
    template <typename DST, typename SRC, typename ELEM> SP_GENFUNC void mulElm(DST *dst, const int size, const SRC *mem0, const ELEM &elm) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] * elm; } }
    template <typename DST, typename SRC, typename ELEM> SP_GENFUNC void divElm(DST *dst, const int size, const SRC *mem0, const ELEM &elm) { for (int i = 0; i < size; i++) { dst[i] = mem0[i] / elm; } }

    //--------------------------------------------------------------------------------
    // matrix
    //--------------------------------------------------------------------------------

    // identity matrix
    SP_GENFUNC void eyeMat(SP_REAL *dst, const int rows, const int cols) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                dst[r * cols + c] = static_cast<SP_REAL>((r == c) ? 1.0 : 0.0);
            }
        }
    }

    // zero matrix
    SP_GENFUNC void zeroMat(SP_REAL *dst, const int rows, const int cols) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                dst[r * cols + c] = static_cast<SP_REAL>(0.0);
            }
        }
    }

    // extension matrix
    SP_GENFUNC void extMat(SP_REAL *dst, const int rows, const int cols, const SP_REAL *mat0, const int rows0, const int cols0) {
        eyeMat(dst, rows, cols);

        for (int r = 0; r < min(rows, rows0); r++) {
            for (int c = 0; c < min(cols, cols0); c++) {
                dst[r * cols + c] = mat0[r * cols0 + c];
            }
        }
    }

    // multiple
    SP_GENFUNC void mulMat(SP_REAL *dst, const int rows, const int cols, const SP_REAL *mat0, const int rows0, const int cols0, const SP_REAL *mat1, const int rows1, const int cols1) {

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                SP_REAL &d = dst[r * cols + c];
                d = 0;
                for (int i = 0; i < cols0; i++) {
                    d += mat0[r * cols0 + i] * mat1[i * cols1 + c];
                }
            }
        }
    }


    // transpose
    SP_GENFUNC void trnMat(SP_REAL *dst, const int rows, const int cols, const SP_REAL *mat0, const int rows0, const int cols0) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                dst[r * cols + c] = mat0[c * cols0 + r];
            }
        }
    }

    // covariance
    SP_GENFUNC void covMat(SP_REAL *dst, const int rows, const int cols, const SP_REAL *mat0, const int rows0, const int cols0) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                SP_REAL &d = dst[r * cols + c];
                d = 0;
                for (int i = 0; i < rows0; i++) {
                    d += mat0[i * cols0 + r] * mat0[i * cols0 + c];
                }
            }
        }
    }

    // skew
    SP_GENFUNC void skewMat(SP_REAL *dst, const int rows, const int cols, const Vec3 &vec) {
        dst[0 * 3 + 0] = 0.0;
        dst[0 * 3 + 1] = -vec.z;
        dst[0 * 3 + 2] = +vec.y;

        dst[1 * 3 + 0] = +vec.z;
        dst[1 * 3 + 1] = 0.0;
        dst[1 * 3 + 2] = -vec.x;

        dst[2 * 3 + 0] = -vec.y;
        dst[2 * 3 + 1] = +vec.x;
        dst[2 * 3 + 2] = 0.0;
    }

    //--------------------------------------------------------------------------------
    // matrix determinant
    //--------------------------------------------------------------------------------

    SP_GENFUNC SP_REAL detMat22(const SP_REAL *mat) {
        return mat[0 * 2 + 0] * mat[1 * 2 + 1] - mat[0 * 2 + 1] * mat[1 * 2 + 0];
    }

    SP_GENFUNC SP_REAL detMat33(const SP_REAL *mat) {
        const SP_REAL v0 = mat[0 * 3 + 0] * (mat[1 * 3 + 1] * mat[2 * 3 + 2] - mat[2 * 3 + 1] * mat[1 * 3 + 2]);
        const SP_REAL v1 = mat[0 * 3 + 1] * (mat[1 * 3 + 0] * mat[2 * 3 + 2] - mat[2 * 3 + 0] * mat[1 * 3 + 2]);
        const SP_REAL v2 = mat[0 * 3 + 2] * (mat[1 * 3 + 0] * mat[2 * 3 + 1] - mat[2 * 3 + 0] * mat[1 * 3 + 1]);

        return v0 - v1 + v2;
    }

    SP_GENFUNC SP_REAL detMat(const SP_REAL *mat, const int rows, const int cols, SP_REAL *buf) {

        if (rows != cols) return 0.0;
        const int size = rows;

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                buf[r * size + c] = mat[r * size + c];
            }
        }

        SP_REAL dst = 1.0;
        for (int i = 0; i < size; i++) {
            if (i == size - 1) {
                dst *= buf[i * size + i];
                continue;
            }

            // partial pivoting
            {
                int pivot = i;
                SP_REAL max = 0.0;
                for (int r = i; r < size; r++) {
                    const SP_REAL val = fabs(buf[r * size + i]);
                    if (val > max) {
                        max = val;
                        pivot = r;
                    }
                }

                if (pivot > i) {
                    for (int c = 0; c < size; c++) {
                        swap(buf[i * size + c], buf[pivot * size + c]);
                    }
                    dst *= -1.0;
                }
            }

            // div
            {
                const SP_REAL div = buf[i * size + i];
                if (fabs(div) < SP_SMALL) return 0.0;

                dst *= div;
                for (int c = i; c < size; c++) {
                    buf[i * size + c] /= div;
                }

                for (int r = i + 1; r < size; r++) {
                    const SP_REAL scl = buf[r * size + i];
                    for (int c = i + 1; c < size; c++) {
                        buf[r * size + c] -= buf[i * size + c] * scl;
                    }
                }
            }
        }
        return dst;
    }

    //--------------------------------------------------------------------------------
    // matrix inverse
    //--------------------------------------------------------------------------------

    SP_GENFUNC bool invMat22(SP_REAL *dst, const SP_REAL *mat) {
        const SP_REAL det = detMat22(mat);
        if (fabs(det) < SP_SMALL) return false;

        dst[0 * 2 + 0] = (+mat[1 * 2 + 1]) / det;
        dst[0 * 2 + 1] = (-mat[0 * 2 + 1]) / det;
        dst[1 * 2 + 0] = (-mat[1 * 2 + 0]) / det;
        dst[1 * 2 + 1] = (+mat[0 * 2 + 0]) / det;

        return true;
    }

    SP_GENFUNC bool invMat33(SP_REAL *dst, const SP_REAL *mat) {
        const SP_REAL det = detMat33(mat);
        if (fabs(det) < SP_SMALL) return false;

        dst[0 * 3 + 0] = (+(mat[1 * 3 + 1] * mat[2 * 3 + 2] - mat[1 * 3 + 2] * mat[2 * 3 + 1])) / det;
        dst[0 * 3 + 1] = (-(mat[0 * 3 + 1] * mat[2 * 3 + 2] - mat[0 * 3 + 2] * mat[2 * 3 + 1])) / det;
        dst[0 * 3 + 2] = (+(mat[0 * 3 + 1] * mat[1 * 3 + 2] - mat[0 * 3 + 2] * mat[1 * 3 + 1])) / det;

        dst[1 * 3 + 0] = (-(mat[1 * 3 + 0] * mat[2 * 3 + 2] - mat[1 * 3 + 2] * mat[2 * 3 + 0])) / det;
        dst[1 * 3 + 1] = (+(mat[0 * 3 + 0] * mat[2 * 3 + 2] - mat[0 * 3 + 2] * mat[2 * 3 + 0])) / det;
        dst[1 * 3 + 2] = (-(mat[0 * 3 + 0] * mat[1 * 3 + 2] - mat[0 * 3 + 2] * mat[1 * 3 + 0])) / det;

        dst[2 * 3 + 0] = (+(mat[1 * 3 + 0] * mat[2 * 3 + 1] - mat[1 * 3 + 1] * mat[2 * 3 + 0])) / det;
        dst[2 * 3 + 1] = (-(mat[0 * 3 + 0] * mat[2 * 3 + 1] - mat[0 * 3 + 1] * mat[2 * 3 + 0])) / det;
        dst[2 * 3 + 2] = (+(mat[0 * 3 + 0] * mat[1 * 3 + 1] - mat[0 * 3 + 1] * mat[1 * 3 + 0])) / det;

        return true;
    }

    SP_GENFUNC bool invMat(SP_REAL *dst, const SP_REAL *mat, const int rows, const int cols, SP_REAL *buf) {

        if (rows != cols) return false;
        const int size = rows;

        if (size == 1) {
            if (fabs(mat[0]) < SP_SMALL) {
                return false;
            }
            else {
                dst[0] = 1.0 / mat[0];
                return true;
            }
        }

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                dst[r * size + c] = (r == c) ? 1.0 : 0.0;
                buf[r * size + c] = mat[r * size + c];
            }
        }

        // Gauss-Jordan
        for (int i = 0; i < size; i++) {
            // partial pivoting
            {
                int pivot = i;
                SP_REAL max = 0.0;
                for (int r = i; r < size; r++) {
                    const SP_REAL val = fabs(buf[r * size + i]);
                    if (val > max) {
                        max = val;
                        pivot = r;
                    }
                }

                if (pivot > i) {
                    for (int c = 0; c < size; c++) {
                        swap(dst[i * size + c], dst[pivot * size + c]);
                        swap(buf[i * size + c], buf[pivot * size + c]);
                    }
                }
            }

            // div
            {
                const SP_REAL div = buf[i * size + i];
                if (fabs(div) < SP_SMALL) return false;

                for (int c = 0; c < size; c++) {
                    dst[i * size + c] /= div;
                    buf[i * size + c] /= div;
                }

                for (int r = 0; r < size; r++) {
                    if (r == i) continue;

                    const SP_REAL scl = buf[r * size + i];
                    for (int c = 0; c < size; c++) {
                        dst[r * size + c] -= dst[i * size + c] * scl;
                        buf[r * size + c] -= buf[i * size + c] * scl;
                    }
                }
            }
        }
        return true;
    }


    //--------------------------------------------------------------------------------
    // matrix eigen
    //--------------------------------------------------------------------------------

    SP_GENFUNC bool eigMat(SP_REAL *eigVec, SP_REAL *eigVal, const SP_REAL *mat, const int rows, const int cols, const bool minOrder = true) {

        if (rows < 2 || cols < 2 || rows != cols) return false;

        const int size = rows;

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                eigVec[r * size + c] = static_cast<SP_REAL>((r == c) ? 1.0 : 0.0);
                eigVal[r * size + c] = mat[r * size + c];
            }
        }

        const int maxit = size * size;

        // jacobi algorithm
        for (int it = 0; it < maxit; it++) {
            int p = 0, q = 0;
            double maxv = 0.0;

            for (int r = 0; r < size; r++) {
                for (int c = r + 1; c < size; c++) {

                    const double val = fabs(eigVal[r * size + c]);
                    if (val > maxv) {
                        maxv = val;
                        p = r;
                        q = c;
                    }
                }
            }
            if (maxv < SP_SMALL) break;

            const double app = eigVal[p * size + p];
            const double apq = eigVal[p * size + q];
            const double aqq = eigVal[q * size + q];

            double sinv, cosv;
            {
                const double a = (app - aqq) / 2.0;
                const double b = -apq;

                // g = cos(2A) = |a| / sqrt(a * a + b * b)
                const double g = fabs(a) / pythag(a, b);

                sinv = sqrt((1.0 - g) / 2.0) * sign(a * b);
                cosv = sqrt((1.0 + g) / 2.0);
            }

            for (int i = 0; i < size; i++) {
                if (i == p || i == q) continue;
                const double tmpa = cosv * eigVal[p * size + i] - sinv * eigVal[q * size + i];
                const double tmpb = sinv * eigVal[p * size + i] + cosv * eigVal[q * size + i];

                eigVal[i * size + p] = eigVal[p * size + i] = static_cast<SP_REAL>(tmpa);
                eigVal[i * size + q] = eigVal[q * size + i] = static_cast<SP_REAL>(tmpb);
            }
            {
                eigVal[p * size + p] = static_cast<SP_REAL>(cosv * cosv * app + sinv * sinv * aqq - 2 * sinv * cosv * apq);
                eigVal[p * size + q] = static_cast<SP_REAL>(sinv * cosv * (app - aqq) + (cosv * cosv - sinv * sinv) * apq);
                eigVal[q * size + p] = static_cast<SP_REAL>(sinv * cosv * (app - aqq) + (cosv * cosv - sinv * sinv) * apq);
                eigVal[q * size + q] = static_cast<SP_REAL>(sinv * sinv * app + cosv * cosv * aqq + 2 * sinv * cosv * apq);
            }

            for (int i = 0; i < size; i++) {
                const double tmpa = cosv * eigVec[i * size + p] - sinv * eigVec[i * size + q];
                const double tmpb = sinv * eigVec[i * size + p] + cosv * eigVec[i * size + q];

                eigVec[i * size + p] = static_cast<SP_REAL>(tmpa);
                eigVec[i * size + q] = static_cast<SP_REAL>(tmpb);
            }
        }

        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                if (c == r) continue;

                eigVal[r * size + c] = 0.0;
            }
        }

        // sort
        for (int c = 0; c < size - 1; c++) {
            int maxid = c;
            int minid = c;

            SP_REAL maxv = 0.0;
            SP_REAL minv = SP_INFINITY;

            for (int i = c; i < size; i++) {
                const SP_REAL val = fabs(eigVal[i * size + i]);
                if (val > maxv) {
                    maxv = val;
                    maxid = i;
                }
                if (val < minv) {
                    minv = val;
                    minid = i;
                }
            }

            const int select = (minOrder == true) ? minid : maxid;

            if (select != c) {
                swap(eigVal[c * size + c], eigVal[select * size + select]);
                for (int r = 0; r < size; r++) {
                    swap(eigVec[r * size + c], eigVec[r * size + select]);
                }
            }
        }

        return true;
    }


    //--------------------------------------------------------------------------------
    // matrix svd (simgular value decomposition)
    //--------------------------------------------------------------------------------

    SP_GENFUNC bool svdMat(SP_REAL *U, SP_REAL *S, SP_REAL *V, const SP_REAL *mat, const int rows, const int cols, const bool minOrder = true) {
        if (rows < 2 || cols < 2 || rows < cols) return false;

        for (int i = 0; i < rows * cols; i++) {
            U[i] = mat[i];
        }
        for (int i = 0; i < cols * cols; i++) {
            V[i] = 0.0;
            S[i] = 0.0;
        }
        SP_REAL *Q = &S[0];
        SP_REAL *R = &S[cols];

        // householder reduction to bidiagonal form
        for (int i = 0; i < cols; i++) {

            {
                double scale = 0.0;
                for (int r = i; r < rows; r++) {
                    scale += fabs(U[r * cols + i]);
                }

                double val = 0.0;
                if (scale > 0.0) {
                    SP_REAL s = 0.0;
                    for (int r = i; r < rows; r++) {
                        U[r * cols + i] /= scale;
                        s += U[r * cols + i] * U[r * cols + i];
                    }

                    double f = U[i * cols + i];
                    double g = -sign(f) * sqrt(s);
                    U[i * cols + i] = f - g;

                    double h = f * g - s;

                    for (int j = i + 1; j < cols; j++) {
                        s = 0.0;
                        for (int r = i; r < rows; r++) {
                            s += U[r * cols + i] * U[r * cols + j];
                        }
                        f = s / h;
                        for (int r = i; r < rows; r++) {
                            U[r * cols + j] += f * U[r * cols + i];
                        }
                    }
                    for (int r = i; r < rows; r++) {
                        U[r * cols + i] *= scale;
                    }
                    val = scale * g;
                }
                Q[i] = val;
            }

            if (i < cols - 1) {
                SP_REAL scale = 0.0;
                for (int c = i + 1; c < cols; c++) {
                    scale += fabs(U[i * cols + c]);
                }

                double val = 0.0;
                if (scale > 0.0) {
                    double s = 0.0;

                    for (int c = i + 1; c < cols; c++) {
                        U[i * cols + c] /= scale;
                        s += U[i * cols + c] * U[i * cols + c];
                    }
                    double f = U[i * cols + i + 1];
                    double g = -sign(f) * sqrt(s);
                    double h = f * g - s;
                    U[i * cols + i + 1] = f - g;

                    for (int c = i + 1; c < cols; c++) {
                        R[c] = U[i * cols + c] / h;
                    }

                    for (int j = i + 1; j < rows; j++) {
                        s = 0.0;
                        for (int c = i + 1; c < cols; c++) {
                            s += U[j * cols + c] * U[i * cols + c];
                        }
                        for (int c = i + 1; c < cols; c++) {
                            U[j * cols + c] += s * R[c];
                        }
                    }
                    for (int c = i + 1; c < cols; c++) {
                        U[i * cols + c] *= scale;
                    }
                    val = scale * g;
                }
                R[i + 1] = val;
            }
        }

        double unorm = 0.0;
        for (int i = 0; i < cols; i++) {
            unorm = max(unorm, fabs(Q[i]) + fabs(R[i]));
        }

        // accumulation of right-hand transformations
        for (int i = cols - 1; i >= 0; i--) {
            const double g = R[i + 1];
            if (i < cols - 1) {
                if (g) {

                    // SP_REAL division to avoid possible underflow
                    for (int j = i + 1; j < cols; j++) {
                        V[j * cols + i] = (U[i * cols + j] / U[i * cols + (i + 1)]) / g;
                    }
                    for (int j = i + 1; j < cols; j++) {
                        double s = 0.0;
                        for (int k = i + 1; k < cols; k++) {
                            s += U[i * cols + k] * V[k * cols + j];
                        }
                        for (int k = i + 1; k < cols; k++) {
                            V[k * cols + j] += s * V[k * cols + i];
                        }
                    }
                }
                for (int j = i + 1; j < cols; j++) {
                    V[i * cols + j] = V[j * cols + i] = 0.0;
                }
            }
            V[i * cols + i] = 1.0;
        }

        // accumulation of left-hand transformations
        for (int i = min(rows, cols) - 1; i >= 0; i--) {

            for (int j = i + 1; j < cols; j++) {
                U[i * cols + j] = 0.0;
            }

            double g = Q[i];
            if (g) {
                g = static_cast<SP_REAL>(1.0 / g);
                for (int j = i + 1; j < cols; j++) {
                    double s = 0.0;
                    for (int k = i + 1; k < rows; k++) {
                        s += U[k * cols + i] * U[k * cols + j];
                    }
                    double f = (s / U[i * cols + i]) * g;

                    for (int k = i; k < rows; k++) {
                        U[k * cols + j] += f * U[k * cols + i];
                    }
                }
                for (int j = i; j < rows; j++) {
                    U[j * cols + i] *= g;
                }
            }
            else {
                for (int j = i; j < rows; j++) {
                    U[j * cols + i] = 0.0;
                }
            }
            U[i * cols + i]++;
        }

        // diagonalization of the bidiagonal form
        const int maxit = 30;

        for (int k = cols - 1; k >= 0; k--) {

            for (int it = 0; it < maxit; it++) {
                int flag = 1;

                int l = 0;
                int n = 0;

                // test for splitting
                for (l = k; l >= 0; l--) {

                    // Note that buf[0] is always zero
                    n = l - 1;
                    if ((fabs(R[l]) + unorm) == unorm) {
                        flag = 0;
                        break;
                    }
                    if ((fabs(Q[n]) + unorm) == unorm) {
                        break;
                    }
                }
                if (flag) {
                    // cancellation of R[l], if l > 0
                    double c = 0.0;
                    double s = 1.0;
                    for (int i = l; i <= k; i++) {
                        double f = s * R[i];
                        R[i] = c * R[i];

                        if ((SP_REAL)(fabs(f) + unorm) == unorm) {
                            break;
                        }
                        double g = Q[i];
                        double h = pythag(f, g);
                        Q[i] = h;
                        h = 1.0 / h;
                        c = g * h;
                        s = -f * h;
                        for (int j = 0; j < rows; j++) {
                            const double y = U[j * cols + n];
                            const double z = U[j * cols + i];
                            U[j * cols + n] = y * c + z * s;
                            U[j * cols + i] = z * c - y * s;
                        }
                    }
                }
                double z = Q[k];

                // convergence
                if (l == k) {
                    // singular value is made nonnegative
                    if (z < 0.0) {
                        Q[k] = -z;
                        for (int j = 0; j < cols; j++) {
                            V[j * cols + k] = -V[j * cols + k];
                        }
                    }
                    break;
                }

                if (it == maxit - 1) {
                    // no convergence in SVD iterations
                    return false;
                }

                // shift from bottom 2-by-2 minor
                n = k - 1;
                double x = Q[l];
                double y = Q[n];
                double g = R[n];
                double h = R[k];
                double f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
                g = pythag(f, 1.0);
                f = ((x - z)*(x + z) + h * ((y / (f + sign(f) * g)) - h)) / x;

                double c = 1.0;
                double s = 1.0;

                // next QR transformation
                for (int j = l; j <= n; j++) {
                    int i = j + 1;
                    g = R[i];
                    y = Q[i];
                    h = s * g;
                    g = c * g;
                    z = pythag(f, h);
                    R[j] = z;
                    c = f / z;
                    s = h / z;
                    f = x * c + g * s;
                    g = g * c - x * s;
                    h = y * s;
                    y *= c;
                    for (int jj = 0; jj < cols; jj++) {
                        x = V[jj * cols + j];
                        z = V[jj * cols + i];
                        V[jj * cols + j] = x * c + z * s;
                        V[jj * cols + i] = z * c - x * s;
                    }
                    z = pythag(f, h);
                    Q[j] = z; /* Rotation can be arbitrary if z = 0. */
                    if (z) {
                        z = 1.0 / z;
                        c = f * z;
                        s = h * z;
                    }
                    f = c * g + s * y;
                    x = c * y - s * g;
                    for (int jj = 0; jj < rows; jj++) {
                        y = U[jj * cols + j];
                        z = U[jj * cols + i];
                        U[jj * cols + j] = y * c + z * s;
                        U[jj * cols + i] = z * c - y * s;
                    }
                }
                R[l] = 0.0;
                R[k] = f;
                Q[k] = x;
            }
        }

        for (int i = 0; i < cols; i++) {
            R[i] = 0.0;
        }
        for (int i = 0; i < cols; i++) {
            swap(S[i * cols + i], Q[i]);
        }

        // sort
        for (int c = 0; c < cols - 1; c++) {
            int maxid = c;
            int minid = c;

            double maxv = 0.0;
            double minv = SP_INFINITY;

            for (int i = c; i < cols; i++) {
                const SP_REAL val = S[i * cols + i];
                if (val > maxv) {
                    maxv = val;
                    maxid = i;
                }
                if (val < minv) {
                    minv = val;
                    minid = i;
                }
            }

            const int select = (minOrder == true) ? minid : maxid;

            if (select != c) {
                swap(S[c * cols + c], S[select * cols + select]);
                for (int r = 0; r < rows; r++) {
                    swap(U[r * cols + c], U[r * cols + select]);
                }
                for (int r = 0; r < cols; r++) {
                    swap(V[r * cols + c], V[r * cols + select]);
                }
            }
        }
        return true;
    }

}

#endif