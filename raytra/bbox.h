#ifndef BBOX_H
#define BBOX_H


#include "basic_math.h"
#include "surface.h"
#include "ray.h"
#include <vector>

#define BOX_VOLUME_EPS 0.001

class BBox;
using BBox_iterator = const std::vector<BBox>::iterator;

// Decided not to have
class BBox {

public:
    
    BBox(){}
    BBox(Rational, Rational, Rational, Rational, Rational, Rational, Surface*);
    
    // Build a BBox from a range of boxes. 
    BBox(BBox_iterator first, BBox_iterator last);
   
    
    bool check_intersetcion(Ray &r);


    Point3d centroid;
    Point3d pmax;
    Point3d pmin;

    Point3d bounds[2];

    // Rational xmin, xmax;
    // Rational ymin, ymax;
    // Rational zmin, zmax;

    // Pointer to the surface the bounding box bounds
    Surface *surf;


};


//bool bbox_axis_compare(Point3d a, Point3d b, Axis axis);
bool axis_compare(const BBox& a, const BBox& b, Axis axis);
Axis rotate_axis(Axis previous_axis);
std::ostream& operator<<(std::ostream &os, const BBox& b);

#endif