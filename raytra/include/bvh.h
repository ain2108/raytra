#ifndef BVH_H
#define BVH_H

#include "bbox.h"
#include "surface.h"
#include "ray.h"

#include <vector>


// TODO: Maybe try to avoid heap allocations by building a heap?
class BVHNode {
public:
    BVHNode *left;
    BVHNode *right;
    
    BBox bbox;

    // Builds the whole subtree
    BVHNode();
    BVHNode(BVHNode* left, BVHNode *right, BBox bbox);
    ~BVHNode(){delete left; delete right;}
        
    
    // Build
    void find_intersection_surfaces(Ray& ray, 
        std::vector<Surface *>& surfs);

};

BVHNode* make_node(BBox_iterator first, BBox_iterator last, Axis ax);
std::ostream& operator<<(std::ostream &os, const BVHNode& n);
void find_intersection_surfaces_NO_REC(Ray& ray, 
    std::vector<Surface *>& surfs, BVHNode * root);

#endif