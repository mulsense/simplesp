//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_VERTEX_H__
#define __SP_VERTEX_H__

#include "spcore/spcore.h"

namespace sp{

    //--------------------------------------------------------------------------------
    // 3d
    //--------------------------------------------------------------------------------

    SP_CPUFUNC bool calcVertexNormal(Mem1<Vec3> &nrms, const Mem1<Mesh3> &meshes) {
        nrms.resize(meshes.size() * 3);
        nrms.zero();

        KdTree<SP_REAL> kdtree(3);

        Mem1<Vec3> mnrms(meshes.size());
        for (int i = 0; i < meshes.size(); i++) {
            kdtree.addData(&meshes[i].pos[0]);
            kdtree.addData(&meshes[i].pos[1]);
            kdtree.addData(&meshes[i].pos[2]);
            mnrms[i] = meshes[i].normal();
        }
        kdtree.makeTree();

        for (int i = 0; i < meshes.size(); i++) {
            for (int j = 0; j < 3; j++) {
                const Mem1<int> index = kdtree.search(&meshes[i].pos[j], 0.1);

                Vec3 nrm = Vec3(0.0, 0.0, 0.0);
                for (int k = 0; k < index.size(); k++) {
                    const int s = index[k] / 3;
                    nrm += mnrms[s];
                }
                nrms[i * 3 + j] = nrm.unit();
            }
        }

        return true;
    }


    template <typename TYPE, typename ELEM>
    SP_CPUFUNC void indexingKd(Mem1<TYPE> &dst, Mem1<int> &idxs, const Mem1<TYPE> &src) {
        dst.clear();
        idxs.clear();
        if (src.size() == 0) return;

        const int dim = sizeof(TYPE) / sizeof(ELEM);
        const int num = src.size() * dim;

        KdTree<ELEM> kdtree(dim);

        for (int i = 0; i < src.size(); i++) {
            kdtree.addData(&src[i]);
        }
        kdtree.makeTree();

        dst.clear();
        idxs.resize(src.size());

        Mem1<int> refs(src.size());
        for (int i = 0; i < src.size(); i++) {
            refs[i] = -1;

            const Mem1<int> index = kdtree.search(&src[i], 0.0001);
            int minv = num;
            for (int k = 0; k < index.size(); k++) {
                minv = min(minv, index[k]);
            }

            if (i == minv) {
                refs[i] = dst.size();
                dst.push(src[i]);
            }
            idxs[i] = refs[minv];
        }

    }

    template <typename TYPE>
    SP_CPUFUNC void indexing(Mem1<TYPE> &dst, Mem1<int> &idxs, const Mem1<TYPE> &src) {
        dst.clear();
        idxs.clear();
        if (src.size() == 0) return;

        dst.clear();
        idxs.resize(src.size());

        for (int i = 0; i < src.size(); i++) {
            int ref = -1;
            for (int j = 0; j < dst.size(); j++) {
                if (src[i] == dst[j]) {
                    ref = j;
                }
            }
            if (ref >= 0) {
                idxs[i] = ref;
            }
            else {
                idxs[i] = dst.size();
                dst.push(src[i]);
            }
        }

    }
}
#endif