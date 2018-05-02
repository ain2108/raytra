#include "point_light.h"


PointLight::PointLight(Rational r, Rational g, 
    Rational b, Point3d p){

        this->r = r;
        this->g = g;
        this->b = b;

        this->pos = p;

}

Vector3d PointLight::to_light(Point3d& from){
    return this->pos - from;
}

// // We need this function for polymorphism
// std::shared_ptr<Light> create_light(Rational r, Rational g, 
//     Rational b, Point3d p){
//         std::shared_ptr<Light> lp = std::make_shared<PointLight>(r, g, b, p);
//         return lp;
// }

Rational PointLight::get_light_multiplier(Vector3d to_light_v){
    return 1.0;
}

std::vector<Point3d> PointLight::sample_light(unsigned int n){
    std::vector<Point3d> samples;
    samples.push_back(this->pos);
    return samples;
}