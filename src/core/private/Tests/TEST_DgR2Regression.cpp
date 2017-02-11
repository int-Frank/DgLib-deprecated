#include "TestHarness.h"
#include "DgR2Regression.h"

typedef Dg::R2::Vector<float> vec2;
typedef Dg::R2::Line<float> line;

TEST(Stack_DgR2Regression, DgR2Regression)
{
  vec2 points[4] = { vec2(1.f, 3.f, 1.f), 
                     vec2(2.f, 5.f, 1.f), 
                     vec2(3.f, 5.f, 1.f),
                     vec2(1.5f, 1.7f, 1.f) };


  line l = Dg::R2::LineOfBestFit(points, 4);
}