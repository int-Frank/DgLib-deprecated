#ifndef TYPES_H
#define TYPES_H

#include "DgR2Vector.h"
#include "DgR3Vector.h"
#include "DgR2Disk.h"
#include "DgR2Line.h"
#include "DgR3Matrix.h"

typedef Dg::R2::Vector<float> vec3;
typedef Dg::R3::Vector<float> vec4;
typedef Dg::R3::Matrix<float> mat4;
typedef Dg::R2::Disk<float>   Disk;
typedef Dg::R2::Line<float>   Line;

#endif