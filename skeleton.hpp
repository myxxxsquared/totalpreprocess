
#ifndef SKELETON_HEADER
#define SKELETON_HEADER

#include "data.hpp"
#include <vector>

std::vector<std::vector<cv::Point2d>>
process_to_skeleton(std::vector<cv::Point2d> polygon);

#endif
