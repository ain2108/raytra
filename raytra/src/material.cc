#include "material.h"

Material::Material( 
    // Diffuse coefficients
    double diff_r, double diff_g, double diff_b,
    // Specular coefficients
    double spec_r, double spec_g, double spec_b, double phong,
    // Ambient light
    double ideal_r, double ideal_g, double ideal_b){

        this->spec_r = spec_r;
        this->spec_g = spec_g;
        this->spec_b = spec_b;
        this->phong = phong;

        this->diff_r = diff_r;
        this->diff_g = diff_g;
        this->diff_b = diff_b;

        this->ideal_r = ideal_r;
        this->ideal_g = ideal_g;
        this->ideal_b = ideal_b;

}