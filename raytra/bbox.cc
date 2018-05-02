#include "bbox.h"

#include <limits>


BBox::BBox(Rational xmin, Rational xmax,
    Rational ymin, Rational ymax,
    Rational zmin, Rational zmax, Surface *surf){
        
        // We need to make sure the BBox has volume
        if (xmin == xmax){
            xmax = xmax + BOX_VOLUME_EPS;
        }

        if (ymin == ymax) {
            ymax = ymax + BOX_VOLUME_EPS;
        }

        if (zmin == zmax) {
            zmax = zmax + BOX_VOLUME_EPS;
        }
        
        this->pmin = Point3d(xmin, ymin, zmin);
        this->pmax = Point3d(xmax, ymax, zmax);

        this->bounds[0] = this->pmin;
        this->bounds[1] = this->pmax;

        // Get the centroid of the box
        this->centroid = Point3d((xmin + xmax) / 2,
            (ymin + ymax) / 2,  
            (zmin + zmax) / 2); 
        
        this->surf = surf;
       
}

// TODO: There must be a better way to do this
// NOTE: Sets the surface pointer to nullptr
BBox::BBox(BBox_iterator first, BBox_iterator last){
    
    this->pmin = Point3d(first->pmin.x, first->pmin.y, first->pmin.z);
    this->pmax = Point3d(first->pmax.x, first->pmax.y, first->pmax.z);
    
    auto second = first + 1;

    for (auto it = second; it != last; ++it){

        this->pmin.x = std::min(this->pmin.x, it->pmin.x);
        this->pmax.x = std::max(this->pmax.x, it->pmax.x);
        this->pmin.y = std::min(this->pmin.y, it->pmin.y);
        this->pmax.y = std::max(this->pmax.y, it->pmax.y);
        this->pmin.z = std::min(this->pmin.z, it->pmin.z);
        this->pmax.z = std::max(this->pmax.z, it->pmax.z);
        
    }

    this->centroid = Point3d(
    (this->pmin.x + this->pmax.x) / 2,
    (this->pmin.y + this->pmax.y) / 2,  
    (this->pmin.z + this->pmax.z) / 2); 

    bounds[0] = this->pmin;
    bounds[1] = this->pmax;

    this->surf = nullptr;

}


/* TODO: Optimize this mess */
// bool BBox::check_intersetcion(Ray &r){

//     auto min = this->pmin;
//     auto max = this->pmax;

//     float tmin = (min.x - r.ori.x) / r.dir.x; 
//     float tmax = (max.x - r.ori.x) / r.dir.x; 
 
//     if (tmin > tmax) std::swap(tmin, tmax); 
 
//     float tymin = (min.y - r.ori.y) / r.dir.y; 
//     float tymax = (max.y - r.ori.y) / r.dir.y; 
 
//     if (tymin > tymax) std::swap(tymin, tymax); 
 
//     if ((tmin > tymax) || (tymin > tmax)) 
//         return false; 
 
//     if (tymin > tmin) 
//         tmin = tymin; 
 
//     if (tymax < tmax) 
//         tmax = tymax; 
 
//     float tzmin = (min.z - r.ori.z) / r.dir.z; 
//     float tzmax = (max.z - r.ori.z) / r.dir.z; 
 
//     if (tzmin > tzmax) std::swap(tzmin, tzmax); 
 
//     if ((tmin > tzmax) || (tzmin > tmax)) 
//         return false; 
 
//     if (tzmin > tmin) 
//         tmin = tzmin; 
 
//     if (tzmax < tmax) 
//         tmax = tzmax; 
 
//     return true; 

// }

// The function adopted from:
// This on is around 100ms faster on the cloud.
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
bool BBox::check_intersetcion(Ray &r){ 
    Rational tmin, tmax, tymin, tymax, tzmin, tzmax; 

 
    tmin = (bounds[r.sign[0]].x - r.ori.x) * r.invdir.x; 
    tmax = (bounds[1-r.sign[0]].x - r.ori.x) * r.invdir.x; 
    tymin = (bounds[r.sign[1]].y - r.ori.y) * r.invdir.y; 
    tymax = (bounds[1-r.sign[1]].y - r.ori.y) * r.invdir.y; 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
    if (tymin > tmin) 
        tmin = tymin; 
    if (tymax < tmax) 
        tmax = tymax; 
 
    tzmin = (bounds[r.sign[2]].z - r.ori.z) * r.invdir.z; 
    tzmax = (bounds[1-r.sign[2]].z - r.ori.z) * r.invdir.z; 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
    if (tzmin > tmin) 
        tmin = tzmin; 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
} 

bool axis_compare(const BBox& a, const BBox& b, Axis ax){
    switch(ax){
        case X_AXIS:
            return a.centroid.x > b.centroid.x;
        case Y_AXIS:
            return a.centroid.y > b.centroid.y;
        case Z_AXIS:
            return a.centroid.z > b.centroid.z;
        default:
            //std::cerr << "ERROR in " << __func__ << ": bad axis\n" << std::cerr;
            std::abort();
    }
}

Axis rotate_axis(Axis previous_axis){
    switch(previous_axis){
        case X_AXIS:
            return Y_AXIS;
        case Y_AXIS:
            return Z_AXIS;
        case Z_AXIS:
            return X_AXIS;
        default:
            //std::cerr << "ERROR in " << __func__ << ": bad axis\n" << std::cerr;
            std::abort();
    }
}


std::ostream& operator<<(std::ostream &os, const BBox& b){
    return os << "pmin: " << b.pmin << " pmax: " << b.pmax;
}