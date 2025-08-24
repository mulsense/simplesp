#include "simplesp.h"

using namespace sp;

int main(){

    print(Vec2(1.0, 2.0));
    
    print(Vec3(1.0, 2.0, 3.0));

    print(getRect2(0, 0, 640, 480));

    print(getVecPD2(Vec2(1.0, 2.0), Vec2(3.0, 4.0)));

    print(getMesh3(Vec3(1.0, 2.0, 3.0), Vec3(4.0, 5.0, 6.0), Vec3(7.0, 8.0, 9.0)));

    Mem1<Vec2> vecs;
    for (int i = 0; i < 10; i++) {
        vecs.push(randuVec2(1.0, 1.0));
    }
    print(vecs);

    return 0;
}