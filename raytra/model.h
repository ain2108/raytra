#ifndef MODEL_H
#define MODEL_H

#include "sphere.h"
#include "light.h"
#include "material.h"
#include "triangle.h"
#include "bvh.h"

#include <vector>
#include <memory>
#include <iostream>
#include <limits>
#include <random>
#include <iostream>

class Model {

public: 

    // Decided not do surface polymorphism. Extra indirection
    // and cache misses. 
    std::vector<Sphere> spheres;
    //std::vector<Plane> planes;
    std::vector<Triangle> triangles;

    // Since lights are few, an extra indirection should be fine.
    // Lights themselves are HEAP allocated
    std::vector<Light *> lights;
    
    std::vector<Material> materials;

    // This is HEAP allocated
    BVHNode* bvh_root;

    // Model id
    int id;

    void add_sphere(Sphere s);
    void add_triangle(Triangle t);
    void add_light(Light *lp);
    void add_material(Material m);

    void build_bvh();

    Intersection find_intersect(Ray& r);
    bool shadow_intersect(Ray& ray, double d_to_src);

    ~Model();

    void stats();
};


#endif