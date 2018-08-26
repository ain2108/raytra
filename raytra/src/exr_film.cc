#include "exr_film.h"

#include <iostream>

void ExrFilm::init_film(int width, int height){
    this->pixels.resizeErase(height, width);
    this->height = height;
    this->width =  width;
}

void ExrFilm::write_pixel(int i, int j, Rational r, Rational g, Rational b, Rational a){
    Imf::Rgba &px = this->pixels[this->height - 1 - j][i];
    px.r = r;
    px.g = g;
    px.b = b;
    px.a = a;
}
