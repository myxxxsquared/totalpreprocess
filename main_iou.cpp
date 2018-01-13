
#include "totalpreprocess.hpp"
#include "cnpy/cnpy.h"
#include "process.hpp"
#include "data.hpp"

// constexpr int NUMSCALES = 6;
// extern const int SCALES[];

// totalpreprocess iou height width result polygon

int main_iou(int argc, char* argv[])
{
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    const char* result_file = argv[4];
    const char* polygon_file = argv[5];

    cv::Mat mat1, mat2;
    mat1.create(height, width, CV_8UC1);
    mat1.setTo(cv::Scalar(0));
    mat2.create(height, width, CV_8UC1);
    mat2.setTo(cv::Scalar(0));
    MatTarget targets[NUMSCALES];

    char buffer[1024];
    for (int i = 0; i < NUMSCALES; ++i)
    {
        sprintf(buffer, "%s_%d.npy", result_file, i);
        auto data = cnpy::npy_load(buffer);
        targets[i].create(data.shape[0], data.shape[1]);
        if( data.shape[0] != height / SCALES[i]
            || data.shape[1] != width / SCALES[i]
            || data.num_bytes() != data.shape[0]*data.shape[1]*sizeof(VecTarget))
            throw "invaild size";
        memcpy(targets[i].data(), data.data<double>(), data.num_bytes());
    }

    for (int i = NUMSCALES - 1; i >= 0; --i)
    {
        auto &target = targets[i];
        for (int j = 0; j < target.rows(); ++j)
        {
            for (int k = 0; k < target.cols(); ++k)
            {
                if (target.at(j, k).vaild > 0.5)
                {
                    auto& t = target.at(j, k);
                    int scale = SCALES[i];
                    double zeropoint = (scale - 1) / 2.0;
                    double x = k * scale + zeropoint;
                    double y = j * scale + zeropoint;
                    double center_x = x + t.deltax * scale;
                    double center_y = y + t.deltay * scale;
                    double xdire_x = cos(t.angle) * t.width * scale;
                    double xdire_y = sin(t.angle) * t.width * scale;
                    double ydire_x = -sin(t.angle) * t.height * scale;
                    double ydire_y = cos(t.angle) * t.height * scale;

                    std::vector<cv::Point2i> pts;
                    pts.resize(4);
                    pts[0].x = center_x + xdire_x + ydire_x;
                    pts[0].y = center_y + xdire_y + ydire_y;
                    pts[1].x = center_x - xdire_x + ydire_x;
                    pts[1].y = center_y - xdire_y + ydire_y;
                    pts[2].x = center_x - xdire_x - ydire_x;
                    pts[2].y = center_y - xdire_y - ydire_y;
                    pts[3].x = center_x + xdire_x - ydire_x;
                    pts[3].y = center_y + xdire_y - ydire_y;

                    cv::fillConvexPoly(mat1, pts, cv::Scalar(1));
                }
            }
        }
    }

    std::ifstream ifs{polygon_file};
    std::vector<Polygon> polygons = loadfile(ifs);
    for(auto& polygon : polygons)
    {
        std::vector<cv::Point2i> pts;
        for(auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
            pts.emplace_back((int)it->x(), (int)it->y());
        cv::fillConvexPoly(mat2, pts, cv::Scalar(1));
    }

    int tt=0, tf=0, ft=0, ff=0;

    for(int i = 0; i < mat1.rows; ++i)
    {
        for(int j = 0; j < mat1.cols; ++j)
        {
            uchar v1 = mat1.at<uchar>(i, j);
            uchar v2 = mat2.at<uchar>(i, j);

            if(v1 == 0 && v2 == 0)
                ff++;
            if(v1 == 1 && v2 == 0)
                tf++;
            if(v1 == 0 && v2 == 1)
                ft++;
            if(v1 == 1 && v2 == 1)
                tt++;
        }
    }

    fprintf(stderr, "persion\trecall\tiou\n");
    printf("%lf\t%lf\t%lf\n", (double)tt / (tt+tf), (double)tt / (tt+ft), (double)tt / (tt+ft+tf));

    return 0;
}
