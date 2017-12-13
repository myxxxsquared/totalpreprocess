


#include "totalpreprocess.hpp"

const int TARGET_NUMBER = 6;

class Process
{
  public:
    cv::Size2i size;
    cv::Mat source;
    MatTarget target[TARGET_NUMBER];

    Polygon_2 bounding;
    std::vector<std::vector<Segment_2>> segments;
};

