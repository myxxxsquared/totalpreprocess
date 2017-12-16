
#ifndef ANGLE_HEADER
#define ANGLE_HEADER

#include "totalpreprocess.hpp"

class Angle
{
  public:
    Angle();
    Angle(double a);
    double angle;

    static Angle bisector(const Angle &a1, const Angle &a2);
    static Angle distance(const Angle &a1, const Angle &a2);
    static Angle average(const Angle &a1, double w1, const Angle &a2, double w2);

    Angle vert() const;
    Angle opposite() const;
    Ray ray(const Point &p) const;
    Vector direct() const;
    Angle to90() const;
};

#endif