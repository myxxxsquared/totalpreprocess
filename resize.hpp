
#ifndef RESIZE_HEADER
#define RESIZE_HEADER

#include "polygonpreprocess.hpp"
#include <vector>

class Resize
{
    public:
    std::vector<Polygon> polygons;
    std::vector<double> shortlen;
    std::vector<double> area;
    cv::Mat image;

    int current;
    int target_height, target_width;

    void loadfile(const char* polyfile, const char* imagefile);
    void savefile(const char* outputfile, cv::Rect roi);
    void generate(const char* outputfile);
    void generate_len(int len, const char* outputfile);
    bool checkroi(cv::Rect roi);
};

#endif