
#include "totalpreprocess.hpp"
#include "skeleton.hpp"
#include "data.hpp"
#include "angle.hpp"
#include "polygonpreprocess.hpp"

using std::vector;
using cv::Point2d;

const int SCALES[] = {8, 16, 32, 64, 128, 256};
const int NUMSCALES = 6;

struct VecTarget
{
    double vaild;
    double angle;
    double deltax, deltay;
    double width, height;
    double curvature;
    double inlayer[8];
    double crosslayer[4];
};

//typedef cv::Vec<float, 18> VecTarget;
typedef cv::Mat_<VecTarget> MatTarget;
typedef cv::Mat_<int> MatIndexs;

/*
 0 vaild
 1 angle
 2 deltax
 3 deltay
 4 width
 5 height
 6 curvature
 7-14 inlayer
 15-18 crosslayer
*/

#define locsame(nj, nk) (nj>=0 && nj < sheight \
            && nk >= 0 && nk < swidth \
            && target.at<VecTarget>(nj, nk).vaild > 0.5 \
            && index.at<int>(j, k) == index.at<int>(nj, nk))
#define crosssame(nj, nk) (target2.at<VecTarget>(nj, nk).vaild > 0.5 \
            && index.at<int>(j, k) == index2.at<int>(nj, nk))
#define dolocsame(m, nj, nk) target.at<VecTarget>(j, k).inlayer[m] = locsame(nj, nk) ? 1.0 : 0.0
#define docrosssame(m, nj, nk) target.at<VecTarget>(j, k).crosslayer[m] = crosssame(nj, nk) ? 1.0 : 0.0

class Preprocess
{
  public:
    std::vector<process_polygon> polygons;
    int height;
    int width;
    MatTarget targets[NUMSCALES];
    MatIndexs indexs[NUMSCALES];

    void process(int height, int width, const char *filename)
    {
        if(height % SCALES[NUMSCALES-1] != 0 || width % SCALES[NUMSCALES-1] != 0 )
            throw "size error";

        this->height = height;
        this->width = width;

        polygons.clear();
        std::ifstream ifs{filename};
        vector<Polygon> ps = loadfile(ifs);
        polygons.reserve(ps.size());
        for(const Polygon& p : ps)
        {
            polygons.emplace_back();
            process_polygon& polygon = polygons.back();
            polygon.bounding = p;
            polygon.init_segments();
        }

        for(int i = 0; i < NUMSCALES; i++)
        {
            const int scale = SCALES[i];
            const int sheight = height/SCALES[i];
            const int swidth = width/SCALES[i];

            const double zeropoint = (scale-1)/2.0;

            targets[i].create(sheight, swidth);
            indexs[i].create(sheight, swidth);

            for(int j = 0; j < sheight; ++j)
            {
                double y = j*scale + zeropoint;

                for(int k = 0; k < swidth; ++k)
                {
                    double x = k*scale + zeropoint;
                    auto& target = targets[i].at<VecTarget>(j, k);
                    auto& index = indexs[i].at<int>(j, k);

                    for(int l = 0; l < (int)polygons.size(); ++l)
                    {
                        process_point temp;
                        bool succeeded = polygons[l].process(x, y, scale, temp);

                        if(succeeded)
                        {
                            target.vaild = 1;
                            target.height = temp.height;
                            target.width = temp.width;
                            target.deltax = temp.deltax;
                            target.deltay = temp.deltay;
                            target.angle = temp.angle;
                            target.curvature = temp.curvature;
                            index = l;
                        }
                        else
                        {
                            target.vaild = 0;
                        }
                    }
                }
            }

            auto& target = targets[i];
            auto& index = indexs[i];
            for(int j = 0; j < sheight; ++j)
            {
                for(int k = 0; k < swidth; ++k)
                {
                    if(target.at<VecTarget>(j, k).vaild < 0.5)
                        continue;
                    dolocsame(0, j-1, k-1);
                    dolocsame(1, j, k-1);
                    dolocsame(2, j+1, k-1);
                    dolocsame(3, j+1, k);
                    dolocsame(4, j+1, k+1);
                    dolocsame(5, j, k+1);
                    dolocsame(6, j-1, k+1);
                    dolocsame(7, j-1, k);

                    if(i != 0)
                    {
                        auto& target2 = targets[i-1];
                        auto& index2 = indexs[i-1];
                        docrosssame(0, 2*j, 2*k);
                        docrosssame(1, 2*j+1, 2*k);
                        docrosssame(2, 2*j, 2*k+1);
                        docrosssame(3, 2*j+1, 2*k+1);
                    }
                }
            }
        }
    }
};
