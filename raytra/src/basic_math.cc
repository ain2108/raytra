#include "basic_math.h"


// Vector3d MEMBERS
Vector3d::Vector3d(){
    x = 0;
    y = 0;
    z = 0;
}

Vector3d::Vector3d(double xval, double yval, double zval){
    x = xval;
    y = yval;
    z = zval; 
}


// Sum of two vectors is a vector of sum of the components
Vector3d operator+(const Vector3d& lhs, const Vector3d rhs){
    return Vector3d(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

// Difference of two vectors
Vector3d operator-(const Vector3d& lhs, const Vector3d rhs){
    return Vector3d(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

// Equality of two vectors
bool operator==(const Vector3d& lhs, const Vector3d rhs){
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

// The opposite vector
Vector3d operator-(const Vector3d& rhs){
    return Vector3d(-rhs.x, -rhs.y, -rhs.z);
}

// Scalar multiplication
Vector3d operator*(double scalar, const Vector3d& vec){
    return Vector3d(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}

Vector3d operator*(const Vector3d& vec, double scalar){
    return Vector3d(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}

// Scalar division

Vector3d operator/(double scalar, const Vector3d& vec){
    return Vector3d(scalar / vec.x, scalar / vec.y, scalar / vec.z);
}

Vector3d operator/(const Vector3d& vec, double scalar){
    return Vector3d(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

std::ostream& operator<<(std::ostream &os, const Vector3d& p){
    return os << "<" <<p.x<< "," <<p.y<< "," <<p.z<< ">";
}

std::istream &operator>>(std::istream &is, Vector3d &v){
    return is >> v.x >> v.y >> v.z;
}

Vector3d normalize(const Vector3d& vec){
    //auto norm = vec.l2norm();
    // TODO: Something causing a probelm here. Bring this asser back!
    // assert(!norm == 0);

    return (vec * (1 / vec.l2norm()));
}

Vector3d invert(const Vector3d& vec){
    return Vector3d(-1 * vec.x, -1 * vec.y, -1 * vec.z);
}



Point3d::Point3d(){
    x = 0;
    y = 0;
    z = 0;
}

Point3d::Point3d(double xval, double yval, double zval){
    x = xval;
    y = yval;
    z = zval; 
}

Point3d operator+(const Point3d& p, const Vector3d& vec){
    return Point3d(p.x + vec.x, p.y + vec.y, p.z + vec.z);
}

Point3d operator+(const Vector3d& vec, const Point3d& p){
    return Point3d(p.x + vec.x, p.y + vec.y, p.z + vec.z);
}

std::ostream& operator<<(std::ostream &os, const Point3d& p){
    return os << "(" <<p.x<< "," <<p.y<< "," <<p.z<< ")";
}

Vector3d operator-(const Point3d& p1, const Point3d& p2){
    return Vector3d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

std::istream& operator>>(std::istream &is, Point3d &p){
    return is >> p.x >> p.y >> p.z;
}

// void dprint(std::string msg){
//     if (DEBUG_PRINT == 1)
//         std::cout << msg << std::endl;
// }
