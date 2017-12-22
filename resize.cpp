
#include "resize.hpp"
#include "data.hpp"
#include <algorithm>

void Resize::loadfile(const char* polyfile, const char* imagefile)
{
    std::ifstream ifs{polyfile};
    this->polygons = ::loadfile(ifs);
    image = cv::imread(imagefile);
    this->shortlen.clear();
    this->area.clear();
    this->shortlen.reserve(this->polygons.size());
    this->area.reserve(this->polygons.size());
    for(int i = 0; i < polygons.size(); ++i)
    {
        auto& polygon = polygons[i];
        double minlen = INFINITY;
        for(auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it)
        {
            double len = it->squared_length();
            if(len < minlen)
                minlen = len;
        }
        this->shortlen.push_back(sqrt(minlen));
        this->area.push_back(polygon.area()/4.0);
    }
}

void Resize::savefile(const char* outputfile, cv::Rect roi)
{
    char buffer[1024];

    cv::Mat targetimage;
    cv::resize(image(roi), targetimage, cv::Size(target_width, target_height));
    sprintf(buffer, "%s%08d.png", outputfile, current);
    cv::imwrite(buffer, targetimage);

    std::vector<Polygon> newpolygons;
    for(auto& polygon: polygons)
    {
        newpolygons.emplace_back();
        auto& back = newpolygons.back();
        for(auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
        {
            double newx = (it->x()-roi.x)*target_height/roi.width;
            double newy = (it->y()-roi.y)*target_width/roi.height;
            back.push_back(Point(newx, newy));
        }
    }
    sprintf(buffer, "%s%08d.poly", outputfile, current);
    std::ofstream ofs{buffer};
    writefile(ofs, newpolygons);

    ++current;
}

void Resize::generate(const char* outputfile)
{
    for(int deflen = std::min(this->image.cols, this->image.rows);
        deflen >= target_height/2; deflen /= 2)
        generate_len(deflen, outputfile);
}

bool Resize::checkroi(cv::Rect roi)
{
    bool result = false;
    Polygon roipoly;
    roipoly.push_back(Point(roi.x, roi.y));
    roipoly.push_back(Point(roi.x + roi.width, roi.y));
    roipoly.push_back(Point(roi.x + roi.width, roi.y + roi.height));
    roipoly.push_back(Point(roi.x, roi.y + roi.height));
    for(int i = 0; i < polygons.size(); ++i)
    {
        if(shortlen[i] > roi.height)
            continue;
        auto& polygon = this->polygons[i];
        std::list<CGAL::Polygon_with_holes_2<CGALK>> in;
        CGAL::intersection(roipoly, polygon, std::back_inserter(in));
        for(auto &inp: in)
            if(inp.outer_boundary().area() > this->area[i])
                return true;

        // for(auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
        // {
        //     if(it->x() >= roi.x && it->x() <= roi.x+roi.width
        //         && it->y() >= roi.y && it->y() <= roi.y+roi.height)
        //         return true;
        // }
    }
    return false;
}

void Resize::generate_len(int len, const char* outputfile)
{
    for(int x = 0; x <= image.cols - len; x += len)
    {
        for(int y = 0; y <= image.rows - len; y += len)
        {
            try
            {
                cv::Rect roi{x, y, len, len};
                if(checkroi(roi))
                    savefile(outputfile, roi);
            }
            catch(...)
            {}
        }
    }
}