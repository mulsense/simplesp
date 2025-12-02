//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_BASE_H__
#define __SP_BASE_H__

#include "spcore/spcom.h"

namespace sp{

    //--------------------------------------------------------------------------------
    // util
    //--------------------------------------------------------------------------------

    // sqrt(a * a + b * b) without destructive underflow or overflow
    SP_GENFUNC SP_REAL pythag(const double a, const double b) { 
        const double x = fabs(a);
        const double y = fabs(b);

        double ret = 0.0;
        if (x > y) {
            ret = x * sqrt(1.0 + (y / x) * (y / x));
        }
        else {
            ret = (y == 0.0) ? 0.0 : y * sqrt(1.0 + (x / y) * (x / y));
        }
        return static_cast<SP_REAL>(ret);
    }

    // combination
    SP_GENFUNC int nCk(const int n, const int k) {
        int ret = 1;
        for (int i = 1; i <= k; i++) {
            ret = ret * (n - i + 1) / i;
        }
        return ret;
    }
    
    //--------------------------------------------------------------------------------
    // random
    //--------------------------------------------------------------------------------
    
    static unsigned int _randseed = 0;
    SP_GENFUNC unsigned int _snext(const unsigned int seed) {
        unsigned int s = seed + 1;
        s ^= (s << 13);
        s ^= (s >> 17);
        s ^= (s << 15);
        return s;
    }

    SP_GENFUNC void srand(const int seed) {
        _randseed = static_cast<unsigned int>(seed);
    }

    SP_GENFUNC int rand() {
        _randseed = _snext(_randseed);
        return static_cast<int>(_randseed >> 1);
    }

    SP_GENFUNC int rand(const int seed) {
        const unsigned int s = _snext(static_cast<unsigned int>(seed));
        return static_cast<int>(s >> 1);
    }

    // get random uniform (-1.0, 1.0)
    SP_GENFUNC SP_REAL randu(const int seed) {
        const unsigned int s = static_cast<unsigned int>(seed);

        const int maxv = 20000;
        const double a = static_cast<double>(rand(s) % (maxv + 1) + 1) / (maxv + 2);
        const double u = 2.0 * a - 1.0;
        return static_cast<SP_REAL>(u);
    }

    // get random uniform (-1.0, 1.0)
    SP_GENFUNC SP_REAL randu() {
        const unsigned s = _randseed;
        _randseed = _snext(s);
        return randu(s);
    }

    // get random gauss
    SP_GENFUNC SP_REAL randg(const int seed) {
        const unsigned s0 = static_cast<unsigned int>(seed);
        const unsigned s1 = _snext(s0);

        const int maxv = 20000;
        const double a = static_cast<double>(rand(s0) % (maxv + 1) + 1) / (maxv + 2);
        const double b = static_cast<double>(rand(s1) % (maxv + 1) + 1) / (maxv + 2);
        const double g = sqrt(-2.0 * log(a)) * sin(2.0 * SP_PI * b);
        return static_cast<SP_REAL>(g);
    }

    // get random gauss
    SP_GENFUNC SP_REAL randg() {
        const unsigned s = _randseed;
        _randseed = _snext(s);
        return randg(s);
    }


    //--------------------------------------------------------------------------------
    // byte order
    //--------------------------------------------------------------------------------

    // get byte order
    SP_GENFUNC ByteOrder getByteOrder() {
        const int v = 1;
        return (*(const char*)&v == 1) ? ByteOrder::LittleEndian : ByteOrder::BigEndian;
    }

    // reverse byte order
    template <typename TYPE> 
    SP_GENFUNC void revByteOrder(TYPE *ptr, const int size) {
        const int n = sizeof(TYPE);
        if (sizeof(TYPE) == 1) return;

        for (int i = 0; i < size; i++) {
            u08 *tmp = (u08*)&ptr[i];

            for (int j = 0; j < n / 2; j++) {
                swap(tmp[j], tmp[n - j - 1]);
            }
        }
    }

    // get ordered value
    template<typename TYPE>
    SP_GENFUNC TYPE ordered(const TYPE &val, const ByteOrder order) {
        TYPE dst = val;
        if (getByteOrder() != order) revByteOrder(&dst, 1);
        return dst;
    }


    //--------------------------------------------------------------------------------
    // bit
    //--------------------------------------------------------------------------------

    static unsigned int SP_BITS_TABLE[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
    };

    SP_GENFUNC int cntBit(const u08 byte) {
        return SP_BITS_TABLE[byte];
    }

    SP_GENFUNC int cntBit(const u08 byte0, const u08 byte1) {
        return 8 - cntBit(byte0 ^ byte1);
    }

    SP_GENFUNC int cntBit(const u08 *bytes0, const u08 *bytes1, const int bsize) {
        int cnt = 0;
        for (int i = 0; i < bsize; i++) {
            cnt += cntBit(bytes0[i], bytes1[i]);
        }
        return cnt;
    }

    SP_GENFUNC void setBit(u08 *byte, const int p, const u08 b) {
        const u08 mask = 0x01 << p;
        *byte = (b != 0) ? *byte | mask : *byte & ~mask;
    }
  
    SP_GENFUNC u08 getBit(const u08 *byte, const int p) {
        const u08 mask = 0x01 << p;
        return (*byte & mask) ? 1 : 0;
    }

    template<typename TYPE>
    SP_GENFUNC void cnvBit(u08 *bytes, const int bsize, const TYPE *src, const int ssize, const TYPE thresh) {

        for (int i = 0; i < bsize; i++) {
            bytes[i] = 0;
        }

        for (int i = 0; i < ssize; i++) {
            if (src[i] >= thresh) {
                setBit(&bytes[i / 8], i % 8, 1);
            }
        }
    }

    //--------------------------------------------------------------------------------
    // access vector
    //--------------------------------------------------------------------------------

    SP_GENFUNC SP_REAL& acsv(Vec2 &vec, const int i) {
        return ((SP_REAL*)&vec)[i];
    }
    SP_GENFUNC const SP_REAL& acsv(const Vec2 &vec, const int i) {
        return ((SP_REAL*)&vec)[i];
    }

    SP_GENFUNC SP_REAL& acsv(Vec3 &vec, const int i) {
        return ((SP_REAL*)&vec)[i];
    }
    SP_GENFUNC const SP_REAL& acsv(const Vec3 &vec, const int i) {
        return ((SP_REAL*)&vec)[i];
    }

    
    //--------------------------------------------------------------------------------
    // access ptr 1d (multi channel)
    //--------------------------------------------------------------------------------

    SP_GENFUNC const int acsid1(const int *dsize, const int d0) {
        const int id0 = max(0, min(dsize[0] - 1, d0));
        return id0;
    }

    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC ELEM& acs1(ExPtr<TYPE> &src, const int d0, const int c = 0) {
        const int id = acsid1(src.dsize, d0);
        return reinterpret_cast<ELEM*>(&src.ptr[id])[c];
    }


    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC const ELEM& acs1(const ExPtr<TYPE> &src, const int d0, const int c = 0) {
        return acs1<TYPE, ELEM>(*const_cast<ExPtr<TYPE>*>(&src), d0, c);
    }


    SP_GENFUNC const int lacsid1(const int *dsize, const int d0) {
        int id0 = d0;
        while (id0 < 0) id0 += dsize[0];
        return id0 % dsize[0];
    }

    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC ELEM& lacs1(ExPtr<TYPE> &src, const int d0, const int c = 0) {
        const int id = lacsid1(src.dsize, d0);
        return reinterpret_cast<ELEM*>(&src.ptr[id])[c];
    }

    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC const ELEM& lacs1(const ExPtr<TYPE> &src, const int d0, const int c = 0) {
        return lacs1<TYPE, ELEM>(*const_cast<ExPtr<TYPE>*>(&src), d0, c);
    }

    //--------------------------------------------------------------------------------
    // access ptr 2d (multi channel)
    //--------------------------------------------------------------------------------

    SP_GENFUNC int acsid2(const int *dsize, const int d0, const int d1, const int c = 0) {
        const int id0 = max(0, min(dsize[0] - 1, d0));
        const int id1 = max(0, min(dsize[1] - 1, d1));
        return id1 * dsize[0] + id0;
    }

    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC ELEM& acs2(ExPtr<TYPE> &src, const int d0, const int d1, const int c = 0) {
        const int id = acsid2(src.dsize, d0, d1);
        return reinterpret_cast<ELEM*>(&src.ptr[id])[c];
    }


    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC const ELEM& acs2(const ExPtr<TYPE> &src, const int d0, const int d1, const int c = 0) {
        return acs2<TYPE, ELEM>(*const_cast<ExPtr<TYPE>*>(&src), d0, d1, c);
    }


    //--------------------------------------------------------------------------------
    // access ptr 3d (multi channel)
    //--------------------------------------------------------------------------------

    SP_GENFUNC int acsid3(const int *dsize, const int d0, const int d1, const int d2, const int c = 0) {
        const int id0 = max(0, min(dsize[0] - 1, d0));
        const int id1 = max(0, min(dsize[1] - 1, d1));
        const int id2 = max(0, min(dsize[2] - 1, d2));
        return (id2 * dsize[1] + id1) * dsize[0] + id0;
    }

    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC ELEM& acs3(ExPtr<TYPE> &src, const int d0, const int d1, const int d2, const int c = 0) {
        const int id = acsid3(src.dsize, d0, d1, d2);
        return reinterpret_cast<ELEM*>(&src.ptr[id])[c];
    }


    template<typename TYPE, typename ELEM = TYPE>
    SP_GENFUNC const ELEM& acs3(const ExPtr<TYPE> &src, const int d0, const int d1, const int d2, const int c = 0) {
        return acs3<TYPE, ELEM>(*const_cast<ExPtr<TYPE>*>(&src), d0, d1, d2, c);
    }



    //--------------------------------------------------------------------------------
    // access ptr 2d color
    //--------------------------------------------------------------------------------

    SP_GENFUNC Col3& acsc(ExPtr<Col3> &src, const int d0, const int d1) {
        const int id = acsid2(src.dsize, d0, d1);
        return src.ptr[id];
    }

    SP_GENFUNC const Col3& acsc(const ExPtr<Col3> &src, const int d0, const int d1) {
        return acsc(*const_cast<ExPtr<Col3>*>(&src), d0, d1);
    }

}

#endif