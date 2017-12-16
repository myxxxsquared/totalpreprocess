
#include "totalpreprocess.hpp"
#include "data.hpp"
#include "skeleton.hpp"

using std::vector;
using std::set;
using std::pair;
using std::make_pair;
using cv::Point2d;

typedef typename Ss::Vertex_const_handle Vertex_const_handle;
typedef typename Ss::Halfedge_const_handle Halfedge_const_handle;
typedef typename Ss::Halfedge_const_iterator Halfedge_const_iterator;

#define MIN_COS_VALUE 0.707

class Point2ds
{
    vector<vector<Point2d>> points;
    vector<Point2d> centers;

    int find_point(Point2d p)
    {
        for(int i = 0; i < this->centers.size(); ++i)
            if(cv::norm(this->centers[i] - p) < this->min_dist)
                return i;
        return -1;
    }
    void update_point_center(int i)
    {
        auto& ps = this->points[i];
        auto& p = this->centers[i];

        p.x = 0;
        p.y = 0;

        for(auto& ep : ps)
        {
            p.x += ep.x;
            p.y += ep.y;
        }

        p.x /= ps.size();
        p.y /= ps.size();
    }
  public:
    double min_dist;
    void init(double min_d)
    {
        points.clear();
        centers.clear();
        min_dist = min_d;
    }
    void add_point(Point2d p)
    {
        int index = this->find_point(p);
        if(index != -1)
        {
            this->points[index].push_back(p);
            this->update_point_center(index);
        }
        else
        {
            this->points.emplace_back();
            this->points.back().push_back(p);
            this->centers.push_back(p);
        }
    }
    int find_nearest(Point2d p)
    {
        double dmin = INFINITY;
        int index = -1;
        for(int i = 0; i < this->centers.size(); ++i)
        {
            double d = cv::norm(this->centers[i] - p);
            if(d<dmin)
            {
                dmin = d;
                index = i;
            }
        }
        return index;
    }
    Point2d at(int i)
    {
        return this->centers[i];
    }
    int size()
    {
        return this->centers.size();
    }
};

class IdGraph
{
  public:
    vector<vector<unsigned char>> edges;

    void init(int d)
    {
        edges.clear();
        this->edges.resize(d);
        for(auto& e : this->edges)
            e.resize(d);
    }

    void add_edge(int i, int j)
    {
        if(i != j)
        {
            edges[i][j] = 1;
            edges[j][i] = 1;
        }
    }

    vector<int> next_edges(int i)
    {
        vector<int> result;
        for(int j = 0; j < edges.size(); ++j)
            if(edges[i][j])
                result.push_back(j);
        return result;
    }
};

static double edge_length(Point p1, Point p2)
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();

    return sqrt(x*x + y*y);
}

static double cos_value(Point2d p1, Point2d p2, Point2d p3)
{
    Point2d dp1 = p2-p1, dp2 = p3-p2;
    return (dp1.x*dp2.x + dp1.y*dp2.y) / cv::norm(dp1) / cv::norm(dp2);
}

class SkeletonProcessor
{
  public:
    Point2ds ps;
    IdGraph graph;
    vector<vector<int>> lines;

    void init(const Ss& ss)
    {
        double max_length = 0;
        for (Halfedge_const_iterator i = ss.halfedges_begin(); i != ss.halfedges_end(); ++i)
        {
            if(!i->is_inner_bisector())
                continue;
            auto p1 = i->opposite()->vertex()->point();
            auto p2 = i->vertex()->point();
            double length = edge_length(p1, p2);
            if(length > max_length)
                max_length = length;
        }

        ps.init(max_length / 10.0);
        for (Halfedge_const_iterator i = ss.halfedges_begin(); i != ss.halfedges_end(); ++i)
        {
            if(!i->is_inner_bisector())
                continue;
            auto p1 = i->opposite()->vertex()->point();
            auto p2 = i->vertex()->point();
            ps.add_point(Point2d(p1.x(), p1.y()));
            ps.add_point(Point2d(p2.x(), p2.y()));
        }

        graph.init(ps.size());
        for (Halfedge_const_iterator i = ss.halfedges_begin(); i != ss.halfedges_end(); ++i)
        {
            if(!i->is_inner_bisector())
                continue;
            auto p1 = i->opposite()->vertex()->point();
            auto p2 = i->vertex()->point();
            graph.add_edge(
                ps.find_nearest(Point2d(p1.x(), p1.y())),
                ps.find_nearest(Point2d(p2.x(), p2.y())));
        }
    }

    int findnextedge(int i, int j)
    {
        Point2d p1 = ps.at(i), p2 = ps.at(j);

        int index = -1;
        double maxcos = -1;

        for(int k = 0; k < graph.edges.size(); ++k)
        {
            if(!graph.edges[j][k])
                continue;
            Point2d p3 = ps.at(k);
            double cosval = cos_value(p1, p2, p3);
            if(cosval > MIN_COS_VALUE
                && cosval > maxcos)
            {
                index = k;
                maxcos = cosval;
            }
        }

        return index;
    }

    vector<int> findnextedges(int i, int j)
    {
        vector<int> result;
        result.push_back(j);

        while(true)
        {
            int k = findnextedge(i, j);

            if(k == -1)
                break;

            graph.edges[j][k] = 0;
            graph.edges[k][j] = 0;

            result.push_back(k);
            i=j;
            j=k;
        }
        return result;
    }

    void process()
    {
        for(int i = 0; i < ps.size(); ++i)
        {
            for(int j = 0; j < ps.size(); ++j)
            {
                if(!graph.edges[i][j])
                    continue;
                
                graph.edges[i][j] = 0;
                graph.edges[j][i] = 0;

                vector<int> p1, p2, p;
                p1 = findnextedges(i, j);
                p2 = findnextedges(j, i);
                p.resize(p1.size() + p2.size());
                for(int i = 0; i < p1.size(); ++i)
                    p[p1.size()-i-1] = p1[i];
                for(int i = 0; i < p2.size(); ++i)
                    p[i+p1.size()] = p2[i];
                lines.push_back(p);
            }
        }
    }
};

vector<vector<Segment_2>> process_to_skeleton(const Polygon_2& poly)
{
    SsPtr ss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
    SkeletonProcessor p;
    p.init(*ss);
    p.process();

    auto& lines = p.lines;

    vector<vector<Segment_2>> result;
    for(auto& l : lines)
    {
        result.emplace_back();
        vector<Point> points;
        for(int pt : l)
            points.emplace_back(p.ps.at(pt).x, p.ps.at(pt).y);
        for(int i = 0; i < points.size() - 1; ++i)
            result.back().emplace_back(points[i], points[i+1]);
    }
    return result;
}
