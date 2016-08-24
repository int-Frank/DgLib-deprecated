#ifndef DGMAKEGRID_H
#define DGMAKEGRID_H

#include "Dg_vector.h"

namespace Dg
{
  //! @ingroup DgUtility_functions
  //!
  //! Creates a list of triangle vertices from a square mesh, where each grid
  //! element is made up of vertices unigue to that element.
  //!
  //! @param[in] a_out Output indices
  //! @param[in] a_dimension number of vertices along each edge [w, h]
  template<typename INT, bool CW = true>
  void MakeGridIndUniqueElem(vector<INT> & a_out
                           , INT a_dimension[2])
  {
    a_out.clear();
    a_out.resize((a_dimension[0] - 1) * (a_dimension[1] - 1) * 2 * 3);
    INT nElems = (a_dimension[0] - 1) * (a_dimension[1] - 1);
    for (INT i = 0; i < nElems; ++i)
    {
      INT tl = i * 4;
      INT tr = tl + 1;
      INT br = tl + 2;
      INT bl = tl + 3;

      if (CW)
      {
        a_out.push_back(bl);
        a_out.push_back(tl);
        a_out.push_back(tr);

        a_out.push_back(bl);
        a_out.push_back(tr);
        a_out.push_back(br);
      }
      else
      {
        a_out.push_back(bl);
        a_out.push_back(tr);
        a_out.push_back(tl);

        a_out.push_back(bl);
        a_out.push_back(br);
        a_out.push_back(tr);
      }
    }
  }

  //! @ingroup DgUtility_functions
  //!
  //! Creates a grid of Points with each division containing unique vertices to that element.
  //!
  //! @param[in] a_out Output points
  //! @param[in] a_dimension number of elements on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakePointGridUniqueElem(vector<Real> & a_out, int a_dimension[2], Real const a_bounds[4 * VectorSize])
  {
    vector<Real> temp;
    int dim[2] = { a_dimension [0] + 1, a_dimension[1] + 1};
    MakePointGrid<Real, VectorSize>(temp, dim, a_bounds);

    a_out.clear();
    a_out.resize(a_dimension[0] * a_dimension[1] * 4 * VectorSize);
    for (int i = 0; i < a_dimension[1]; ++i)
    {
      for (int j = 0; j < a_dimension[0]; ++j)
      {
        int ind = i * (dim[0]) * VectorSize + j * VectorSize;
        a_out.push_back(temp[ind]);
        a_out.push_back(temp[ind + 1]);
        a_out.push_back(temp[ind + 2]);
        if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));

        ind += VectorSize;
        a_out.push_back(temp[ind]);
        a_out.push_back(temp[ind + 1]);
        a_out.push_back(temp[ind + 2]);
        if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));

        ind += (dim[0]) * VectorSize;
        a_out.push_back(temp[ind]);
        a_out.push_back(temp[ind + 1]);
        a_out.push_back(temp[ind + 2]);
        if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));

        ind -= VectorSize;
        a_out.push_back(temp[ind]);
        a_out.push_back(temp[ind + 1]);
        a_out.push_back(temp[ind + 2]);
        if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));
      }
    }
  }

  //! @ingroup DgUtility_functions
  //!
  //! Creates a list of triangle vertices from a square mesh
  //!
  //! @param[in] a_out Output indices
  //! @param[in] a_dimension number of points on each side of the grid [w, h]
  template<typename INT, bool CW = true>
  void MakeGridInd(vector<INT> & a_out
                 , INT a_dimension[2])
  {
    a_out.clear();
    a_out.resize((a_dimension[0] - 1) * (a_dimension[1] - 1) * 2 * 3);
    for (INT y = 0; y < a_dimension[1] - 1; ++y)
    {
      for (INT x = 0; x < a_dimension[0] - 1; ++x)
      {
        INT tl = y * (a_dimension[0]) + x;
        INT tr = tl + 1;
        INT bl = tl + a_dimension[0];
        INT br = bl + 1;

        if (CW)
        {
          a_out.push_back(bl);
          a_out.push_back(tl);
          a_out.push_back(tr);

          a_out.push_back(bl);
          a_out.push_back(tr);
          a_out.push_back(br);
        }
        else
        {
          a_out.push_back(bl);
          a_out.push_back(tr);
          a_out.push_back(tl);

          a_out.push_back(bl);
          a_out.push_back(br);
          a_out.push_back(tr);
        }
      }
    }
  }


  //! @ingroup DgUtility_functions
  //!
  //! Creates a grid of Points.
  //!
  //! @param[in] a_out Output points
  //! @param[in] a_dimension number of points on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakePointGrid(vector<Real> & a_out, int a_dimension, Real const a_bounds[4 * VectorSize])
  {
    int dim[2] = { a_dimension, a_dimension };
    MakePointGrid<Real, VectorSize>(a_out, dim, a_bounds);
  }


  //! @ingroup DgUtility_functions
  //!
  //! Creates a grid of Points.
  //!
  //! @param[in] a_out Output points
  //! @param[in] a_dimension number of points on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakePointGrid(vector<Real> & a_out, int a_dimension[2], Real const a_bounds[4 * VectorSize])
  {
    static_assert(VectorSize == 3 || VectorSize == 4, "Must have a vector size of 3 or 4");

    Real x_left = a_bounds[0] - a_bounds[VectorSize * 2];
    Real y_left = a_bounds[1] - a_bounds[VectorSize * 2 + 1];
    Real z_left = a_bounds[2] - a_bounds[VectorSize * 2 + 2];

    Real x_right = a_bounds[VectorSize] - a_bounds[VectorSize * 3];
    Real y_right = a_bounds[VectorSize + 1] - a_bounds[VectorSize * 3 + 1];
    Real z_right = a_bounds[VectorSize + 2] - a_bounds[VectorSize * 3 + 2];

    a_out.clear();
    a_out.resize((a_dimension[0] * a_dimension[1]) * VectorSize);
    for (int i = 0; i < a_dimension[1]; ++i)
    {
      Real frac_h = static_cast<Real>(i) / static_cast<Real>(a_dimension[1] - 1);

      Real xs = a_bounds[0] - frac_h * x_left;
      Real ys = a_bounds[1] - frac_h * y_left;
      Real zs = a_bounds[2] - frac_h * z_left;

      Real xe = a_bounds[VectorSize + 0] - frac_h * x_right;
      Real ye = a_bounds[VectorSize + 1] - frac_h * y_right;
      Real ze = a_bounds[VectorSize + 2] - frac_h * z_right;

      Real dx = xe - xs;
      Real dy = ye - ys;
      Real dz = ze - zs;

      for (int j = 0; j < a_dimension[0]; ++j)
      {
        Real frac_w = static_cast<Real>(j) / static_cast<Real>(a_dimension[0] - 1);
        a_out.push_back(xs + frac_w * dx);
        a_out.push_back(ys + frac_w * dy);
        a_out.push_back(zs + frac_w * dz);
        if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));
      }
    }
  }

  //! @ingroup DgUtility_functions
  //!
  //! Creates a grid of lines. Useful for rendering planes.
  //!
  //! @param[in] a_out Output points
  //! @param[in] a_dimension dimension on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakeLineGrid(vector<Real> & a_out, int a_dimension, Real const a_bounds[4 * VectorSize])
  {
    int dim[2] = { a_dimension, a_dimension };
    MakeLineGrid<Real, VectorSize>(a_out, dim, a_bounds);
  }

  //! @ingroup DgUtility_functions
  //!
  //! Creates a grid of lines. Useful for rendering planes.
  //! @param[in] a_out Output
  //! @param[in] a_dimension dimension on each side of the grid [w, h]
  //! @param[in] a_bounds [tl, tr, bl, br]
  template<typename Real, int VectorSize>
  void MakeLineGrid(vector<Real> & a_out, int a_dimension[2], Real const a_bounds[4 * VectorSize])
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

    a_out.clear();
    a_out.resize((a_dimension[0] + a_dimension[1] + 2) * VectorSize * 2);
    for (int i = 0; i < nDrawsH; ++i)
    {
      Real frac = static_cast<Real>(i) / static_cast<Real>(a_dimension[0]);

      a_out.push_back(a_bounds[0] - frac * x_left);
      a_out.push_back(a_bounds[1] - frac * y_left);
      a_out.push_back(a_bounds[2] - frac * z_left);
      if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));

      a_out.push_back(a_bounds[VectorSize + 0] - frac * x_right);
      a_out.push_back(a_bounds[VectorSize + 1] - frac * y_right);
      a_out.push_back(a_bounds[VectorSize + 2] - frac * z_right);
      if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));
    }

    for (int i = 0; i < nDrawsV; ++i)
    {
      Real frac = static_cast<Real>(i) / static_cast<Real>(a_dimension[1]);

      a_out.push_back(a_bounds[0] + frac * x_top);
      a_out.push_back(a_bounds[1] + frac * y_top);
      a_out.push_back(a_bounds[2] + frac * z_top);
      if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));

      a_out.push_back(a_bounds[VectorSize * 2 + 0] + frac * x_bottom);
      a_out.push_back(a_bounds[VectorSize * 2 + 1] + frac * y_bottom);
      a_out.push_back(a_bounds[VectorSize * 2 + 2] + frac * z_bottom);
      if (VectorSize == 4) a_out.push_back(static_cast<Real>(1.0));
    }
  }
}

#endif