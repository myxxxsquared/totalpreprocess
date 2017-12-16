
#include "totalpreprocess.hpp"
#include "angle.hpp"

Angle::Angle() : angle(0) {}

Angle::Angle(double a)
{
    if (a < 0)
    {
        a -= ((int)(a / (2 * M_PI) - 2)) * (2 * M_PI);
    }
    angle = fmod(a, 2 * M_PI);
};

Angle Angle::bisector(const Angle &a1, const Angle &a2)
{
    if (abs(a1.angle - a2.angle) < M_PI)
    {
        return Angle((a1.angle + a2.angle) / 2);
    }
    else
    {
        return Angle((a1.angle + a2.angle) / 2 + M_PI);
    }
}

Angle Angle::average(const Angle &a1, double w1, const Angle &a2, double w2)
{
    if (abs(a1.angle - a2.angle) < M_PI)
    {
        return Angle((a1.angle * w1 + a2.angle * w2) / (w1 + w2));
    }
    else
    {
        return Angle((a1.angle * w1 + a2.angle * w2) / (w1 + w2) + M_PI);
    }
}

Angle Angle::distance(const Angle &a1, const Angle &a2)
{
    double d = abs(a1.angle - a2.angle);
    if (d > M_PI)
        return 2 * M_PI - d;
    else
        return d;
}

Angle Angle::vert() const
{
    return Angle(angle + M_PI / 2);
}

Angle Angle::opposite() const
{
    return Angle(angle + M_PI);
}

Ray Angle::ray(const Point &p) const
{
    return Ray(p, Point(p.x() + cos(angle)*10000, p.y() + sin(angle)*10000));
}

Vector Angle::direct() const
{
    return Vector(cos(angle), sin(angle));
}

Angle Angle::to90() const
{
    return Angle(fmod(angle, (M_PI)));
}
