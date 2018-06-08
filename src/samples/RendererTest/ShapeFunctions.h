#ifndef SHAPEFUNCTIONS_H
#define SHAPEFUNCTIONS_H

#include <cmath>

#include "Mod_Renderer_LineMesh.h"

//Shapes will be centered around the origin, with a maximum 
//dimension of 1
Renderer::LineMesh GenerateStar();
Renderer::LineMesh GenerateBox();
Renderer::LineMesh GenerateDebugPattern();
Renderer::LineMesh GenerateWheel();
Renderer::LineMesh GenerateSpiral();

#endif