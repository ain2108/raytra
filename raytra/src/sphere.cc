#include "sphere.h"
#include <iostream>

Sphere::Sphere(){
    this->c = Point3d(0, 0, 0);
    this->r = 0;
}

Sphere::Sphere(Point3d center, Rational radius, int m_id){
    this->c = center;
    this->r = radius;
    this->material_id = m_id;
}


// TODO: OPTIMIZE
double Sphere::compute_discriminant(Ray& r){
    auto difv = r.ori - this->c;
    auto b_sq = dot(r.dir, difv) * dot(r.dir, difv);
    auto the4ac = dot(r.dir, r.dir)*(dot(difv, difv) - this->r * this->r);

    return (b_sq - the4ac);
}

// TODO: Return Intersection object
Intersection Sphere::intersection(Ray& r, Rational t_min){

    Intersection inter;

    auto difv = r.ori - this->c;
    auto b_sq = dot(r.dir, difv) * dot(r.dir, difv);
    auto the4ac = dot(r.dir, r.dir)*(dot(difv, difv) - this->r * this->r);
    
    auto dis = (b_sq - the4ac);

    // The ray does not hit the sphere
    if(dis < 0){
        inter.intersection_type = NO_P;
        return inter;
    // If the ray hits a single point
    }else if(dis == 0){
        inter.intersection_type = ONE_P;
        inter.t1 = dot(-r.dir, difv) / dot(r.dir, r.dir);
        
        if(inter.t1 > t_min){
            inter.intersection_type = NO_P;
            return inter;
        }


        // If t is negative
        if(inter.t1 < 0){
            inter.intersection_type--;
            return inter;
        }


    // If the ray hits through the sphere
    }else{

        // Set the intersection type to TWO_P
        inter.intersection_type = TWO_P;
        inter.t1 = (dot(-r.dir, difv) - sqrt(dis)) / dot(r.dir, r.dir);

        if(inter.t1 > t_min){
            inter.intersection_type = NO_P;
            return inter;
        }

        inter.t2 = (dot(-r.dir, difv) + sqrt(dis)) / dot(r.dir, r.dir);
        
        // If t is negative, the first intersection is behind the
        // camera
        if(inter.t1 < 0){
           
            inter.t1 = inter.t2;
            inter.intersection_type--;

            if(inter.t1 < 0){
                inter.intersection_type--;
                return inter;
            }
        }
        
    }

    inter.inter_p = r.value_at(inter.t1);
    inter.normal = normalize((inter.inter_p - this->c) * (1/this->r));
    inter.mat_id = this->material_id;
    inter.smooth_normal_available = false;

    return inter;
}

std::ostream& operator<<(std::ostream &os, const Sphere& s){
    return std::cout << "radius: " << s.r << "\t center: " << s.c;
}



