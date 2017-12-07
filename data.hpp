
#ifndef DATA_HEADER
#define DATA_HEADER

#include <opencv2/opencv.hpp>
#include <vector>
#include <istream>

std::vector<std::vector<cv::Point2d>> loadfile(std::istream& ifs);

#endif
