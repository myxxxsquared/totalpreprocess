
#include "totalpreprocess.hpp"
#include "polygonpreprocess.hpp"
#include "skeleton.hpp"

void process_polygon::init_segments()
{
    this->segments.clear();

    std::vector<std::vector<Segment>> simpless = process_to_skeleton(this->bounding);
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
        for(int i = 0; i < (int)simples.size()-1; ++i)
        {
            Angle angle_last = i == 0 ? back[i].angle : back[i-1].angle;
            Angle angle_cur = back[i].angle;
            Angle angle_next = (i == (int)simples.size() - 1) ? back[i].angle : back[i+1].angle;

            back[i].left_angle = Angle::bisector(angle_last, angle_cur);
            back[i].right_angle = Angle::bisector(angle_next, angle_cur);

            Angle deltaangle = Angle::distance(back[i].left_angle, back[i].right_angle);
            back[i].curvature = deltaangle.angle / back[i].length;
        }
    }
}

bool process_polygon::check_inside(Point point)
{
    return CGAL::bounded_side_2(
        bounding.vertices_begin(),
        bounding.vertices_end(),
        point) == CGAL::ON_UNBOUNDED_SIDE;
}

process_segment* process_polygon::find_nearest(Point point)
{
    double mindist = INFINITY;
    process_segment* minseg = nullptr;

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

    return minseg;
}

bool process_polygon::nearest_point(const Ray& r, double& result)
{
    double minval = INFINITY;
    bool found = false;
    for(auto it = bounding.edges_begin(); it != bounding.edges_end(); ++it)
    {
        Segment s = *it;
        auto inter = CGAL::intersection(r, s);
        if(auto * pt=boost::get<Segment>(&*inter))
        {
            double newdist = CGAL::squared_distance(r.source(), *pt);
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

Angle process_polygon::get_angle(Point point, process_segment* minseg)
{
    double dist1 = sqrt(CGAL::squared_distance(point, minseg->segment.source()));
    double dist2 = sqrt(CGAL::squared_distance(point, minseg->segment.target()));
    return Angle::average(minseg->left_angle, dist2, 
            minseg->right_angle, dist1);
}

bool process_polygon::process(double x, double y, double scale, process_point& result)
{
    Point point{x, y};
    if(!check_inside(point))
        return false;

    process_segment* minseg = find_nearest(point);
    if(!minseg)
        return false;

    Angle angle = get_angle(point, minseg);

    Angle realangle = angle.to90();
    Vector e1 = realangle.direct();
    Vector e2 = realangle.vert().direct();

    double xl, xr, yt, yb;

    if(!nearest_point(angle.vert().ray(point), yt))
        return false;
    if(!nearest_point(angle.vert().opposite().ray(point), yb))
        return false;
    double height = yt + yb;

    if(scale / height > 1.5 || height / scale > 1.5 )
        return false;

    if(!nearest_point(angle.opposite().ray(point), xl))
        return false;
    if(!nearest_point(angle.ray(point), xr))
        return false;
    if(xl > scale/2)
        xl = scale/2;
    if(xr > scale/2)
        xr = scale/2;

    double width = xl + xr;
    Point newcenter = point + (xr-xl) * e1 + (yt-yb) * e2;
    double deltax = newcenter.x() - point.x();
    double deltay = newcenter.y() - point.y();

    result.height = height;
    result.width = width;
    result.deltax = deltax;
    result.deltay = deltay;
    result.angle = realangle.angle;
    result.curvature = minseg->curvature;

    return true;
}