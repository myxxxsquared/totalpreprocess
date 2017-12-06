
#ifndef DATA_HEADER
#define DATA_HEADER

#include <vector>
#include <istream>

struct Point2d
{ 
    double x, y;
    Point2d();
    Point2d(double px, double py);
    static double distance(Point2d p1, Point2d p2);
    static double cosval(Point2d p1, Point2d p2, Point2d p3);
};

std::vector<std::vector<Point2d>> loadfile(std::istream& ifs);

#endif
