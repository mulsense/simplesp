//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_MODEL_H__
#define __SP_MODEL_H__

#include "spcore/spcore.h"
#include "spcore/spkdtree.h"
#include "spapp/spimg/sprender.h"

namespace sp{
    
    //--------------------------------------------------------------------------------
    // model edge
    //--------------------------------------------------------------------------------
   
    struct Edge {
        Vec3 pos, drc, nrm[2];
    };


    //--------------------------------------------------------------------------------
    // model util
    //--------------------------------------------------------------------------------

    SP_CPUFUNC Vec3 getModelCent(const Mem1<Mesh3> &model){
        Vec3 sum = Vec3(0.0, 0.0, 0.0);
        for (int i = 0; i < model.size(); i++){
            sum += model[i].center();
        }

        return sum / model.size();
    }

    SP_CPUFUNC SP_REAL getModelRadius(const Mem1<Mesh3> &model){
        Mem1<SP_REAL> mem(model.size());
        for (int i = 0; i < mem.size(); i++){
            mem[i] = model[i].center().length();
        }

        return max(mem);
    }

    SP_CPUFUNC SP_REAL getModelDistance(const Mem1<Mesh3> &model, const CamParam &cam){

        const double radius = getModelRadius(model);
        const double distance = 1.2 * std::max(cam.fx, cam.fy) * radius / (0.5 * std::min(cam.dsize[0], cam.dsize[1]));
    
        return static_cast<SP_REAL>(distance);
    }

    SP_CPUFUNC Mem1<VecPD3> getModelPoint(const Mem1<Mesh3> &model, const int density = 50){
        const CamParam cam = getCamParam(density, density);
        const SP_REAL distance = getModelDistance(model, cam);
        
        Mem1<VecPD3> tmp;
        const int num = getGeodesicMeshNum(0);
        for (int i = 0; i < num; i++){
            const Vec3 v = getGeodesicMesh(0, i).center() * (-1.0);
            const Pose pose = getPose(getRotDirection(v), Vec3(0.0, 0.0, distance));
            Mem2<VecPD3> map;
            renderVecPD(map, cam, pose, model);

            const Mat mat = getMat(invPose(pose));
            for (int j = 0; j < map.size(); j++){
                if (map[j].pos.z > 0 && map[j].pos.dot(map[j].drc) < 0){
                    tmp.push(mat * map[j]);
                }
            }
        }

        tmp = shuffle(tmp);

        Mem1<VecPD3> pnts;
        const double unit = 2 * distance / (cam.fx + cam.fy);

        for (int i = 0; i < tmp.size(); i++){
            bool check = true;
            for (int j = 0; j < pnts.size(); j++){
                if (pnts[j].drc.dot(tmp[i].drc) > 0.5 && (pnts[j].pos - tmp[i].pos).length() < unit){
                    check = false;
                    break;
                }
            }
            if (check == true){
                pnts.push(tmp[i]);
            }
        }

        return pnts;
    }

    SP_CPUFUNC Mem1<Edge> getModelEdge(const Mem1<Mesh3> &model, const int density = 50) {

        KdTree<SP_REAL> kdtree;
        kdtree.init(3);
        for (int i = 0; i < model.size(); i++) {
            for (int j = 0; j < 3; j++) {
                kdtree.addData(&model[i].pos[j]);
            }
        }
        kdtree.makeTree();

        Mem1<Edge> edges;

        const SP_REAL radius = getModelRadius(model);
        const SP_REAL unit = 2.0 * radius / density;

        for (int i = 0; i < model.size(); i++) {

            for (int j = 0; j < 3; j++) {
                const Vec3 A = model[i].pos[(j + 0) % 3];
                const Vec3 B = model[i].pos[(j + 1) % 3];
                const Vec3 V = (B - A).unit();

                const Mem1<int> list = kdtree.search(&A, (B - A).length() + 0.01);

                for (int k = 0; k < list.size(); k++) {
                    const int mid = list[k] / 3;
                    const int pid = list[k] % 3;
                    if (mid <= i) continue;

                    const Vec3 C = model[mid].pos[(pid + 0) % 3];
                    const Vec3 D = model[mid].pos[(pid + 1) % 3];

                    const Vec3 F = (C - A).length() > (D - A).length() ? C : D;

                    if (fabs(V.dot((F - A).unit())) < 0.99) continue;
                    if (fabs(V.dot((D - C).unit())) < 0.99) continue;

                    const Vec3 O = V.dot(C) < V.dot(D) ? C : D;
                    const Vec3 P = V.dot(C) < V.dot(D) ? D : C;

                    const Vec3 X = V.dot(A) > V.dot(O) ? A : O;
                    const Vec3 Y = V.dot(B) < V.dot(P) ? B : P;

                    if ((Y - X).length() < 0.01) continue;

                    const int div = ceil((Y - X).length() / unit);
                    for (int d = 0; d < div; d++) {
                        Edge edge;
                        edge.pos = (Y - X) / (div + 1.0) * (d + 1.0) + X;
                        edge.drc = V;
                        edge.nrm[0] = model[i].normal();
                        edge.nrm[1] = model[mid].normal();

                        edges.push(edge);
                    }
                }

            }
        }

        return edges;
    }


    //--------------------------------------------------------------------------------
    // pose model
    //--------------------------------------------------------------------------------

    class PoseModel {
    public:

        Pose pose;
        Mem1<Edge> edges;
        Mem1<VecPD3> pnts;

    public:

        PoseModel() {
        }

        PoseModel(const PoseModel &pmodel) {
            *this = pmodel;
        }

        PoseModel& operator = (const PoseModel &pmodel) {
            pose = pmodel.pose;
            edges = pmodel.edges;
            pnts = pmodel.pnts;
            return *this;
        }
    };


    SP_CPUFUNC Mem1<PoseModel> getPoseModel(const Mem1<Mesh3> &model, const double distance, const int level = 2, const int density = 50) {

        const double radius = getModelRadius(model);
        const double unit = 2.0 * radius / density;

        const int size = 300;
        const double f = distance * size / (1.2 * 2.0 * radius);
        const CamParam cam = getCamParam(size, size, f, f);

        Mem1<PoseModel> pmodels;

        // pose
        {
            const int num = getGeodesicMeshNum(level);
            pmodels.resize(num);

            for (int i = 0; i < num; i++) {
                const Vec3 v = getGeodesicMesh(level, i).center() * (-1.0);
                const Pose pose = getPose(getRotDirection(v), Vec3(0.0, 0.0, distance));
               
                pmodels[i].pose = pose;
            }
        }

        // contour edge
        {
            const Mem1<Edge> &edges = getModelEdge(model, density);
  
            KdTree<SP_REAL> kdtree;
            kdtree.init(3);
            for (int i = 0; i < edges.size(); i++) {
                kdtree.addData(&edges[i].pos);
            }
            kdtree.makeTree();

#if SP_USE_OMP
#pragma omp parallel for
#endif
            for (int i = 0; i < pmodels.size(); i++) {
                PoseModel &pmodel = pmodels[i];
                pmodel.edges.clear();

                const Pose &pose = pmodel.pose;

                Mem2<VecPD3> map;
                renderVecPD(map, cam, pose, model);

                const Mat pmat = getMat(pose);
                const Mat rmat = getMat(pose.rot);

                Mem1<bool> flags(edges.size());
                flags.zero();

                for (int j = 0; j < edges.size(); j++) {
                    const Vec3 pos = pmat * edges[j].pos;
                    const Vec2 pix = mulCamD(cam, prjVec(pos));
                    if (flags[j] == true) continue;

                    const int x = round(pix.x);
                    const int y = round(pix.y);
                    bool contour = false ;
                    for (int v = -1; v <= 1; v++) {
                        for (int u = -1; u <= 1; u++) {
                            const VecPD3 &vec = map(x + u, y + v);
                            if (vec.pos.z == 0.0) {
                                contour = true;
                                goto _exit0;
                            }
                        }
                    }
                _exit0:

                    const Vec3 nrm0 = rmat * edges[j].nrm[0];
                    const Vec3 nrm1 = rmat * edges[j].nrm[1];
                    
                    if (contour == true && nrm0.dot(pos) * nrm1.dot(pos) <= 0.0)
                    {
                        pmodel.edges.push(edges[j]);

                        const Mem1<int> list = kdtree.search(&edges[j].pos, unit);
                        for (int k = 0; k < list.size(); k++) {
                            flags[list[k]] = true;
                        }
                    }
                }
            }
        }

        // surface point
        {
            const Mem1<VecPD3> &pnts = getModelPoint(model, density);

#if SP_USE_OMP
#pragma omp parallel for
#endif
            for (int i = 0; i < pmodels.size(); i++) {
                PoseModel &pmodel = pmodels[i];
                pmodel.pnts.clear();

                const Pose &pose = pmodel.pose;

                Mem2<VecPD3> map;
                renderVecPD(map, cam, pose, model);
                
                const Mat pmat = getMat(pose);
                const Mat rmat = getMat(pose.rot);

                for (int j = 0; j < pnts.size(); j++) {
                    const Vec3 pos = pmat * pnts[j].pos;
                    const Vec3 drc = rmat * pnts[j].drc;

                    const Vec2 pix = mulCamD(cam, prjVec(pos));

                    const int x = round(pix.x);
                    const int y = round(pix.y);
                    bool visible = false;
                    for (int v = -1; v <= 1; v++) {
                        for (int u = -1; u <= 1; u++) {
                            const VecPD3 &vec = map(x + u, y + v);
                            if (vec.pos.z > pos.z - SP_SMALL) {
                                visible = true;
                                goto _exit1;
                            }
                        }
                    }
                _exit1:

                    if (visible == true && drc.dot(pos) <= 0.0) {
                        pmodel.pnts.push(pnts[j]);
                    }
                }
            }
        }

        return pmodels;
    }


    //--------------------------------------------------------------------------------
    // sample model
    //--------------------------------------------------------------------------------

    SP_CPUFUNC Mem1<Mesh3> loadPlane(const double size, const int xyz, const int nrm) {
        Mem1<Mesh3> model;
        const double hs = size * 0.5;
        Vec3 a, b, c, d;
        switch(xyz){
        case 0:
            a = Vec3(0.0, -hs, -hs);
            b = Vec3(0.0, +hs, -hs);
            c = Vec3(0.0, +hs, +hs);
            d = Vec3(0.0, -hs, +hs);
            break;
        case 1:
            a = Vec3(-hs, 0.0, -hs);
            b = Vec3(-hs, 0.0, +hs);
            c = Vec3(+hs, 0.0, +hs);
            d = Vec3(+hs, 0.0, -hs);
            break;
        case 2:
            a = Vec3(-hs, -hs, 0.0);
            b = Vec3(+hs, -hs, 0.0);
            c = Vec3(+hs, +hs, 0.0);
            d = Vec3(-hs, +hs, 0.0);
            break;

        }
        if (nrm > 0) {
            model.push(Mesh3(a, b, c));
            model.push(Mesh3(a, c, d));
        }
        else {
            model.push(Mesh3(c, b, a));
            model.push(Mesh3(c, a, d));
        }
        return model;
    }

    SP_CPUFUNC Mem1<Mesh3> loadGeodesicDorm(const double size, const int div) {
        Mem1<Mesh3> model;

        const int num = getGeodesicMeshNum(div);
        for (int i = 0; i < num; i++) {
            model.push(getGeodesicMesh(div, i) * size);
        }

        return model;
    }

    SP_CPUFUNC Mem1<Mesh3> loadCube(const double size) {
        Mem1<Mesh3> model;

        const double half = size / 2.0;

        for (int z = -1; z <= +1; z += 2) {
            for (int y = -1; y <= +1; y += 2) {
                for (int x = -1; x <= +1; x += 2) {
                    if ((x * y * z) > 0) continue;

                    const Vec3 p0 = Vec3(+x, +y, +z) * half;
                    const Vec3 px = Vec3(-x, +y, +z) * half;
                    const Vec3 py = Vec3(+x, -y, +z) * half;
                    const Vec3 pz = Vec3(+x, +y, -z) * half;

                    model.push(Mesh3(p0, py, px));
                    model.push(Mesh3(p0, pz, py));
                    model.push(Mesh3(p0, px, pz));
                }
            }
        }
          
        return model;
    }

    SP_CPUFUNC Mem1<Mesh3> loadCone(const Vec3 &drc, const double radius, const int div = 36) {
        Mem1<Mesh3> model;

        const Vec3 nx = getRotDirection(drc).inverse() * Vec3(1.0, 0.0, 0.0);
        const Vec3 ny = getRotDirection(drc).inverse() * Vec3(0.0, 1.0, 0.0);

        const double step = 360.0 / div;
        for (int i = 0; i < div; i++) {
            const double pa = (i + 0) * step * SP_PI / 180.0;
            const double pb = (i + 1) * step * SP_PI / 180.0;
            const Vec3 a = (nx * cos(pa) + ny * sin(pa)) * radius;
            const Vec3 b = (nx * cos(pb) + ny * sin(pb)) * radius;
            const Vec3 c = drc;

            model.push(Mesh3(a, b, c));
            model.push(Mesh3(b, a, Vec3(0.0, 0.0, 0.0)));
        }

        return model;
    }

    SP_CPUFUNC Mem1<Mesh3> loadCylinder(const Vec3 &drc, const double radius, const int div = 36) {
        Mem1<Mesh3> model;

        const Vec3 nx = getRotDirection(drc).inverse() * Vec3(1.0, 0.0, 0.0);
        const Vec3 ny = getRotDirection(drc).inverse() * Vec3(0.0, 1.0, 0.0);

        const double step = 360.0 / div;
        for (int i = 0; i < div; i++) {
            const double pa = (i + 0) * step * SP_PI / 180.0;
            const double pb = (i + 1) * step * SP_PI / 180.0;
            const Vec3 a = (nx * cos(pa) + ny * sin(pa)) * radius;
            const Vec3 b = (nx * cos(pb) + ny * sin(pb)) * radius;
            const Vec3 c = a + drc;
            const Vec3 d = b + drc;

            model.push(Mesh3(a, b, d));
            model.push(Mesh3(d, c, a));
            model.push(Mesh3(b, a, Vec3(0.0, 0.0, 0.0)));
            model.push(Mesh3(a, b, Vec3(0.0, 0.0, 0.0)) + drc);
        }

        return model;
    }
}

#endif