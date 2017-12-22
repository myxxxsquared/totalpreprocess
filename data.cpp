
#include "totalpreprocess.hpp"
#include "data.hpp"

using cv::Point2d;
using std::istream;
using std::string;
using std::vector;

vector<Polygon> loadfile(istream &ifs)
{
    string line;

    vector<Polygon> result;
    Polygon current;

    while (ifs.good())
    {
        getline(ifs, line);
        if (line.size() == 0 || line[0] == 'n' || line[0] == 'e')
        {
            if (current.size() != 0)
            {
                if(current.is_clockwise_oriented())
                    std::reverse(current.vertices_begin(), current.vertices_end());
                result.push_back(current);
                current.clear();
            }
            continue;
        }
        if (line[0] == 'p')
        {
            double x, y;
            sscanf(line.c_str(), "p %lf, %lf", &x, &y);
            current.push_back(Point(x, y));
        }
    }

    return result;
}

void writefile(std::ostream& ofs, const std::vector<Polygon>& polygons)
{
    for(auto& polygon: polygons)
    {
        ofs << "n\n";
        for(auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
            ofs << "p " << it->x() << ", " << it->y() << "\n";
        ofs << "e\n";
    }
}
