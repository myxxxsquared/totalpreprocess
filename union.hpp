
#include "totalpreprocess.hpp"
#include "polygonpreprocess.hpp"
#include "process.hpp"
#include "matrix.h"
#include "data.hpp"
#include "cnpy.h"
#include "angle.hpp"

class Union
{
  public:
    MatTarget targets[NUMSCALES];
    double vaild, inlayer, crosslayer;
    std::vector<std::vector<VecTarget>> polygons;
    std::vector<std::vector<std::tuple<int, int, int>>> indexs;
    std::vector<Polygon> resultpolygons;

    cv::Mat image;

    Union(double vaild, double inlayer, double crosslayer);
    void load_file(const char *filename);
    void find_next_polygons(int i, int j, int scale);
    void find_polygons();
    void process_polygons();
    void paint_image(const char* imgfile);

    void process_data();
};

// 1. vaild bit (Percision, Recall)
// 2. inlayer-link bit (Percision, Recall, Maching)
// 3. crosslayer-link bit (Percision, Recall)
// 4. each polygon (iou, split rate, combine rate)
// 5. image (iou, percision, recall)
