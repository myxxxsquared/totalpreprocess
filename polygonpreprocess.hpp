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
    double shortest;

    void init_segments(int height, int width);
    bool process(double x, double y, double scale, process_point &result);

  private:
    bool check_inside(const Point& point) const;
    const process_segment *find_nearest(const Point& point) const;
    bool nearest_point(const Ray &r, double &result) const;
    Angle get_angle(const Point& point, const process_segment *minseg) const;
    int image_width, image_height;
};

#endif