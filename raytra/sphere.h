#ifndef SPHERE_H
#define SPHERE_H

#include "basic_math.h"
#include "ray.h"
#include "intersection.h"
#include "surface.h"

class Sphere : public Surface {
public:
    Point3d c;
    Rational r;

    Sphere();
    Sphere(Point3d center,Rational radius, int m_id);
    Intersection intersection(Ray& r,Rational t_min) override;

private:
   Rational compute_discriminant(Ray& r);
};

std::ostream& operator<<(std::ostream &os, const Sphere& s);


#endif