


#include "totalpreprocess.hpp"
#include "skeleton.hpp"

#include <CGAL/squared_distance_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_2_algorithms.h>

#include <list>

typedef CGAL::Ray_2<CGALK> Ray;
typedef CGAL::Polygon_with_holes_2<CGALK> Polygon_with_holes

using std::vector;
using cv::Point2d;

const int TARGET_NUMBER = 6;

class Angle
{
public:
    Angle() : angle(0) {};
    Angle(double a) {
        if(a<0)
        {
            a -= ((int)(a/(2*M_PI)+2)) * (2*M_PI);
        }
        angle = fmod(a, 2*M_PI);
    };
    double angle;

    static Angle bisector(Angle a1, Angle a2)
    {
        if(abs(a1.angle-a2.angle) < M_PI)
        {
            return Angle((a1.angle+a2.angle)/2);
        }
        else
        {
            return Angle((a1.angle+a2.angle)/2 + M_PI);
        }
    }
    static Angle distance(Angle a1, Angle a2)
    {
        double d = abs(a1.angle - a2.angle);
        if(d>M_PI)
            return 2*M_PI - d;
        else
            return d;
    }
    static Angle average(Angle a1, double w1, Angle a2, double w2)
    {
        if(abs(a1.angle-a2.angle) < M_PI)
        {
            return Angle((a1.angle*w1+a2.angle*w2)/(w1+w2));
        }
        else
        {
            return Angle((a1.angle*w1+a2.angle*w2)/(w1+w2) + M_PI);
        }
    }

    Angle vert()
    {
        return Angle(angle+M_PI/2);
    }
    Angle opposite()
    {
        return Angle(angle+M_PI);
    }

    Ray ray(Point p)
    {
        return Ray(p, Point(p.x()+cos(angle), p.y()+sin(angle)));
    }

    Vector direct()
    {
        return Vector(cos(angle), sin(angle));
    }

    Angle to90()
    {
        double d = angle % (M_PI/2);
        if(d>M_PI/4)
            return Angle(d-M_PI/2);
        else
            return Angle(d);
    }
};

struct text_segment
{
    Segment_2 segment;
    double length
    Angle angle;
    double curvature;
    Angle left_angle, right_angle;
};

class Process
{
  public:
    cv::Size2i size;
    cv::Mat source;
    MatTarget target[TARGET_NUMBER];

    Polygon_2 bounding;
    vector<vector<text_segment>> segments;

    void init_segments();
    void calc_target(Point2d pt, int& s1, int& s2, VecTarget& t1, VecTarget t2);
};

void Process::init_segments()
{
    this->segments.clear();

    vector<vector<Segment_2>> simpless = process_to_skeleton(this->bounding);
    this->segments.reserve(simpless.size());
    for(auto& simples: simpless)
    {
        this->segments.emplace_back();
        auto& back = this->segments.back();
        back.reserve(simples.size());
        for(auto simple: simples)
        {
            back.emplace_back();
            auto& last = back.back();
            last.segment = simple;
            last.length = sqrt(last.segment.squared_length());
            auto vector = last.segment.to_vector();
            last.angle = atan2(vector.y(), vector.x());
        }
        for(int i = 0; i < simples.size()-1; ++i)
        {
            Angle angle_last = i == 0 ? back[i].angle : back[i-1].angle;
            Angle angle_cur = back[i].angle;
            Angle angle_next = (i == simples.size() - 1) ? back[i].angle : back[i+1].angle;

            back[i].left_angle = Angle::bisector(angle_last, angle_cur);
            back[i].right_angle = Angle::bisector(angle_next, angle_cur);

            Angle deltaangle = Angle::distance(back[i].left_angle, back[i].right_angle);
            back[i].curvature = deltaangle.angle / back[i].angle;
        }
    }
}

bool nearest_point(Ray& r, Polygon_2& poly, double& result)
{
    double minval = INFINITY;
    bool found = false;
    for(auto it = poly.edges_begin(); it != poly.edges_end(); ++it)
    {
        Segment s = *it;
        Point *pt;
        auto inter = CGAL::intersection(r, s);
        if(inter && pt=boost::get<Segment>(&*inter))
        {
            double newdist = CGAL::squared_distance(minpt, *pt);
            if(newdist<minval)
            {
                minval = newdist;
                found = true;
            }
        }
    }
    if(found)
        result = sqrt(minval);
    return found;
}

void double2scale(double height, int&s1, int&s2)
{
    int curscale = 0;
    for(; height>(1.0/1.5); height/=2, curscale++)
    {
        if(height<1.5)
        {
            s1 = curscale;
            s2 = height>(2.0/1.5) ? curscale+1 : -1;
            return;
        }
    }
    s1 = -1;
    s2 = -1;
}

void Process::calc_target(
    Point2d pt,
    int& s1,
    int& s2,
    VecTarget& t1,
    VecTarget& t2)
{
    Point point{pt.x, pt.y};
    auto side = CGAL::bounded_side_2(bounding.vertices_begin(), bounding.vertices_end(), point);
    if(side != CGAL::ON_UNBOUNDED_SIDE)
    {
        s1 = -1;
        s2 = -1;
        return;
    }

    double mindist = INFINITY;
    text_segment* minseg = nullptr;

    for(auto& ss : this->segments)
    {
        for(auto &s : ss)
        {
            double newdist = CGAL::squared_distance(point, s.segment);
            if(newdist<mindist)
            {
                mindist = newdist;
                minseg = &s;
            }
        }
    }
    if(!minseg)
    {
        s1 = -1;
        s2 = -1;
        return;
    }

    double dist1 = sqrt(CGAL::squared_distance(point, minseg->segment.source()));
    double dist2 = sqrt(CGAL::squared_distance(point, minseg->segment.target()));
    Angle angle = Angle::average(minseg->left_angle, dist2, 
            minseg->right_angle, dist1);
    Ray r1 = angle.vert.ray(point);
    Ray r2 = angle.vert.opposite.ray(point);
    double d1, d2;
    if(!nearest_point(r1, this->bounding, d1))
    {
        s1 = -1;
        s2 = -1;
        return;
    }
    if(!nearest_point(r2, this->bounding, d2))
    {
        s1 = -1;
        s2 = -1;
        return;
    }
    double height = d1+d2;
    double2scale(height, s1, s2);

    Angle realangle = angle.to90();
    Vector e1 = realangle.direct();
    Vector e2 = realangle.vert().direct();

    Polygon rectpoly;
    rectpoly.push_back(point + e1*(scale/2) + e2*(scale/2*3));
    rectpoly.push_back(point + e1*(scale/2) - e2*(scale/2*3));
    rectpoly.push_back(point - e1*(scale/2) - e2*(scale/2*3));
    rectpoly.push_back(point - e1*(scale/2) + e2*(scale/2*3));

    std::list<Polygon> polyI;
    double minx=INFINITY, maxx=-INFINITY, miny=INFINITY, maxy=-INFINITY;
    bool succeeded = false;
    CGAL::intersection(bounding, rectpoly, std::back_inserter(polyI));
    for(auto it = polyI.begin(); it != polyI.end(); ++it)
    {
        for(auto it2 = it->vertices_begin(); it2 != it->vertices_end() ;++it2)
        {
            succeeded = true;
            double x = e1 * Vector(*it2);
            double y = e2 * Vector(*it2);
            if(x>maxx)
                maxx = x;
            if(x<minx)
                minx = x;
            if(y>maxy)
                maxy = y;
            if(y<miny)
                miny = y;
        }
    }
    double width = maxx - minx;
    double height = maxy - miny;

    Point newcenter = point + (maxx+minx)/2 * e1 + (maxy+miny)/2 * e2;
    double deltax = newcenter.x() - point.x();
    double deltay = newcenter.y() - point.y();
}
