#include "basic_math.h"
#include "parser.h"
#include "ray.h"
#include "intersection.h"
#include "point_light.h"
#include "exr_film.h"
#include "model.h"
#include "bbox.h"
#include "bvh.h"

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

#include <chrono>
#include <iostream>
#include <string>
#include <cmath>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>    

using namespace std;
using namespace Imf;
using namespace Imath;

using us = chrono::microseconds;
using ms = chrono::milliseconds;
using get_time = chrono::steady_clock;




void
writeRgba (const char fileName[],
           const Rgba *pixels,
           int width,
           int height)
{
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //
    
    
    RgbaOutputFile file (fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}

void usage(){
    cout << "usage: ./raytra <scene.scn> <out.exr> <bvh_flag:(0 or 1)> <samples_per_primary> <samples_per_shadow>" << endl;    
}


// Took this conversion function form the fine gentleman Dan Moulding
// https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c/6154614#6154614
enum STR2INT_ERROR { STR2INT_SUCCESS, STR2INT_OVERFLOW, STR2INT_UNDERFLOW, STR2INT_INCONVERTIBLE };
STR2INT_ERROR str2int (int &i, char const *s, int base = 0)
{
    char *end;
    long  l;
    errno = 0;
    l = strtol(s, &end, base);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return STR2INT_OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return STR2INT_UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0') {
        return STR2INT_INCONVERTIBLE;
    }
    i = l;
    return STR2INT_SUCCESS;
}


void get_samples_input(int argc, char * argv[], 
    int &primary_sampling, int &shadow_sampling){

    switch(argc){
        int i;
        STR2INT_ERROR err;
        case 3:
            primary_sampling = DEFAULT_SAMPLES_PER_PRIMARY;
            shadow_sampling = DEFAULT_SAMPLES_PER_SHADOW;
            break;
        case 4:
            i = 0;
            err = str2int(i, argv[3], 10);
            if (err != STR2INT_SUCCESS){
                usage();
                exit(0);
            }
            if (i == 0){
                primary_sampling = 0;
            }else{
                primary_sampling = i*i;
            }

            shadow_sampling = DEFAULT_SAMPLES_PER_SHADOW;
            break;
        
        case 5:

            i = 0;
            err = str2int(i, argv[3], 10);
            if (err != STR2INT_SUCCESS){
                usage();
                exit(0);
            }

            if (i == 0){
                primary_sampling = 0;
            }else{
                primary_sampling = i*i;
            }

            err = str2int(i, argv[4], 10);
            if (err != STR2INT_SUCCESS){
                usage();
                exit(0);
            }
            
            if (i == 0){
                shadow_sampling = 0;
            }else{
                shadow_sampling = i*i;
            }
    }
}

int main(int argc, char * argv[]){

    char * file_scn;
    char * file_exr;
    if(argc < 3 || argc > 5){
        usage();
        return 0;
    }else{
        file_scn = argv[1];
        file_exr = argv[2];
    }    

    srand (time(NULL));

    // Get the parser up
    auto parser = Parser();

    Camera cam;
    Model model;

    // Parse the .scn file
    parser.parse(file_scn, cam, model);

    // Get some stats
    //model.stats();

    // Get the film
    ExrFilm film;
    film.init_film(cam.nx, cam.ny);

    cout << "Shooting..." << endl;
    auto start = get_time::now();

#ifdef NO_BVH_ACCELERATION
    std::cout << "BVH: OFF" << std::endl;
    model.bvh_root = nullptr;
#else
    model.build_bvh();
    std::cout << "BVH: ON" << std::endl;
#endif

get_samples_input(argc, argv, cam.SAMPLES_PER_PRIMARY, cam.SAMPLES_PER_SHADOW);
std::cout << "Primary Samples: " << cam.SAMPLES_PER_PRIMARY << " Shadow Sampling: " << cam.SAMPLES_PER_SHADOW << std::endl;


   // std::cout << "Primary Samples: " << cam.SAMPLES_PER_PRIMARY << " Shadow Sampling: " << cam.SAMPLES_PER_SHADOW << std::endl;
    cam.shoot(film, model);
    auto end = get_time::now();
    cout << "Time elapsed: " << chrono::duration_cast<ms>(end - start).count() << " ms "<< endl;

    writeRgba(file_exr, &film.pixels[0][0], cam.nx, cam.ny);


    return 0;
}
