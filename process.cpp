
#include "totalpreprocess.hpp"
#include "skeleton.hpp"
#include "data.hpp"
#include "angle.hpp"

using std::vector;
using cv::Point2d;


class Preprocess
{
public:
    void load_file(int height, int weidth, const char* filename)
    {
        std::ifstream ifs{filename};
        vector<Polygon> polygons = loadfile(ifs);
    }
};
