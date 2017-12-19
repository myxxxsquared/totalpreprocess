
#include "totalpreprocess.hpp"

#include "data.hpp"
#include "skeleton.hpp"
#include "process.hpp"
#include "angle.hpp"

#include "cnpy.h"

using std::vector;
using std::ifstream;
using cv::Mat;
using cv::Scalar;
using cv::line;
using cv::imshow;
using cv::waitKey;
using cv::Point2d;
using cv::Vec2d;

int main_generator(int argc, char* argv[])
{
    if(argc != 6)
        throw "useage: totalpreprocess generator height width input output";

    const int height = atoi(argv[2]);
    const int width = atoi(argv[3]);
    const char* const input_file = argv[4];
    const char* const output_file = argv[5];

    Preprocess process;
    process.process(height, width, input_file);

    char buffer[1024];
    for(int i = 0; i < NUMSCALES; ++i)
    {
        const int scale = SCALES[i];
        const int sheight = height / scale;
        const int swidth = width / scale;

        sprintf(buffer, "%s_%d.npy", output_file, i);
        cnpy::npy_save(buffer, (double*)process.targets[i].data(),
            {(unsigned int)sheight, (unsigned int)swidth, 19});
    }

    return 0;
}
