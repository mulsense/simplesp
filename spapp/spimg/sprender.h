//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_RENDER_H__
#define __SP_RENDER_H__

#include "spcore/spcore.h"

namespace sp{

    //--------------------------------------------------------------------------------
    // render mesh
    //--------------------------------------------------------------------------------

    template<typename TYPE>
    SP_CPUFUNC void renderMesh(Mem<TYPE> &dst, const CamParam &cam, const Pose &pose, const Mesh3 &mesh, const TYPE &val, const double thick = 1.0){

        renderLine(dst, cam, pose, mesh.pos[0], mesh.pos[1], val, thick);
        renderLine(dst, cam, pose, mesh.pos[1], mesh.pos[2], val, thick);
        renderLine(dst, cam, pose, mesh.pos[2], mesh.pos[0], val, thick);
    }

    template<typename TYPE>
    SP_CPUFUNC void renderMesh(Mem<TYPE> &dst, const CamParam &cam, const Pose &pose, const Mem1<Mesh3> &mesh, const TYPE &val, const double thick = 1.0){

        for (int i = 0; i < mesh.size(); i++){
            renderMesh(dst, cam, pose, mesh[i], val, thick);
        }
    }


    //--------------------------------------------------------------------------------
    // render geom
    //--------------------------------------------------------------------------------
    
    SP_CPUFUNC void renderVecPD(Mem<VecPD3> &dst, const CamParam &cam, const Pose &pose, const Mesh3 &mesh) {

        if (cmp(dst.dsize, cam.dsize, 2) == false) {
            dst.resize(2, cam.dsize);
            dst.zero();
        }

        Rect2 rect;

        const Mesh3 pm = pose * mesh;
        {
            int xs = dst.dsize[0];
            int xe = 0;

            int ys = dst.dsize[1];
            int ye = 0;

            bool valid = false;
            for (int i = 0; i < 3; i++) {
                if (pm.pos[i].z < SP_SMALL) continue;

                valid |= true;

                const Vec2 pix = mulCamD(cam, prjVec(pm.pos[i]));

                xs = min(xs, floor(pix.x + 1));
                xe = max(xe, floor(pix.x + 1));

                ys = min(ys, floor(pix.y + 1));
                ye = max(ye, floor(pix.y + 1));
            }
            if (valid == false) return;

            rect = andRect(getRect2(dst.dsize), getRect2(xs, ys, xe - xs, ye - ys));
        }

        const Vec3 nrm = pm.normal();

        const Vec3 base = pm.pos[0];
        const Vec3 A = pm.pos[1] - base;
        const Vec3 B = pm.pos[2] - base;

        SP_REAL mat[3 * 3] = { -A.x, -B.x, 0.0, -A.y, -B.y, 0.0, -A.z, -B.z, 0.0 };
        SP_REAL val[3] = { base.x, base.y, base.z };

        for (int v = rect.dbase[1]; v < rect.dbase[1] + rect.dsize[1]; v++) {
            for (int u = rect.dbase[0]; u < rect.dbase[0] + rect.dsize[0]; u++) {
                const Vec2 prj = npxUndist(cam, invCam(cam, Vec2(u, v)));
                const Vec3 vec = Vec3(prj.x, prj.y, 1.0);

                mat[0 * 3 + 2] = vec.x;
                mat[1 * 3 + 2] = vec.y;
                mat[2 * 3 + 2] = vec.z;

                SP_REAL inv[3 * 3];
                if (invMat33(inv, mat) == false) continue;

                SP_REAL result[3];
                mulMat(result, 3, 1, inv, 3, 3, val, 3, 1);

                if (result[0] < 0.0 || result[1] < 0.0 || result[0] + result[1] > 1.0) continue;

                const SP_REAL depth = result[2];
                if (depth < SP_SMALL) continue;

                const SP_REAL ref = acs2(dst, u, v).pos.z;
                if (ref == 0.0 || depth < ref) {
                    acs2(dst, u, v) = VecPD3(vec * depth, nrm);
                }
            }
        }

    }

    SP_CPUFUNC void renderVecPD(Mem<VecPD3> &dst, const CamParam &cam, const Pose &pose, const Mem<Mesh3> &meshes) {

        for (int i = 0; i < meshes.size(); i++) {
            renderVecPD(dst, cam, pose, meshes[i]);
        }
    }

    SP_CPUFUNC SP_REAL extractZ(const VecPD3& src) {
        return src.pos.z;
    }

    SP_CPUFUNC SP_REAL extractZ(const Vec3& src) {
        return src.z;
    }

    SP_CPUFUNC SP_REAL extractZ(const double src) {
        return src;
    }

    template<typename DEPTH>
    SP_CPUFUNC void renderDepth(Mem<DEPTH> &dst, const CamParam &cam, const Pose &pose, const Mem<Mesh3> &meshes) {

        Mem<VecPD3> pnmap;
        for (int i = 0; i < meshes.size(); i++) {
            renderVecPD(pnmap, cam, pose, meshes[i]);
        }

        dst.resize(2, cam.dsize);
        for (int i = 0; i < dst.size(); i++) {
            dst[i] = extractZ(pnmap[i]);
        }
    }

}

#endif
