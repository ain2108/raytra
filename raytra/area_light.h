#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include "basic_math.h"
#include "light.h"
#include <iostream>

class AreaLight : public Light {

public:
    Point3d pos;
    Vector3d dir;
    Vector3d udir;
    Vector3d vdir;
    Rational length;
    Point3d origin;

    virtual ~AreaLight(){};

    AreaLight(Point3d, Vector3d, Vector3d, Rational, Rational, Rational, Rational);
    
    Vector3d to_light(Point3d& from) override;

    std::vector<Point3d> sample_light(unsigned int n) override;
    Rational get_light_multiplier(Vector3d to_light_v) override;
    

};


#endif