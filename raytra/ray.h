#ifndef RAY_H
#define RAY_H

#include "basic_math.h"
#include <iostream>


// r(t) = P + dt 
class Ray {

public:
    Point3d ori;
    Vector3d dir;
    Vector3d invdir;
    int sign[3];
    
    Ray();
    Ray(Point3d o, Vector3d dir);
    Ray(Point3d p1, Point3d p2);

    Point3d value_at(double t);
};

std::ostream& operator<<(std::ostream &os, const Ray& r);



#endif