#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>

#include "camera.h"
#include "sphere.h"
#include "model.h"

//
// Basic parser for the scene files:
//
class Parser {
public:
    virtual void parse(
                       const char *file,
                       Camera &cam,
                       Model& model);
};

bool read_wavefront_file (
    const char *file,
    std::vector< int > &tris,
    std::vector< float > &verts);

#endif
