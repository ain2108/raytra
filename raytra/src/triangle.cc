#include "triangle.h"

Triangle::Triangle(){}

Triangle::Triangle(Point3d p1, Point3d p2, Point3d p3, int mat_id){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;

    this->material_id = mat_id;

    this->normal = normalize(cross(p2 - p1, p3 - p1));
    
    this->p1normal = Vector3d(0, 0, 0);
    this->p2normal = Vector3d(0, 0, 0);
    this->p3normal = Vector3d(0, 0, 0);


}


/*
* This function adopted from wikipedia.
* https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
*/
Intersection Triangle::intersection(Ray& ray, Rational t_min){
    
    Intersection in;

    // Compute t
    //auto t = dot(this->p1 - r.ori, this->normal) * (1 / dot(r.dir ,this->normal));

    // Setup the system
    auto edge1 = this->p2 - this->p1;
    auto edge2 = this->p3 - this->p1;
    auto h = cross(ray.dir, edge2);
    auto a = dot(edge1, h);

    if (a > -EPSILON && a < EPSILON){
        in.intersection_type = NO_P;
        return in;
    }

    auto f = 1/a;
    auto s = ray.ori - this->p1;
    auto q = cross(s, edge1);

    auto t = f * dot(edge2, q);
    if (t > t_min + EPSILON || t < 0){
        in.intersection_type = NO_P;
        return in;
    }

    auto u = f * dot(s,h);

    if (u < 0.0 || u > 1.0){
        in.intersection_type = NO_P;
        return in;
    }

    // auto q = cross(s, edge1);
    auto v = f * dot(ray.dir, q);

    if (v < 0.0 || u + v > 1.0){
        in.intersection_type = NO_P;
        return in;
    }

    // auto t = f * dot(edge2, q);
    // if (t > t_min + EPSILON || t < 0){
    //     in.intersection_type = NO_P;
    //     return in;
    // }

    // One point of intersection, of course
    in.intersection_type = ONE_P;
    in.t1 = t;

    // If we made it this far, t seems like a good candidate
    in.inter_p = ray.value_at(in.t1);
    in.normal = this->normal;
    in.mat_id = this->material_id;

    if (!(this->p1normal == Vector3d(0,0,0))){
        in.smooth_normal = normalize((1 - u - v)*this->p1normal + u*this->p2normal + v*this->p3normal);
        in.smooth_normal_available = true;
        //std::cout << "smooth normal added\n";
    }else{
        in.smooth_normal_available = false;
    }

    //std::cerr << "Returning intersection: " << t << in.t1 << std::endl;

    return in;
 }
    