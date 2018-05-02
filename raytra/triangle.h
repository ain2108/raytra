#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "basic_math.h"
#include "ray.h"
#include "intersection.h"
#include "surface.h"

#define EPSILON 0.0000001

class Triangle : public Surface {
public:
    Point3d p1;
    Point3d p2;
    Point3d p3;

    Vector3d normal;

    Triangle();
    Triangle(Point3d p1, Point3d p2, Point3d p3, int mat_id);
    Intersection intersection(Ray& r, Rational t_min) override;

    Vector3d p1normal;
    Vector3d p2normal;
    Vector3d p3normal;
};



#endif