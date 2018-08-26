#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"
#include "intersection.h"

class Intersection;

class Surface {
public:
    virtual Intersection intersection(Ray& r, double t_min) = 0;
    int material_id;
};






#endif