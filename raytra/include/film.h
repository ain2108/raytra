#ifndef FILM_H
#define FILM_H

#include "basic_math.h"

class Film {

public:
    virtual void write_pixel(int i, int j, Rational r, Rational g, Rational b, Rational a) = 0;
    int width;
    int height;
};


#endif