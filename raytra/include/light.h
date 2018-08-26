#ifndef LIGHT_H
#define LIGHT_H

#include "basic_math.h"
#include <vector>

class Light{

public:

    virtual Vector3d to_light(Point3d& from) = 0;
    virtual std::vector<Point3d> sample_light(unsigned int n) = 0;
    virtual Rational get_light_multiplier(Vector3d to_light_v) = 0;

    virtual ~Light(){};

    double r;
    double g;
    double b;
};

#endif