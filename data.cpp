
#include <cmath>
#include <vector>
#include <istream>
#include <string>

#include "data.hpp"

using std::vector;
using std::istream;
using std::string;
using std::getline;

Point2d::Point2d() {}

Point2d::Point2d(double px, double py) : x(px), y(py) {}

double Point2d::distance(Point2d p1, Point2d p2)
{
    double x = p1.x - p2.x, y = p1.y - p2.y;
    return sqrt(x*x+y*y);
}

double Point2d::cosval(Point2d p1, Point2d p2, Point2d p3)
{
    double x1 = p2.x - p1.x, x2 = p3.x - p2.x;
    double y1 = p2.y - p1.y, y2 = p3.y - p2.y;
    return (x1*x2+y1*y2)/sqrt((x1*x1+y1*y1)/(x2*x2+y2*y2));
}

vector<vector<Point2d>> loadfile(std::istream& ifs)
{
    string line;

    vector<vector<Point2d>> result;
    vector<Point2d> current;

    while(ifs.good())
    {
        getline(ifs, line);
        if(line.size() == 0 || line[0] == 'n' || line[0] == 'e')
        {
            if(current.size() != 0)
            {
                result.push_back(current);
                current.clear();
            }
            continue;
        }
        if(line[0] == 'p')
        {
            int x, y;
            sscanf(line.c_str(), "p %d, %d", &x, &y);
            current.push_back(Point2d(x, y));
        }
    }

    return result;
}
