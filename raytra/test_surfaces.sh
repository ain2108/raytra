#!/bin/bash
mkdir results

# Testing the spheres
./raytra spheres_arealight.scn ./results/spheres00.exr 0 0
./raytra spheres_arealight.scn ./results/spheres11.exr 1 1
./raytra spheres_arealight.scn ./results/spheres22.exr 2 2
./raytra spheres_arealight.scn ./results/spheres33.exr 3 3

# Testing lego
./raytra ./fixedLego.scn ./results/lego00.exr 0 0
./raytra ./fixedLego.scn ./results/lego11.exr 1 1
./raytra ./fixedLego.scn ./results/lego22.exr 2 2

# Testing the car
./raytra ./fixedTeapot.scn ./results/teapot.exr 3 3

# Test the lamp
./raytra ./streetlamp_scene.scn ./results/lamp.exr 3 3

# Test the bunny
./raytra ./bunny.scn  ./results/bunny.exr 3 3
