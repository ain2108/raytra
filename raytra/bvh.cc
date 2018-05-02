#include "bvh.h"
#include <algorithm>

BVHNode::BVHNode(BVHNode* left, BVHNode *right, BBox bbox){
    this->left = left;
    this->right = right;
    this->bbox = bbox;
}

BVHNode::BVHNode(){
    this->left = nullptr;
    this->right = nullptr;
}

#define RESERVE_NODES 50
void find_intersection_surfaces_NO_REC(Ray& ray, 
    std::vector<Surface *>& surfs, BVHNode * root){

        auto current = root;

        std::vector<BVHNode *> stack;
        stack.reserve(50);
        stack.push_back(root);

        while(stack.size() > 0){

            current = stack.back();
            stack.pop_back();

            if (current->bbox.check_intersetcion(ray) == false){
                continue;
            }

            // Push the children onto the stack
            if (current->left != nullptr){
                stack.push_back(current->left);
            }

            if (current->right != nullptr){
                stack.push_back(current->right);
            }

            if (current->bbox.surf != nullptr){
                surfs.push_back(current->bbox.surf);
            }
        }
}

void BVHNode::find_intersection_surfaces(Ray& ray, 
    std::vector<Surface *>& surfs){

        //std::cerr << "Looking for intersection!\n";

        // If we do not intersect the bbox, we are done.
        if (this->bbox.check_intersetcion(ray) == false){
            return;
        }

        //std::cerr << "Box hit!\n";


        // For leafs, this will point to their surface
        if (this->bbox.surf != nullptr) {
            //std::cerr << "Surface added: " << bbox.surf << std::endl;
            surfs.push_back(this->bbox.surf);
        }

        if (this->left != nullptr){
            this->left->find_intersection_surfaces(ray, surfs);
        }

        if (this->right != nullptr){
            this->right->find_intersection_surfaces(ray, surfs);
        }

        return;
}

// The function builds the subtree
// NOTE: Function dependent of RANDOM ACCESS ITERATORS
BVHNode* make_node(BBox_iterator first, BBox_iterator last, Axis ax){
    
    // Base case
    if (first == last) {
        return nullptr;
    }

    // If this node is a leaf
    if (last - first == 1) {
        return new BVHNode(nullptr, nullptr, (*first));
    }

    // Create a node and assign the bbox
    auto bvh_node_p = new BVHNode(nullptr, nullptr, BBox(first, last));

    // NOTE: Because vector iterators are random acceess
    auto middle = first + (last - first) / 2;

    // Partition around the middle
    std::nth_element(first, middle, last, 
        [&ax](const BBox& a, const BBox& b) -> bool {
            return axis_compare(a, b, ax);
        });

    bvh_node_p->left = make_node(first, middle, rotate_axis(ax));
    bvh_node_p->right = make_node(middle, last, rotate_axis(ax));

    return bvh_node_p;
}



std::ostream& operator<<(std::ostream &os, const BVHNode& n){

    //std::cerr << "hello" << std::endl;

    if(n.left == nullptr && n.right == nullptr ) {
        return os << "{ ()" << "|" << n.bbox << "|" << "() }";        
    }

    if(n.left == nullptr) {
        return os << "{ ()" << "|" << n.bbox << "|" << *n.right <<"}";        
    }

    if(n.right == nullptr) {
        return os << "{ ()" << "|" << n.bbox << "|" << "() }";        
    }

    return os << "{" << *n.left << "|" << n.bbox << "|" << *n.right <<"}";
}