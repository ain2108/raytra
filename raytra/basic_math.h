#ifndef BASIC_MATH_H
#define BASIC_MATH_H


#define DEBUG_PRINT 0

#include <cmath>
#include <iostream>
#include <cassert>
#include <string>

using Rational = double;

class Vector3d;

using Axis = const char;
Axis X_AXIS = 'X';
Axis Y_AXIS = 'Y';
Axis Z_AXIS = 'Z';


// Sum of two vectors is a vector of sum of the components
Vector3d operator+(const Vector3d& lhs, const Vector3d rhs);

// Difference of two vectors
Vector3d operator-(const Vector3d& lhs, const Vector3d rhs);

// The opposite vector
Vector3d operator-(const Vector3d& rhs);

// Equality of two vectors
bool operator==(const Vector3d& lhs, const Vector3d rhs);

// Scalar multiplication
Vector3d operator*(double scalar, const Vector3d& vec);
Vector3d operator*(const Vector3d& vec, double scalar);

Vector3d operator/(double scalar, const Vector3d& vec);
Vector3d operator/(const Vector3d& vec, double scalar);
        

std::ostream& operator<<(std::ostream &os, const Vector3d& p);
std::istream& operator>>(std::istream &is, Vector3d &v);

// Normalize
Vector3d normalize(const Vector3d& vec);
Vector3d invert(const Vector3d& vec);


class Vector3d {
    
public:
    Rational x;
    Rational y;
    Rational z;


    Vector3d();

    Vector3d(double xval, double yval, double zval);

    inline double l2norm() const {
        return (std::sqrt(x*x + y*y + z*z));
    }

    
 };



class Point3d {
    
public:
    Rational x;
    Rational y;
    Rational z;

    Point3d();

    Point3d(double xval, double yval, double zval);

};

//Cross product: lhs x rhs
inline Vector3d cross(const Vector3d& lhs, const Vector3d& rhs){
    auto crossv = Vector3d();
    crossv.x = lhs.y * rhs.z - lhs.z * rhs.y;
    crossv.y = lhs.z * rhs.x - lhs.x * rhs.z;
    crossv.z = lhs.x * rhs.y - lhs.y * rhs.x;
    return crossv;
}

// Dot Product: a . b
inline Rational dot(const Vector3d& lhs, const Vector3d& rhs){
    return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
}


Point3d operator+(const Point3d& p, const Vector3d& vec);
Point3d operator+(const Vector3d& vec, const Point3d& p);
Vector3d operator-(const Point3d& p1, const Point3d& p2);
std::ostream& operator<<(std::ostream &os, const Point3d& p);
std::istream& operator>>(std::istream &is, Point3d &v);


// More utility functions
inline Rational determinant(const Vector3d& c1, 
    const Vector3d& c2, const Vector3d& c3){
        return c1.x*(c2.y*c3.z - c3.y*c2.z) -
               c2.x*(c1.y*c3.z - c3.y*c1.z) +
               c3.x*(c1.y*c2.z - c2.y*c1.z);
}

inline double random_number(){
    return ((double) std::rand()) / RAND_MAX;
}

// void dprint(std::string msg);



#endif
