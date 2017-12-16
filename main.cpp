
#include "totalpreprocess.hpp"

#include "data.hpp"
#include "skeleton.hpp"

using std::vector;
using std::ifstream;
using cv::Mat;
using cv::Scalar;
using cv::line;
using cv::imshow;
using cv::waitKey;

#define MAKE_X(x) (int)(x)

int main(int argc, char* argv[])
{
    // const char* filename = "poly_gt_img.mat.output";
    // ifstream ifs{filename};

    // Mat image;
    // //image.create(700, 1000, CV_8UC3);
    // //image = Scalar(0, 0, 0);
    // image = cv::imread("img.jpg");

    // vector<vector<cv::Point2d>> polygons = loadfile(ifs);
    // for(auto& polygon : polygons)
    // {
    //     for(int i = 0; i < polygon.size(); ++i)
    //     {
    //         auto& pti = polygon[i];
    //         auto& ptj = polygon[(i+1)%polygon.size()];
    //         cv::line(image,
    //                 cv::Point(MAKE_X(pti.x), MAKE_X(pti.y)),
    //                 cv::Point(MAKE_X(ptj.x), MAKE_X(ptj.y)),
    //                 Scalar(255, 255, 0));
    //     }
    //     vector<vector<cv::Point2d>> lines = process_to_skeleton(polygon);
    //     for(auto& line: lines)
    //     {
    //         for(int i = 1; i < line.size(); ++i)
    //         {
    //             auto& pti = line[i-1];
    //             auto& ptj = line[i];
    //             cv::line(image,
    //                 cv::Point(MAKE_X(pti.x), MAKE_X(pti.y)),
    //                 cv::Point(MAKE_X(ptj.x), MAKE_X(ptj.y)),
    //                 Scalar(0, 255, 255));
    //         }
    //     }
    // }

    // imshow("tp", image);
    // while(1)
    //     waitKey();
}
