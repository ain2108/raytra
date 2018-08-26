#include "model.h"

Model::~Model(){
    if(bvh_root != nullptr){
        delete bvh_root;
    }

    for(auto l : this->lights){
        delete l;
    }

}

void Model::add_sphere(Sphere s){
    //std::cout << "add sphere" << std::endl;
    this->spheres.push_back(s);
}

void Model::add_triangle(Triangle t){
    this->triangles.push_back(t);
}

void Model::add_light(Light* lp){
    this->lights.push_back(lp);
}
void Model::add_material(Material m){
    this->materials.push_back(m);
}

void Model::build_bvh(){

    std::vector<BBox> bboxes;

    // We build bounding boxes for each shape
    unsigned int i = 0;
    for(i = 0; i < this->spheres.size(); i++){

        auto sphere = this->spheres[i];
        
        auto center = sphere.c;
        auto min_x = center.x - sphere.r;
        auto max_x = center.x + sphere.r;

        auto min_y = center.y - sphere.r;
        auto max_y = center.y + sphere.r;

        auto min_z = center.z - sphere.r;
        auto max_z = center.z + sphere.r;

        bboxes.push_back(BBox(min_x, max_x, min_y, max_y, min_z, max_z, &this->spheres[i]));

    }

    
    for(i = 0; i < this->triangles.size(); i++){

        auto tr = this->triangles[i];

        auto min_x = std::min(tr.p1.x, std::min(tr.p2.x, tr.p3.x));
        auto max_x = std::max(tr.p1.x, std::max(tr.p2.x, tr.p3.x));

        auto min_y = std::min(tr.p1.y, std::min(tr.p2.y, tr.p3.y));
        auto max_y = std::max(tr.p1.y, std::max(tr.p2.y, tr.p3.y));

        auto min_z = std::min(tr.p1.z, std::min(tr.p2.z, tr.p3.z));
        auto max_z = std::max(tr.p1.z, std::max(tr.p2.z, tr.p3.z));

        bboxes.push_back(BBox(min_x, max_x, min_y, max_y, min_z, max_z, &this->triangles[i]));        
    }

    //std::cerr << "Built " << bboxes.size() << " bounding boxes\n";
    this->bvh_root = make_node(bboxes.begin(), bboxes.end(), X_AXIS);

    return;
}

void Model::stats(){
    std::cout << "Printing model stats:" << std::endl;
    std::cout << "Number of Spheres: " << this->spheres.size() << std::endl;
    std::cout << "Number of Lights " << this->lights.size() << std::endl;
    std::cout << "Number of Materials " << this->materials.size() << std::endl;
}

Intersection Model::find_intersect(Ray& ray){
    
    Intersection intersection;
    intersection.intersection_type = NO_P;
    double t_min = std::numeric_limits<Rational>::max();
    
    // TODO: Get rid of this conditional by moving
    // this decision further up the stack
    // Use the accelerator
    //std::cerr << "Looking for intersect\n";
    if (this->bvh_root != nullptr){
        std::vector<Surface *> candidates;

#ifndef NONRECURSIVE_BVH
        this->bvh_root->find_intersection_surfaces(ray, candidates);
#else        
        find_intersection_surfaces_NO_REC(ray, candidates, this->bvh_root);
#endif
        //std::cerr << "done adding\n";

        for(auto su : candidates){
            
            auto temp = su->intersection(ray, t_min);
            //std::cerr << "looking for intersection\n";
            if(temp.intersection_type >= ONE_P){
                t_min = temp.t1;
                intersection = temp;
                //std::cerr << "found smth\n";
            }
        }

        //std::cerr << "Returning intersection\n";

        return intersection;
    }


    // Intersect each sphere and look for the smallest t
    for(auto x : this->spheres){
        auto temp = x.intersection(ray, t_min);
        if(temp.intersection_type >= ONE_P){
            t_min = temp.t1;
            intersection = temp;
        }
    }

    // Lets now intersect each triangle
    for(auto x : this->triangles){
        auto temp = x.intersection(ray, t_min);
        if(temp.intersection_type == ONE_P){
            t_min = temp.t1;
            intersection = temp;
            //std::cout << "updating intersection" << std::endl;
        }
    }



    return intersection;

}


bool Model::shadow_intersect(Ray& ray, double d_to_src){
    
    double t_min = std::numeric_limits<double>::max();


    if (this->bvh_root != nullptr){
        std::vector<Surface *> candidates;

#ifdef NONRECURSIVE_BVH
        this->bvh_root->find_intersection_surfaces(ray, candidates);
#else        
        find_intersection_surfaces_NO_REC(ray, candidates, this->bvh_root);
#endif

        for(auto su : candidates){
            auto temp = su->intersection(ray, t_min);
            if(temp.intersection_type >= ONE_P){
                if (d_to_src < temp.t1) {
                    //std::cout << "False Shadow" << std::endl;                
                    return false;
                }else{
                    return true;
                }
            }
        }

        //std::cerr << "Returning intersection\n";

        return false;
    }



    // Shadow ray hits a sphere
    for(auto x : this->spheres){
        auto temp = x.intersection(ray, t_min);
        if(temp.intersection_type >= ONE_P){
            if (d_to_src < temp.t1) {
                //std::cout << "False Shadow" << std::endl;                
                return false;
            }else{
                return true;
            }
        }
    }

    // Shadow ray hits a triangle?
    for(auto x : this->triangles){
        auto temp = x.intersection(ray, t_min);
        if(temp.intersection_type >= ONE_P){
            if (d_to_src < temp.t1) {
                //std::cout << "False Shadow" << std::endl;
                return false;
            }else{
                return true;
            }
        }
    }



    return false;

}