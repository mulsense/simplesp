#include "simplesp.h"

using namespace sp;


int main() {

    {
        //--------------------------------------------------------------------------------
        // principal component analysis (2D)
        //--------------------------------------------------------------------------------

        Mem1<Vec2> data;

        // generate data
        {
            const double angle = 30.0 * SP_PI / 180.0;

            Mat mat(2, 2);
            mat(0, 0) = +::cos(angle);
            mat(0, 1) = -::sin(angle);

            mat(1, 0) = +::sin(angle);
            mat(1, 1) = +::cos(angle);

            for (int i = 0; i < 200; i++) {
                const Vec2 vec = mat * randgVec2(20.0, 5.0) + getVec2(50.0, 50.0);
                data.push(vec);
            }
        }
    
        {
            const Vec2 m = mean(data);

            Mat mat(data.size(), 2);
            for (int i = 0; i < data.size(); i++) {
                mat(i, 0) = data[i].x - m.x;
                mat(i, 1) = data[i].y - m.y;
            }

            Mat eigVec, eigVal;
            eigMat(eigVec, eigVal, covMat(mat) / data.size(), false);


            printf("eigVec\n");
            print(eigVec);

            printf("eigVal\n");
            print(eigVal);

            printf("first principal component\n");
            printf("%+.3lf, %+.3lf\n", eigVec(0, 0), eigVec(1, 0));
            printf("second principal component\n");
            printf("%+.3lf, %+.3lf\n", eigVec(0, 1), eigVec(1, 1));

            printf("\n\n");
        }
    }

    return 0;
}
