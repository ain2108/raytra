#include "area_light.h"

AreaLight::AreaLight(Point3d pos,
    Vector3d dir,
    Vector3d udir,
    Rational length, 
    Rational r, Rational g, Rational b){

        this->pos = pos;
        this->dir = normalize(dir);
        this->length = length;
        this->udir = normalize(udir);
        this->vdir = normalize(cross(this->udir, this->dir));
        this->r = r;
        this->g = g;
        this->b = b;

        this->origin =  (this->pos + (-this->udir*(this->length/2))) + (-this->vdir*(this->length/2));

    }

std::vector<Point3d> AreaLight::sample_light(unsigned int n){
    std::vector<Point3d> samples;

    if (n == 0){
        samples.push_back(this->pos);
        return samples;
    }

    auto cell_length = this->length / n;
    auto short_udir = (this->length * this->udir) / n;
    auto short_vdir = (this->length * this->vdir) / n;

    for (unsigned int p = 0; p < n; p++){
        for (unsigned int q = 0; q < n; q++){
            auto uv =  short_udir * p + short_udir * (random_number() * cell_length);
            auto vv =  short_vdir * q + short_vdir * (random_number() * cell_length);
            auto sample = origin + uv + vv;
            samples.push_back(sample);
        }
    }

    return samples;

}

Rational AreaLight::get_light_multiplier(Vector3d to_light_v){
    auto lm = dot(normalize(to_light_v), this->dir);
    if (lm < 0){
        lm = -lm;
    }else{
        lm = 0;
    }
    return lm;
}

Vector3d AreaLight::to_light(Point3d& from){
    // std::cerr << "Function not supported\n";
    // abort();
    return this->pos - from;
}