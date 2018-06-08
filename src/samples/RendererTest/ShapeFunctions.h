#ifndef SHAPEFUNCTIONS_H
#define SHAPEFUNCTIONS_H

#include <cmath>

#include "Mod_Renderer_LineMesh.h"
#include "Mod_Renderer_TriangleMesh.h"

//Shapes will be centered around the origin, with a maximum 
//dimension of 1
Renderer::LineMesh GenerateLineStar();
Renderer::LineMesh GenerateLineBox();
Renderer::LineMesh GenerateLineDebugPattern();
Renderer::LineMesh GenerateLineWheel();
Renderer::LineMesh GenerateLineSpiral();

Renderer::TriangleMesh GenerateFilledShape(int nSides);

#endif