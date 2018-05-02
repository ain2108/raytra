#include "ray.h"

Ray::Ray(){
    this->ori = Point3d(0,0,0);
    this->dir = Vector3d(0,0,0);
}

Ray::Ray(Point3d p, Vector3d v){
    this->ori = p;
    this->dir = normalize(v);
    this->invdir = 1.0 / this->dir;
    sign[0] = (invdir.x < 0); 
    sign[1] = (invdir.y < 0); 
    sign[2] = (invdir.z < 0); 
}

Ray::Ray(Point3d p1, Point3d p2){
    *this = Ray(p1, p2 - p1);
}

Point3d Ray::value_at(double t){
    return (this->ori + t * this->dir);
}

std::ostream& operator<<(std::ostream &os, const Ray& r){
    return std::cout << "r(t) = " << r.ori << " + t" << r.dir; 
}