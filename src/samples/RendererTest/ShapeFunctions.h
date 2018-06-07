#ifndef SHAPEFUNCTIONS_H
#define SHAPEFUNCTIONS_H

#include <cmath>

#include "Mod_Renderer_LineMesh.h"

Renderer::LineMesh GenerateStar();
Renderer::LineMesh GenerateBox();
Renderer::LineMesh GenerateDebugPattern();
Renderer::LineMesh GenerateSnowFlake();
Renderer::LineMesh GenerateFace();

#endif