
#include "union.hpp"

Union::Union(double vaild, double inlayer, double crosslayer)
    : inlayer(inlayer), crosslayer(crosslayer), vaild(vaild)
{
}

void Union::load_file(const char *filename)
{
    char buffer[1024];

    for (int i = 0; i < NUMSCALES; ++i)
    {
        sprintf(buffer, "%s_%d.npy", filename, i);
        auto data = cnpy::npy_load(buffer);
        targets[i].create(data.shape[0], data.shape[1]);
        if(data.num_bytes() != data.shape[0]*data.shape[1]*sizeof(VecTarget))
            throw "invaild size";
        memcpy(targets[i].data(), data.data<double>(), data.num_bytes());
    }
}

#define inlayercheck(newindex, newi, newj)                                                                                                                       \
    if (curt.inlayer[newindex] > this->inlayer && newi >= 0 && newi < curm.rows() && newj >= 0 && newj < curm.cols() && curm.at(newi, newj).vaild > this->vaild) \
    {                                                                                                                                                            \
        indexes.emplace_back(std::make_tuple(newi, newj, curs));                                                                                                 \
        t.emplace_back(curm.at(newi, newj));                                                                                                                     \
        curm.at(newi, newj).vaild = 0;                                                                                                                           \
        havenew = true;                                                                                                                                          \
    }

#define crosslayercheck(newindex, newi, newj)                                                    \
    if (curt.crosslayer[newindex] > this->crosslayer && curc.at(newi, newj).vaild > this->vaild) \
    {                                                                                            \
        indexes.emplace_back(std::make_tuple(newi, newj, curs - 1));                             \
        t.emplace_back(curc.at(newi, newj));                                                     \
        curc.at(newi, newj).vaild = 0;                                                           \
        havenew = true;                                                                          \
    }

void Union::find_next_polygons(int i, int j, int scale)
{
    std::vector<std::tuple<int, int, int>> indexes;
    std::vector<VecTarget> t;
    indexes.emplace_back(std::make_tuple(i, j, scale));
    t.emplace_back(this->targets[scale][i][j]);
    bool havenew;
    do
    {
        havenew = false;
        for (int l = 0; l < (int)indexes.size(); ++l)
        {
            VecTarget &curt = t.at(l);
            int curi = std::get<0>(indexes.at(l));
            int curj = std::get<1>(indexes.at(l));
            int curs = std::get<2>(indexes.at(l));
            auto &curm = this->targets[curs];

            inlayercheck(0, curi - 1, curj - 1);
            inlayercheck(1, curi, curj - 1);
            inlayercheck(2, curi + 1, curj - 1);
            inlayercheck(3, curi + 1, curj);
            inlayercheck(4, curi + 1, curj + 1);
            inlayercheck(5, curi, curj + 1);
            inlayercheck(6, curi - 1, curj + 1);
            inlayercheck(7, curi - 1, curj);

            if (curs > 0)
            {
                auto &curc = this->targets[curs - 1];
                crosslayercheck(0, 2 * curi, 2 * curj);
                crosslayercheck(1, 2 * curi + 1, 2 * curj);
                crosslayercheck(2, 2 * curi, 2 * curj + 1);
                crosslayercheck(3, 2 * curi + 1, 2 * curj + 1);
            }
        }
    } while (havenew);

    this->polygons.emplace_back(t);
    this->indexs.emplace_back(indexes);
}

void Union::find_polygons()
{
    for (int i = NUMSCALES - 1; i >= 0; --i)
    {
        auto &target = targets[i];
        for (int j = 0; j < target.rows(); ++j)
        {
            for (int k = 0; k < target.cols(); ++k)
            {
                if (target.at(j, k).vaild > this->vaild)
                {
                    target.at(j, k).vaild = 0;
                    find_next_polygons(j, k, i);
                }
            }
        }
    }
}

void Union::process_polygons()
{
    for (int i = 0; i < (int)this->polygons.size(); ++i)
    {
        auto &inpolys = this->polygons.at(i);
        auto &indexs = this->indexs.at(i);

        Polygon_set s;
        bool first = true;

        for (int j = 0; j < (int)inpolys.size(); ++j)
        {
            auto &index = indexs.at(j);
            auto &target = inpolys.at(j);

            int y_ = std::get<0>(index);
            int x_ = std::get<1>(index);
            int scale = SCALES[std::get<2>(index)];
            double zeropoint = (scale - 1) / 2.0;
            double x = x_ * scale + zeropoint;
            double y = y_ * scale + zeropoint;

            double center_x = x + target.deltax * scale;
            double center_y = y + target.deltay * scale;
            double xdire_x = cos(target.angle) * target.width * scale;
            double xdire_y = sin(target.angle) * target.width * scale;
            double ydire_x = -sin(target.angle) * target.height * scale;
            double ydire_y = cos(target.angle) * target.height * scale;

            Polygon p;

            p.push_back(Point(
                center_x - xdire_x + ydire_x,
                center_y - xdire_y + ydire_y));
            p.push_back(Point(
                center_x - xdire_x - ydire_x,
                center_y - xdire_y - ydire_y));
            p.push_back(Point(
                center_x + xdire_x - ydire_x,
                center_y + xdire_y - ydire_y));
            p.push_back(Point(
                center_x + xdire_x + ydire_x,
                center_y + xdire_y + ydire_y));

            // cv::Mat matrix;
            // matrix.create(512, 512, CV_8UC1);
            // matrix.setTo(cv::Scalar(0));
            // for (auto it = p.edges_begin(); it != p.edges_end(); ++it)
            // {
            //     auto pti = it->source();
            //     auto ptj = it->target();
            //     cv::line(matrix, cv::Point(pti.x(), pti.y()), cv::Point(ptj.x(), ptj.y()), cv::Scalar(255));
            // }
            // cv::imshow("test", matrix);
            // cv::waitKey();

            // if (p.is_clockwise_oriented())
            // {
            //     std::reverse(p.vertices_begin(), p.vertices_end());
            // }

            // for (auto it = p.vertices_begin(); it != p.vertices_end(); ++it)
            // {
            //     printf("%.20lf, %.20lf\n", it->x(), it->y());
            // }
            // printf("\n");
            if(first)
                s.insert(p);
            else
                s.join(p);
            first = false;
        }
        // for (auto &polygon : set)
        // {
        //     s.join(polygon);
        // }
        std::list<Polygon_with_holes> lst;
        s.polygons_with_holes(std::back_inserter(lst));
        for (auto &l : lst)
            this->resultpolygons.emplace_back(l.outer_boundary());
    }
}

void Union::paint_image(const char *imgfile)
{
    this->image = cv::imread(imgfile);
    for (auto &polygon : this->resultpolygons)
    {
        for (auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it)
        {
            auto pti = it->source();
            auto ptj = it->target();
            cv::line(this->image,
                     cv::Point(pti.x(), pti.y()),
                     cv::Point(ptj.x(), ptj.y()),
                     cv::Scalar(255, 255, 0));
        }
    }

    cv::imshow("show", this->image);
    cv::waitKey();
}

void Union::process_data()
{
    
}