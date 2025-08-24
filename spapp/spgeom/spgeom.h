//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_GEOM_H__
#define __SP_GEOM_H__

#include "spcore/spcore.h"

namespace sp {

    //--------------------------------------------------------------------------------
    // projection error
    //--------------------------------------------------------------------------------

    SP_CPUFUNC SP_REAL calcPrjErr(const Pose &pose, const CamParam &cam, const Vec2 &pix, const Vec3 &obj) {
        const Vec3 vec = pose * obj;

        SP_REAL ret = 0.0;
        if (vec.z > 0) {
            ret = (pix - mulCamD(cam, prjVec(vec))).length();
        }
        else {
            ret = SP_INFINITY;
        }

        return ret;
    }

    SP_CPUFUNC SP_REAL calcPrjErr(const Pose &pose, const CamParam &cam, const Vec2 &pix, const Vec2 &obj) {
        return calcPrjErr(pose, cam, pix, Vec3(obj.x, obj.y, 0.0));
    }

    SP_CPUFUNC Mem1<SP_REAL> calcPrjErr(const Pose &pose, const CamParam &cam, const Mem1<Vec2> &pixs, const Mem1<Vec3> &objs) {
        SP_ASSERT(pixs.size() == objs.size());

        Mem1<SP_REAL> errs(pixs.size());
        for (int i = 0; i < pixs.size(); i++) {
            errs[i] = calcPrjErr(pose, cam, pixs[i], objs[i]);
        }
        return errs;
    }


    SP_CPUFUNC Mem1<SP_REAL> calcPrjErr(const Mem1<Pose> &poses, const Mem1<CamParam> &cams, const Mem1<Vec2> &pixs, const Vec3 &obj) {
        SP_ASSERT(poses.size() == cams.size() && poses.size() == pixs.size());

        Mem1<SP_REAL> errs(poses.size());
        for (int i = 0; i < poses.size(); i++) {
            errs[i] = calcPrjErr(poses[i], cams[i], pixs[i], obj);
        }
        return errs;
    }

    //--------------------------------------------------------------------------------
    // triangulation
    //--------------------------------------------------------------------------------

    SP_CPUFUNC bool refinePnt3d(Vec3 &pnt, const Mem1<Pose> &poses, const Mem1<Vec2> &npxs, const int maxit = 1) {
        SP_ASSERT(poses.size() == npxs.size());

        const int unit = 2;
        if (npxs.size() < unit) return false;

        Mat J(poses.size() * 2, 3);
        Mat E(poses.size() * 2, 1);
        Mem1<SP_REAL> errs(poses.size());

        Mat jacob(2, 3);
        for (int it = 0; it < maxit; it++) {
            for (int i = 0; i < poses.size(); i++) {
                const Vec3 pos = poses[i] * pnt;

                jacobPosToNpx(jacob.ptr, pos);
                const Mat R = getMat(poses[i].rot);
                jacob = jacob * R;
                memcpy(&J(i * 2, 0), jacob.ptr, jacob.size() * sizeof(SP_REAL));

                const Vec2 err = npxs[i] - prjVec(pos);
                E(i * 2 + 0, 0) = err.x;
                E(i * 2 + 1, 0) = err.y;
                errs[i] = err.length();
            }

            Mat result;
            if (solver::solveAX_B(result, J, E, solver::calcW(errs, 2)) == false) return false;

            pnt += Vec3(result[0], result[1], result[2]);
        }

        for (int i = 0; i < poses.size(); i++) {
            if ((poses[i] * pnt).z <= 0.0) return false;
        }

        return true;
    }

    SP_CPUFUNC bool refinePnt3d(Vec3 &pnt, const Mem1<Pose> &poses, const Mem1<CamParam> &cams, const Mem1<Vec2> &pixs, const int maxit = 1) {
        SP_ASSERT(poses.size() == cams.size() && poses.size() == pixs.size());

        Mem1<Vec2> npxs(pixs.size());
        for (int i = 0; i < npxs.size(); i++) {
            npxs[i] = invCamD(cams[i], pixs[i]);
        }
        return refinePnt3d(pnt, poses, npxs, maxit);
    }


    SP_CPUFUNC bool calcPnt3d(Vec3 &pnt, const Mem1<Pose> &poses, const Mem1<Vec2> &npxs) {
        SP_ASSERT(poses.size() == npxs.size());

        Mat M(poses.size() * 2, 3);
        Mat V(poses.size() * 2, 1);

        for (int i = 0; i < poses.size(); i++) {
            const Vec2 &npx = npxs[i];

            const Mat R = getMat(poses[i].rot);
            const Vec3 &trn = poses[i].pos;

            M(i * 2 + 0, 0) = R(0, 0) - npx.x * R(2, 0);
            M(i * 2 + 0, 1) = R(0, 1) - npx.x * R(2, 1);
            M(i * 2 + 0, 2) = R(0, 2) - npx.x * R(2, 2);

            M(i * 2 + 1, 0) = R(1, 0) - npx.y * R(2, 0);
            M(i * 2 + 1, 1) = R(1, 1) - npx.y * R(2, 1);
            M(i * 2 + 1, 2) = R(1, 2) - npx.y * R(2, 2);

            V(i * 2 + 0, 0) = npx.x * trn.z - trn.x;
            V(i * 2 + 1, 0) = npx.y * trn.z - trn.y;
        }

        Mat result;
        if (solver::solveAX_B(result, M, V) == false) return false;

        pnt = Vec3(result[0], result[1], result[2]);

        for (int i = 0; i < poses.size(); i++) {
            if ((poses[i] * pnt).z <= 0.0) return false;
        }

        return true;
    }

    SP_CPUFUNC bool calcPnt3d(Vec3 &pnt, const Mem1<Pose> &poses, const Mem1<CamParam> &cams, const Mem1<Vec2> &pixs) {
        SP_ASSERT(poses.size() == cams.size() && poses.size() == pixs.size());

        Mem1<Vec2> npxs(pixs.size());
        for (int i = 0; i < npxs.size(); i++) {
            npxs[i] = invCamD(cams[i], pixs[i]);
        }
        return calcPnt3d(pnt, poses, npxs);
    }

    SP_CPUFUNC bool calcPnt3d(Vec3 &pnt, const Pose &pose0, const Vec2 &npx0, const Pose &pose1, const Vec2 &npx1) {
        const Pose _poses[2] = { pose0, pose1 };
        const Vec2 _npxs[2] = { npx0, npx1 };

        const Mem1<Pose> poses(2, _poses);
        const Mem1<Vec2> npxs(2, _npxs);

        return calcPnt3d(pnt, poses, npxs);
    }

    SP_CPUFUNC bool calcPnt3d(Vec3 &pnt, const Pose &pose0, const CamParam &cam0, const Vec2 &pix0, const Pose &pose1, const CamParam &cam1, const Vec2 &pix1) {
        const Pose _poses[2] = { pose0, pose1 };
        const CamParam _cams[2] = { cam0, cam1 };
        const Vec2 _pixs[2] = { pix0, pix1 };

        const Mem1<Pose> poses(2, _poses);
        const Mem1<CamParam> cams(2, _cams);
        const Mem1<Vec2> pixs(2, _pixs);

        return calcPnt3d(pnt, poses, cams, pixs);
    }

    SP_CPUFUNC bool calcPnt3d(Mem1<Vec3> &pnts, Mem1<bool> &mask, const Pose &pose0, const CamParam &cam0, const Mem1<Vec2> &pixs0, const Pose &pose1, const CamParam &cam1, const Mem1<Vec2> &pixs1) {
        SP_ASSERT(pixs0.size() == pixs1.size());

        pnts.resize(pixs0.size());
        mask.resize(pixs0.size());
        pnts.zero();
        mask.zero();

        for (int i = 0; i < pixs0.size(); i++) {
            Vec3 pnt;
            if (calcPnt3d(pnt, pose0, cam0, pixs0[i], pose1, cam1, pixs1[i]) == false) continue;

            pnts[i] = pnt;
            mask[i] = true;
        }
        return true;
    }

    //--------------------------------------------------------------------------------
    // pose (refine)
    //--------------------------------------------------------------------------------
  
    // 3D-3D pose
    SP_CPUFUNC bool refinePose(Pose &pose, const Mem1<Vec3> &objs0, const Mem1<Vec3> &objs1, const int maxit = 10) {
        SP_ASSERT(objs0.size() == objs1.size());

        const int num = objs0.size();

        const int unit = 3;
        if (num < unit) return false;
        
        Mat J(3 * num, 6);
        Mat E(3 * num, 1);
        Mem1<SP_REAL> errs(num);

        for (int it = 0; it < maxit; it++) {
            for (int i = 0; i < num; i++) {
                jacobPoseToPos(&J(i * 3, 0), pose, objs1[i]);

                const Vec3 err = objs0[i] - pose * objs1[i];
                E(i * 3 + 0, 0) = err.x;
                E(i * 3 + 1, 0) = err.y;
                E(i * 3 + 2, 0) = err.z;

                errs[i] = err.length();
            }

            Mat delta;
            if (solver::solveAX_B(delta, J, E, solver::calcW(errs, 3)) == false) return false;

            pose = updatePose(pose, delta.ptr);
        }

        return true;
    }


    //--------------------------------------------------------------------------------
    // pose (basic)
    //--------------------------------------------------------------------------------

    // 3D-3D pose
    SP_CPUFUNC bool calcPose(Pose &pose, const Mem1<Vec3> &objs0, const Mem1<Vec3> &objs1, const int maxit = 10) {
        SP_ASSERT(objs0.size() == objs1.size());

        const int num = objs0.size();

        const int unit = 3;
        if (num < unit) return false;

        const Vec3 cent0 = mean(objs0);
        const Vec3 cent1 = mean(objs1);

        const Mem1<Vec3> mobjs0 = objs0 - cent0;
        const Mem1<Vec3> mobjs1 = objs1 - cent1;

        {
            Mat mat = zeroMat(3, 3);
            for (int i = 0; i < num; i++) {
                const SP_REAL *y = reinterpret_cast<const SP_REAL*>(&mobjs0[i]);
                const SP_REAL *x = reinterpret_cast<const SP_REAL*>(&mobjs1[i]);

                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        mat(r, c) += x[r] * y[c];
                    }
                }
            }
            Mat U, S, V;
            svdMat(U, S, V, mat, false);

            Mat H = eyeMat(3, 3);
            H(2, 2) = detMat(V * trnMat(U));

            pose.rot = getRot(V * H * trnMat(U));

            pose.pos = cent0 - pose.rot * cent1;
        }

        if (maxit - 1 > 0) {
            if (refinePose(pose, objs0, objs1, maxit - 1) == false) return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------------
    // pose (RANSAC + refine)
    //--------------------------------------------------------------------------------

    SP_CPUFUNC bool calcPnt3dRANSAC(Vec3 &pos, const Mem1<Pose> &poses, const Mem1<CamParam> &cams, const Mem1<Vec2> &pixs, const SP_REAL thresh = 3.0) {
        SP_ASSERT(poses.size() == cams.size() && poses.size() == pixs.size());

        const int num = poses.size();
        const int unit = 2;

        if (num < unit) return false;
        if (pixs.size() < unit * 2) {
            return calcPnt3d(pos, poses, cams, pixs);
        }

        int maxit = ransacAdaptiveStop(SP_RANSAC_MINEVAL, unit);

        RandomSample<Pose> _poses(poses, unit);
        RandomSample<CamParam> _cams(cams, unit);
        RandomSample<Vec2> _pixs(pixs, unit);

        SP_REAL maxe = 0.0;
        int it = 0;
        for (it = 0; it < maxit; it++) {

            const Mem1<Pose> rposes = _poses.gen(it);
            const Mem1<CamParam> rcams = _cams.gen(it);
            const Mem1<Vec2> rpixs = _pixs.gen(it);

            Vec3 test;
            if (calcPnt3d(test, rposes, rcams, rpixs) == false) continue;

            const Mem1<SP_REAL> errs = calcPrjErr(poses, cams, pixs, test);
            const SP_REAL eval = ransacEval(errs, unit, thresh);

            if (eval > maxe) {
                //SP_PRINTD("eval %lf\n", eval);
                maxe = eval;
                maxit = ransacAdaptiveStop(eval, unit);

                pos = test;
            }
        }
        //SP_PRINTD("RANSAC iteration %d rate %.2lf\n", it, maxe);
        if (maxe < SP_RANSAC_MINEVAL) return false;

        // refine
        {
            const Mem1<SP_REAL> errs = calcPrjErr(poses, cams, pixs, pos);
            const Mem1<Pose> dposes = filter(poses, errs, thresh * 2);
            const Mem1<CamParam> dcams = filter(cams, errs, thresh * 2);
            const Mem1<Vec2> dpixs = filter(pixs, errs, thresh * 2);

            if (refinePnt3d(pos, dposes, dcams, dpixs) == false) return false;
        }
        return true;
    }

    // 3D-3D pose
    SP_CPUFUNC bool calcPoseRANSAC(Pose &pose, const Mem1<Vec3> &objs0, const Mem1<Vec3> &objs1, const SP_REAL thresh = 10.0) {
        SP_ASSERT(objs0.size() == objs1.size());
      
        const int num = objs0.size();
        const int unit = 3;

        if (num < unit) return false;
        if (num < unit * 2) {
            return calcPose(pose, objs0, objs1);
        }

        int maxit = ransacAdaptiveStop(SP_RANSAC_MINEVAL, unit);

        RandomSample<Vec3> _objs0(objs0, unit);
        RandomSample<Vec3> _objs1(objs1, unit);

        SP_REAL maxe = 0.0;
        int it = 0;
        for (it = 0; it < maxit; it++) {

            const Mem1<Vec3> robjs0 = _objs0.gen(it);
            const Mem1<Vec3> robjs1 = _objs1.gen(it);

            Pose test;
            if (calcPose(test, robjs0, robjs1, 1) == false) continue;

            Mem1<SP_REAL> errs;
            for (int i = 0; i < num; i++) {
                errs.push((objs0[i] - test * objs1[i]).length());
            }
            const SP_REAL eval = ransacEval(errs, unit, thresh);

            if (eval > maxe) {
                //SP_PRINTD("eval %lf\n", eval);
                maxe = eval;
                maxit = ransacAdaptiveStop(eval, unit);

                pose = test;
            }
        }
        //SP_PRINTD("RANSAC iteration %d rate %.2lf\n", it, maxe);
        if (maxe < SP_RANSAC_MINEVAL) return false;

        // refine
        {
            Mem1<SP_REAL> errs;
            for (int i = 0; i < num; i++) {
                errs.push((objs0[i] - pose * objs1[i]).length());
            }
            const Mem1<Vec3> dobjs0 = filter(objs0, errs, thresh * 2);
            const Mem1<Vec3> dobjs1 = filter(objs1, errs, thresh * 2);

            if (refinePose(pose, dobjs0, dobjs1) == false) return false;
        }

        return true;
    }

}
#endif