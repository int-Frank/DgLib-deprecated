#ifndef DGMAKEGRID_H
#define DGMAKEGRID_H

namespace Dg
{
  //! Creates a grid of lines. Useful for rendering planes.
  //!
  //! @param[in] a_out Output points
  //! @param[in] a_dimension dimension on each side of the grid
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakeGrid(Real * a_out, int a_dimension, Real const a_bounds[4 * VectorSize])
  {
    int dim[2] = { a_dimension, a_dimension };
    MakeGrid(a_out, dim, a_bounds);
  }

  //! Creates a grid of lines. Useful for rendering planes.
  //! @param[in] a_out Output
  //! @param[in] a_dimension dimension on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakeGrid(Real * a_out, int a_dimension[2], Real const a_bounds[4 * VectorSize])
  {
    static_assert(VectorSize == 3 || VectorSize == 4, "Must have a vector size of 3 or 4");
    int nDrawsH = (a_dimension[0] + 1);
    int nDrawsV = (a_dimension[1] + 1);

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

    for (int i = 0; i < nDrawsH; ++i)
    {
      int h0 = (i * 4 * VectorSize);
      int h1 = (i * 4 * VectorSize) + VectorSize;

      Real frac = static_cast<Real>(i) / static_cast<Real>(a_dimension[0]);
      
      a_out[h0 + 0] = a_bounds[0] - frac * x_left;
      a_out[h0 + 1] = a_bounds[1] - frac * y_left;
      a_out[h0 + 2] = a_bounds[2] - frac * z_left;

      a_out[h1 + 0] = a_bounds[VectorSize + 0] - frac * x_right;
      a_out[h1 + 1] = a_bounds[VectorSize + 1] - frac * y_right;
      a_out[h1 + 2] = a_bounds[VectorSize + 2] - frac * z_right;

      if (VectorSize == 4)
      {
        a_out[h0 + 3] = a_out[h1 + 3] = static_cast<Real>(1.0);
      }
    }

    for (int i = 0; i < nDrawsV; ++i)
    {
      int v0 = (i * 4 * VectorSize) + VectorSize * 2;
      int v1 = (i * 4 * VectorSize) + VectorSize * 3;

      Real frac = static_cast<Real>(i) / static_cast<Real>(a_dimension[1]);

      a_out[v0 + 0] = a_bounds[0] + frac * x_top;
      a_out[v0 + 1] = a_bounds[1] + frac * y_top;
      a_out[v0 + 2] = a_bounds[2] + frac * z_top;

      a_out[v1 + 0] = a_bounds[VectorSize * 2 + 0] + frac * x_bottom;
      a_out[v1 + 1] = a_bounds[VectorSize * 2 + 1] + frac * y_bottom;
      a_out[v1 + 2] = a_bounds[VectorSize * 2 + 2] + frac * z_bottom;

      if (VectorSize == 4)
      {
        a_out[v0 + 3] = a_out[v1 + 3] = static_cast<Real>(1.0);
      }
    }
  }
}

#endif