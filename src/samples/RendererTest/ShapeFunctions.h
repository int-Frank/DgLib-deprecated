#ifndef SHAPEFUNCTIONS_H
#define SHAPEFUNCTIONS_H

#include <cmath>

#include "Mod_Context_Line.h"
#include "Mod_Context_Triangle.h"

//Shapes will be centered around the origin, with a maximum 
//dimension of 1
Context::LineMesh GenerateLineStar();
Context::LineMesh GenerateLineBox();
Context::LineMesh GenerateLineDebugPattern();
Context::LineMesh GenerateLineWheel();
Context::LineMesh GenerateLineSpiral();

Context::TriangleMesh GenerateFilledShape(int nSides);

#endif