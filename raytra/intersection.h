#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "basic_math.h"
#include "surface.h"

#define NO_P 0
#define ONE_P 1
#define TWO_P 2

class Intersection {

public:

    char intersection_type;
    bool smooth_normal_available;
    
    // The type of material
    int mat_id;
    
    // First and second intersection parameters
    double t1;
    double t2;

    // Intersection point
    Point3d inter_p;

    // Geometric Normal
    Vector3d normal;
    Vector3d smooth_normal;

    Intersection();

};








#endif