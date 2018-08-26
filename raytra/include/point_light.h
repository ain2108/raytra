#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"
#include "basic_math.h"
#include <memory>


class PointLight : public Light {

public:
    Point3d pos;
    virtual ~PointLight(){}    

    PointLight(Rational r, Rational g, Rational b, Point3d p);
    std::vector<Point3d> sample_light(unsigned int n) override;

    Vector3d to_light(Point3d& from) override;
    Rational get_light_multiplier(Vector3d to_light_v) override;

};

std::shared_ptr<Light> create_light(Rational r, Rational g, 
    Rational b,  Point3d p);

#endif