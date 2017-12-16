#include <boost/shared_ptr.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <opencv2/opencv.hpp>

#include <list>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <fstream>
#include <istream>
#include <string>

typedef CGAL::Cartesian<double> CGALK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel CGALK_SS;
typedef CGALK::Point_2 Point;
typedef CGALK_SS::Point_2 PointSs;
typedef CGAL::Polygon_2<CGALK> Polygon;
typedef CGAL::Segment_2<CGALK> Segment;
typedef CGAL::Vector_2<CGALK> Vector;
typedef CGAL::Straight_skeleton_2<CGALK_SS> Ss;
typedef boost::shared_ptr<Ss> SsPtr;
typedef CGAL::Ray_2<CGALK> Ray;
typedef CGAL::Polygon_with_holes_2<CGALK> Polygon_with_holes;
