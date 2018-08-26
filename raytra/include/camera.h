#ifndef CAMERA_H
#define CAMERA_H

#include "basic_math.h"
#include "ray.h"
#include "film.h"
#include "model.h"

#include <cmath>


#define DEFAULT_REFLECTION_DEPTH 6
#define DEFAULT_SAMPLES_PER_PRIMARY 4
#define DEFAULT_SAMPLES_PER_SHADOW 1

class Camera {
    
public:
    Point3d eye;

    double d; // Focal Length

    // ALL NORMALIZED
    Vector3d w; // away from the direction of the camera 
    Vector3d v; // UP of the camera
    Vector3d u; 

    int nx; // Width of the image in pixels
    int ny; // Height of the image in pixels

    Rational l, r, b, t;

    // Configs
    unsigned int ref_depth;
    int SAMPLES_PER_PRIMARY;
    int SAMPLES_PER_SHADOW;

    Camera(){}

    Camera(Point3d e, Vector3d dir, 
        Rational focal, Rational width, Rational height,
        unsigned pixel_width, unsigned pixel_height);

    void create_frame(Point3d e, Vector3d dir);

    Ray ray_through_pixel(unsigned int i, unsigned int j);
    std::vector<Ray> mc_ray_through_pixel(unsigned int i, unsigned int j, unsigned int n);

    void shoot(Film& f, Model& m);
    void compute_pixel(int i, int j, Film& f, Model& m, Intersection& in);
    Vector3d compute_radiance(Model& m, Ray& ray, unsigned int depth);
private:
    Rational get_u_scalar(unsigned int i);
    Rational get_v_scalar(unsigned int j);

};

std::ostream& operator<<(std::ostream &os, const Camera& cam);


#endif