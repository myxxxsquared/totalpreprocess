
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

#define MAKE_X(x) (int)(x)

int main(int argc, char* argv[])
{
    Preprocess process;
    process.process(512, 512, "../testdata/poly.output");
    Mat image, tmpimage;
    tmpimage = cv::imread("../testdata/img.jpg");
    image.create(512, 512, tmpimage.type());
    cv::Rect rect{0, 0, tmpimage.cols, tmpimage.rows};
    tmpimage.copyTo(image(rect));


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
        const int sheight = 512 / SCALES[i];
        const int swidth = 512 / SCALES[i];

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
                    Point2d center{x+target.deltax, y+target.deltay};
                    Point2d xdire{cos(target.angle)*target.width, sin(target.angle)*target.width};
                    Point2d ydire{-sin(target.angle)*target.height, cos(target.angle)*target.height};
                    cv::circle(image, cv::Point2d(x, y), 1, cv::Scalar(0, 255, 0), -1);
                    cv::circle(image, center, 1, cv::Scalar(255, 0, 0), -1);
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

    cv::imwrite("../testdata/output.png", image);

    char buffer[256];
    for(int i = 0; i < NUMSCALES; ++i)
    {
        const int scale = SCALES[i];
        const int sheight = 512 / SCALES[i];
        const int swidth = 512 / SCALES[i];

        sprintf(buffer, "../testdata/output_%d.npy", i);
        cnpy::npy_save(buffer, (double*)process.targets[i].data(),
            {sheight, swidth, 19});
    }

    // imshow("text", image);
    // waitKey();
}
