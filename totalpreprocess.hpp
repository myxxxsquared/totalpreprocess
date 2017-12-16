#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Polygon_2_algorithms.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel CGALK;
typedef CGALK::Point_2 Point;
typedef CGAL::Polygon_2<CGALK> Polygon_2;
typedef CGAL::Segment_2<CGALK> Segment_2;
typedef CGAL::Vector_2<CGALK> Vector;
typedef CGAL::Straight_skeleton_2<CGALK> Ss;
typedef boost::shared_ptr<Ss> SsPtr;

#include <opencv2/opencv.hpp>
typedef cv::Vec<float, 32> VecTarget;
typedef cv::Mat_<VecTarget> MatTarget;

#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <fstream>
#include <istream>
#include <string>
