#ifndef EXR_FILM_H
#define EXR_FILM_H

#include <ImfArray.h>
#include <ImfMatrixAttribute.h>
#include <ImfStringAttribute.h>
#include <ImfRgbaFile.h>

#include "film.h"

class ExrFilm : public Film {

public:
    void init_film(int width, int height);
    void write_pixel(int i, int j, Rational r, Rational g, Rational b, Rational a);

    Imf::Array2D<Imf::Rgba> pixels;
};

#endif