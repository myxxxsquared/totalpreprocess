#ifndef PROCESS_HEADER
#define PROCESS_HEADER

#include "data.hpp"
#include "angle.hpp"
#include "polygonpreprocess.hpp"
#include "matrix.h"

/*
 0 vaild
 1 angle
 2 deltax
 3 deltay
 4 width
 5 height
 6 curvature
 7-14 inlayer
 15-18 crosslayer
*/
struct VecTarget
{
    double vaild;
    double angle;
    double deltax, deltay;
    double width, height;
    double curvature;
    double inlayer[8];
    double crosslayer[4];
};

typedef xmat::Matrix<VecTarget> MatTarget;
typedef xmat::Matrix<int> MatIndexs;

constexpr int NUMSCALES = 6;
extern const int SCALES[];

class Preprocess
{
  public:
    std::vector<process_polygon> polygons;
    int height;
    int width;
    MatTarget targets[NUMSCALES];
    MatIndexs indexs[NUMSCALES];

    void process(int height, int width, const char *filename);
};

#endif