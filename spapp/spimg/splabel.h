//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_LABEL_H__
#define __SP_LABEL_H__

#include "spcore/spcore.h"

namespace sp{

    //--------------------------------------------------------------------------------
    // labeling
    //--------------------------------------------------------------------------------

    SP_CPUFUNC int labeling(Mem2<int> &map, const Mem2<u08> &bin, const bool near8 = false){
        
        const int step = bin.dsize[0];

        map.resize(bin.dsize);

        Mem1<int> table;
        table.reserve(bin.size());

        const Rect2 rect(bin.dsize);

        const int linkNum = (near8 == true) ? 4 : 2;
        const int link[][2] = { { -1, 0 }, { 0, -1 }, { -1, -1 }, { +1, -1 } };

        const u08 *pBin = bin.ptr;
        int *pMap = map.ptr;
        int *pTable = table.ptr;

        for (int v = 0; v < bin.dsize[1]; v++){
            for (int u = 0; u < bin.dsize[0]; u++){
                pMap[v * step + u] = -1;
                if (pBin[v * step + u] == 0) continue;

                int crntLabel = table.size();

                int *pTmp[4] = { 0 };

                // check min label
                for (int i = 0; i < linkNum; i++){
                    const int ru = u + link[i][0];
                    const int rv = v + link[i][1];

                    if (rect.contains(ru, rv) == false) continue;
                    if (pBin[rv * step + ru] == 0) continue;

                    const int refLabel = table[pMap[rv * step + ru]];
                    pTmp[i] = &pMap[rv * step + ru];
                    if (refLabel < crntLabel){
                        crntLabel = refLabel;
                    }
                }

                if (crntLabel == table.size()){
                    table.push(crntLabel);
                }
                else{
                    for (int i = 0; i < linkNum; i++){
                        if (pTmp[i] != NULL) {
                            int p = *pTmp[i];
                            while (p != pTable[p]) {
                                const int prev = pTable[p];
                                pTable[p] = crntLabel;
                                p = prev;
                            }
                            pTable[p] = crntLabel;
                        }
                    }
                }

                pMap[v * step + u] = crntLabel;
            }
        }

        const int tableNum = table.size();

        // update table
        for (int i = 0; i < tableNum; i++){
            int p = i;
            while (pTable[p] != p){
                p = pTable[p];
            }
            pTable[i] = p;
        }

        int labelNum = 0;

        // update label
        int maxv = -1;
        for (int i = 0; i < tableNum; i++){
            if (pTable[i] > maxv){
                maxv = pTable[i];

                for (int j = i; j < tableNum; j++){
                    if (pTable[j] == maxv){
                        pTable[j] = labelNum;
                    }
                }
                labelNum++;
            }
        }

        // update map
        for (int i = 0; i < bin.size(); i++){
            int &id = pMap[i];
            if (id < 0) continue;

            id = pTable[id];
        }

        return labelNum;
    }

    SP_CPUFUNC Mem1<int> getLabelCount(const Mem2<int> &map) {
        const int labelNum = round(max(map) + 1);
        //const int step = map.dsize[0];

        Mem1<int> dst(labelNum);
        dst.zero();

        const int *pMap = map.ptr;
        int *pDst = dst.ptr;

        for (int v = 0; v < map.dsize[1]; v++) {
            for (int u = 0; u < map.dsize[0]; u++) {
                const int id = *pMap++;
                if (id < 0) continue;
                pDst[id]++;
            }
        }
        return dst;
    }

    SP_CPUFUNC Mem1<Rect2> getLabelRect(const Mem2<int> &map){
        const int labelNum = round(max(map) + 1);
        const int step = map.dsize[0];

        Mem1<Rect2> dst(labelNum);
        dst.zero();

        const int *pMap = map.ptr;
        Rect2 *pDst = dst.ptr;

        for (int v = 0; v < map.dsize[1]; v++){
            for (int u = 0; u < map.dsize[0]; u++){
                const int id = *pMap++;
                if (id < 0) continue;
                Rect2 &rect = pDst[id];
                if (rect.dsize[0] == 0 || rect.dsize[1] == 0) {
                    rect = Rect2(u, v, 1, 1);
                }
                else {
                    const int ru = rect.dbase[0];
                    const int rv = rect.dbase[1];
                    const int rw = rect.dsize[0];
                    const int rh = rect.dsize[1];

                    rect.dbase[0] = std::min(u, ru);
                    rect.dsize[0] = std::max(u + 1, ru + rw) - rect.dbase[0];

                    rect.dbase[1] = std::min(v, rv);
                    rect.dsize[1] = std::max(v + 1, rv + rh) - rect.dbase[1];
                }
            }
        }
        return dst;
    }

    SP_CPUFUNC Mem1<Mem1<Vec2> > getLabelContour(const Mem2<int> &map, const bool onPixel = true, const bool useImgFrame = true){

        const Mem1<Rect2> rects = getLabelRect(map);

        // 8 nears clockwise search
        const int order8[8][2] = {
            { -1, -1 }, { 0, -1 }, { +1, -1 }, { +1, 0 },
            { +1, +1 }, { 0, +1 }, { -1, +1 }, { -1, 0 }
        };
        const int start8[3][3] = {
            { 0, 1, 2 },
            { 7, 0, 3 },
            { 6, 5, 4 }
        };

        // 4 nears clockwise search
        const int order4[8][2] = {
            { 0, -1 }, { +1, 0 }, { 0, +1 }, { -1, 0 }
        };
        const int start4[3][3] = {
            { 0, 0, 0 },
            { 3, 0, 1 },
            { 0, 2, 0 }
        };

        const int(*order)[2] = (onPixel == true) ? order8 : order4;
        const int(*start)[3] = (onPixel == true) ? start8 : start4;

        Mem1< Mem1<Vec2> > dst(rects.size());

        for (int i = 0; i < dst.size(); i++){
            const Rect2 rect = rects[i];

            if (useImgFrame == false){
                if ((Rect2(map.dsize) - 1).contains(rect) == false) continue;
            }

            int sx = 0;
            int sy = 0;
            for (int y = rect.dbase[1]; y < rect.dbase[1] + rect.dsize[1]; y++){
                for (int x = rect.dbase[0]; x < rect.dbase[0] + rect.dsize[0]; x++){
                    if (map(x, y) == i){
                        sx = x;
                        sy = y;
                        goto _exit;
                    }
                }
            }
        _exit:;

            int vec[2] = { +1, 0 };
            int cx = sx;
            int cy = sy;

            int cnt = 0;
            while (1){

                if (onPixel == true) {
                    const int s = (start[vec[1] + 1][vec[0] + 1] + 8 - 2) % 8;
                    
                    for (int j = 0; j < 8; j++) {
                        const int t = (s + j) % 8;
                        const int x = cx + order[t][0];
                        const int y = cy + order[t][1];
                        if (rect.contains(x, y) == false) continue;

                        if (map(x, y) == i) {
                            vec[0] = x - cx;
                            vec[1] = y - cy;

                            cx = x;
                            cy = y;
                            break;
                        }
                    }
                    dst[i].push(Vec2(cx, cy));
                }
                else {
                    const int s = (start[vec[1] + 1][vec[0] + 1] + 4 - 1) % 4;
                   
                    Rect2 trect = rect;
                    trect.dsize[0]++;
                    trect.dsize[1]++;

                    // clockwise
                    bool list[4] = { map(cx - 1, cy - 1) == i, map(cx, cy - 1) == i, map(cx, cy) == i, map(cx - 1, cy) == i };
                  
                    if (cx == 0) {
                        list[0] = false;
                        list[3] = false;
                    }
                    if (cy == 0) {
                        list[0] = false;
                        list[1] = false;
                    }
                    if (cx == map.dsize[0]) {
                        list[1] = false;
                        list[2] = false;
                    }
                    if (cy == map.dsize[1]) {
                        list[2] = false;
                        list[3] = false;
                    }

                    bool edge[4] = { list[0] != list[1], list[1] != list[2], list[2] != list[3], list[3] != list[0] };

                    for (int j = 0; j < 4; j++) {
                        const int t = (s + j) % 4;
                        const int x = cx + order[t][0];
                        const int y = cy + order[t][1];
                        if (trect.contains(x, y) == false) continue;

                        if (edge[t] == true) {
                            vec[0] = x - cx;
                            vec[1] = y - cy;

                            cx = x;
                            cy = y;

                            break;
                        }
                    }

                    dst[i].push(Vec2(cx - 0.5, cy - 0.5));
                }

                if (cx == sx && cy == sy){
                    break;
                }
            }
        }

        return dst;
    }

}

#endif