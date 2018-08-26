#ifndef MATERIAL_H
#define MATERIAL_H


class Material {

public:

    Material(double, double, double, double,
            double, double, double,
            double, double, double);

    // Diffuse coefficients
    double diff_r;
    double diff_g;
    double diff_b;

    // Specular coefficients
    double spec_r;
    double spec_g;
    double spec_b;

    double phong;

    // Ambient light
    double ideal_r;
    double ideal_g;
    double ideal_b;

    int id;

};






#endif