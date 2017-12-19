
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

#define MAKE_X(x) ((int)(x))

int main_viewer(int argc, char* argv[])
{
    // totalpreprocess viewer height height polygon feature image [image_output]

    if(argc != 7 && argc != 8)
        throw "useage: totalpreprocess viewer width height polygon feature image [image_output]";
    
    const int height = atoi(argv[2]);
    const int width = atoi(argv[3]);
    const char* const polygon_file = argv[4];
    const char* const feature_file = argv[5];
    const char* const image_file = argv[6];
    const char* const output_file = argc == 8 ? argv[7] : nullptr;

    Mat image = cv::imread(image_file);
    Preprocess process;
    process.process(height, width, polygon_file);

    char buffer[1024];
    for(int i = 0; i < NUMSCALES; ++i)
    {
        const int scale = SCALES[i];
        const int sheight = height / scale;
        const int swidth = width / scale;

        sprintf(buffer, "%s_%d.npy", feature_file, i);
        auto data = cnpy::npy_load(buffer);
        memcpy(process.targets[i].data(), data.data<double>(), sheight*swidth*sizeof(VecTarget));
    }

    for(auto& polygon : process.polygons)
    {
        for(int i = 0; i < (int)polygon.bounding.size(); ++i)
        {
            auto& pti = polygon.bounding[i];
            auto& ptj = polygon.bounding[(i+1)%polygon.bounding.size()];
            cv::line(image,
                    cv::Point(MAKE_X(pti.x()), MAKE_X(pti.y())),
                    cv::Point(MAKE_X(ptj.x()), MAKE_X(ptj.y())),
                    Scalar(255, 255, 0));
        }
        for(auto& line: polygon.segments)
        {
            for(int i = 0; i < (int)line.size(); ++i)
            {
                auto& pti = line[i].segment.source();
                auto& ptj = line[i].segment.target();
                cv::circle(image, cv::Point(MAKE_X(pti.x()), MAKE_X(pti.y())), 2, Scalar(255, 255, 255), -1);
                cv::circle(image, cv::Point(MAKE_X(ptj.x()), MAKE_X(ptj.y())), 2, Scalar(255, 255, 255), -1);
                cv::line(image,
                    cv::Point(MAKE_X(pti.x()), MAKE_X(pti.y())),
                    cv::Point(MAKE_X(ptj.x()), MAKE_X(ptj.y())),
                    Scalar(0, 255, 255));
                // cv::line(image,
                //     cv::Point(MAKE_X(pti.x()), MAKE_X(pti.y())),
                //     cv::Point(MAKE_X(pti.x()+50*cos(line[i].angle.angle)), MAKE_X(pti.y()+50*sin(line[i].angle.angle))),
                //     Scalar(0, 0, 0));
                // cv::line(image,
                //     cv::Point(MAKE_X(pti.x()), MAKE_X(pti.y())),
                //     cv::Point(MAKE_X(pti.x()+50*cos(line[i].left_angle.angle)), MAKE_X(pti.y()+50*sin(line[i].left_angle.angle))),
                //     Scalar(255, 255, 255));
                // cv::line(image,
                //     cv::Point(MAKE_X(ptj.x()), MAKE_X(ptj.y())),
                //     cv::Point(MAKE_X(ptj.x()-50*cos(line[i].right_angle.angle)), MAKE_X(ptj.y()-50*sin(line[i].right_angle.angle))),
                //     Scalar(255, 255, 255));
            }
        }
    }

    for(int i = 0; i < NUMSCALES; ++i)
    {
        const int scale = SCALES[i];
        const int sheight = height / SCALES[i];
        const int swidth = width / SCALES[i];

        const double zeropoint = (scale - 1) / 2.0;

        for (int j = 0; j < sheight; ++j)
        {
            double y = j * scale + zeropoint;

            for (int k = 0; k < swidth; ++k)
            {
                double x = k * scale + zeropoint;
                auto &target = process.targets[i].at(j, k);

                if(target.vaild > 0.5)
                {
                    Point2d center{x+target.deltax*scale, y+target.deltay*scale};
                    Point2d xdire{cos(target.angle)*target.width*scale, sin(target.angle)*target.width*scale};
                    Point2d ydire{-sin(target.angle)*target.height*scale, cos(target.angle)*target.height*scale};
                    // if(target.inlayer[5] > 0.5)
                    // {
                    //     cv::circle(image, Point2d(x, y), 3, cv::Scalar(255, 0, 0), -1);
                    // }
                    // else
                    // {
                    //     cv::circle(image, Point2d(x, y), 3, cv::Scalar(0, 0, 0), -1);
                    // }
                    cv::circle(image, cv::Point2d(x, y), 2, cv::Scalar(0, 255, 0), -1);
                    cv::circle(image, center, 2, cv::Scalar(255, 0, 0), -1);
                    Point2d p1 = center + xdire + ydire;
                    Point2d p2 = center - xdire + ydire;
                    Point2d p3 = center - xdire - ydire;
                    Point2d p4 = center + xdire - ydire;
                    cv::line(image, center, center+xdire, cv::Scalar(0, 0, 255));
                    cv::line(image, p1, p2, cv::Scalar(0));
                    cv::line(image, p2, p3, cv::Scalar(0));
                    cv::line(image, p3, p4, cv::Scalar(0));
                    cv::line(image, p4, p1, cv::Scalar(0));
                }
            }
        }
    }

    if(output_file)
    {
       cv::imwrite(output_file, image); 
    }
    else
    {
        imshow("test", image);
        waitKey();
    }

    return 0;
}
