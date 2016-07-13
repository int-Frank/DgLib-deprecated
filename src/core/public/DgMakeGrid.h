#ifndef DGMAKEGRID_H
#define DGMAKEGRID_H

namespace Dg
{
  //! Creates a grid of lines. 
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakeGrid(Real * a_data, int a_dimension, Real const a_bounds[4 * VectorSize])
  {
    static_assert(VectorSize == 3 || VectorSize == 4, "Must have a vector size of 3 or 4");
    int nDraws = (a_dimension + 1);

    Real x_top = a_bounds[VectorSize] - a_bounds[0];
    Real x_bottom = a_bounds[VectorSize * 3] - a_bounds[VectorSize * 2];
    Real x_left = a_bounds[0] - a_bounds[VectorSize * 2];
    Real x_right = a_bounds[VectorSize] - a_bounds[VectorSize * 3];

    Real y_top = a_bounds[VectorSize + 1] - a_bounds[0 + 1];
    Real y_bottom = a_bounds[VectorSize * 3 + 1] - a_bounds[VectorSize * 2 + 1];
    Real y_left = a_bounds[0 + 1] - a_bounds[VectorSize * 2 + 1];
    Real y_right = a_bounds[VectorSize + 1] - a_bounds[VectorSize * 3+ 1];

    Real z_top = a_bounds[VectorSize + 2] - a_bounds[0 + 2];
    Real z_bottom = a_bounds[VectorSize * 3 + 2] - a_bounds[VectorSize * 2 + 2];
    Real z_left = a_bounds[0 + 2] - a_bounds[VectorSize * 2 + 2];
    Real z_right = a_bounds[VectorSize + 2] - a_bounds[VectorSize * 3 + 2];

    for (int i = 0; i < nDraws; ++i)
    {
      int h0 = (i * 4 * VectorSize);
      int h1 = (i * 4 * VectorSize) + VectorSize;
      int v0 = (i * 4 * VectorSize) + VectorSize * 2;
      int v1 = (i * 4 * VectorSize) + VectorSize * 3;

      Real frac = static_cast<Real>(i) / static_cast<Real>(a_dimension);

      a_data[h0 + 0] = a_bounds[0] - frac * x_left;
      a_data[h0 + 1] = a_bounds[1] - frac * y_left;
      a_data[h0 + 2] = a_bounds[2] - frac * z_left;

      a_data[h1 + 0] = a_bounds[VectorSize + 0] - frac * x_right;
      a_data[h1 + 1] = a_bounds[VectorSize + 1] - frac * y_right;
      a_data[h1 + 2] = a_bounds[VectorSize + 2] - frac * z_right;

      a_data[v0 + 0] = a_bounds[0] + frac * x_top;
      a_data[v0 + 1] = a_bounds[1] + frac * y_top;
      a_data[v0 + 2] = a_bounds[2] + frac * z_top;

      a_data[v1 + 0] = a_bounds[VectorSize * 2 + 0] + frac * x_bottom;
      a_data[v1 + 1] = a_bounds[VectorSize * 2 + 1] + frac * y_bottom;
      a_data[v1 + 2] = a_bounds[VectorSize * 2 + 2] + frac * z_bottom;

      if (VectorSize == 4)
      {
        a_data[h0 + 3] = static_cast<Real>(1.0);
        a_data[h1 + 3] = static_cast<Real>(1.0);
        a_data[v0 + 3] = static_cast<Real>(1.0);
        a_data[v1 + 3] = static_cast<Real>(1.0);
      }
    }
  }
}

#endif