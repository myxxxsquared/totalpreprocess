
#include "totalpreprocess.hpp"
#include "process.hpp"

const int SCALES[] = {8, 16, 32, 64, 128, 256};

#define locsame(nj, nk) (nj >= 0 && nj < sheight && nk >= 0 && nk < swidth && target.at(nj, nk).vaild > 0.5 && index.at(j, k) == index.at(nj, nk))
#define crosssame(nj, nk) (target2.at(nj, nk).vaild > 0.5 && index.at(j, k) == index2.at(nj, nk))
#define dolocsame(m, nj, nk) target.at(j, k).inlayer[m] = locsame(nj, nk) ? 1.0 : 0.0
#define docrosssame(m, nj, nk) target.at(j, k).crosslayer[m] = crosssame(nj, nk) ? 1.0 : 0.0

void Preprocess::process(int height, int width, const char *filename)
{
    if (height % SCALES[NUMSCALES - 1] != 0 || width % SCALES[NUMSCALES - 1] != 0)
        throw "size error";

    this->height = height;
    this->width = width;

    polygons.clear();
    std::ifstream ifs{filename, std::ifstream::in};
    std::vector<Polygon> ps = loadfile(ifs);
    polygons.reserve(ps.size());
    for (const Polygon &p : ps)
    {
        polygons.emplace_back();
        process_polygon &polygon = polygons.back();
        polygon.bounding = p;
        polygon.init_segments(height, width);
    }

    for (int i = 0; i < NUMSCALES; i++)
    {
        const int scale = SCALES[i];
        const int sheight = height / SCALES[i];
        const int swidth = width / SCALES[i];

        const double zeropoint = (scale - 1) / 2.0;

        targets[i].create(sheight, swidth);
        indexs[i].create(sheight, swidth);

        for (int j = 0; j < sheight; ++j)
        {
            double y = j * scale + zeropoint;

            for (int k = 0; k < swidth; ++k)
            {
                double x = k * scale + zeropoint;
                auto &target = targets[i].at(j, k);
                auto &index = indexs[i].at(j, k);

                for (int l = 0; l < (int)polygons.size(); ++l)
                {
                    process_point temp;
                    bool succeeded = polygons[l].process(x, y, scale, temp);

                    if (succeeded)
                    {
                        target.vaild = 1;
                        target.height = temp.height;
                        target.width = temp.width;
                        target.deltax = temp.deltax;
                        target.deltay = temp.deltay;
                        target.angle = temp.angle;
                        target.curvature = temp.curvature;
                        index = l;
                        break;
                    }
                    else
                    {
                        target.vaild = 0;
                    }
                }
            }
        }

        auto &target = targets[i];
        auto &index = indexs[i];
        for (int j = 0; j < sheight; ++j)
        {
            for (int k = 0; k < swidth; ++k)
            {
                if (target.at(j, k).vaild < 0.5)
                    continue;
                dolocsame(0, j - 1, k - 1);
                dolocsame(1, j, k - 1);
                dolocsame(2, j + 1, k - 1);
                dolocsame(3, j + 1, k);
                dolocsame(4, j + 1, k + 1);
                dolocsame(5, j, k + 1);
                dolocsame(6, j - 1, k + 1);
                dolocsame(7, j - 1, k);

                if (i != 0)
                {
                    auto &target2 = targets[i - 1];
                    auto &index2 = indexs[i - 1];
                    docrosssame(0, 2 * j, 2 * k);
                    docrosssame(1, 2 * j + 1, 2 * k);
                    docrosssame(2, 2 * j, 2 * k + 1);
                    docrosssame(3, 2 * j + 1, 2 * k + 1);
                }
            }
        }
    }
}
