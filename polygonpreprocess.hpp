#ifndef POLYGON_PREPROCESS_HELADR
#define POLYGON_PREPROCESS_HELADR

#include "totalpreprocess.hpp"
#include "angle.hpp"

struct process_segment
{
    Segment segment;
    double length;
    Angle angle;
    double curvature;
    Angle left_angle, right_angle;
};

struct process_point
{
    double height, width;
    double deltax, deltay;
    double angle;
    double curvature;
};

struct process_polygon
{
    Polygon bounding;
    std::vector<std::vector<process_segment>> segments;

    void init_segments();
    bool process(double x, double y, double scale, process_point& result);
private:
    bool check_inside(Point point);
    process_segment* find_nearest(Point point);
    bool nearest_point(const Ray& r, double& result);
    Angle get_angle(Point point, process_segment* minseg);
};


#endif