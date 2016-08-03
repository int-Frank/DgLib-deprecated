#include "TestHarness.h"
#include "DgMakeGrid.h"

TEST(Stack_DgMakeGrid_INI, creation_DgMakeGrid_INI)
{
  Dg::vector_pod<float> result;
  int dim2[2] = { 4, 4 };
  int dim1 = 8;
  float const bounds3[12] =
  {
     0.0f,   3.0f,   0.0f,
     3.0f,   3.0f,   0.0f,
     0.0f,   0.0f,   0.0f,
     3.0f,   0.0f,   0.0f 
  }; 
  
  /*float const bounds3[12] =
  {
    -0.5f,   0.5f,   0.0f,
    0.5f,   0.5f,   0.0f,
    -0.5f,  -0.5f,   0.0f,
    0.5f,  -0.5f,   0.0f
  };*/

  float const bounds4[16] =
  {
    -0.5f,   0.5f,   0.0f, 1.0f,
     0.5f,   0.5f,   0.0f, 1.0f,
    -0.5f,  -0.5f,   0.0f, 1.0f,
     0.5f,  -0.5f,   0.0f, 1.0f
  }; 

  //Line grids
  Dg::MakeLineGrid<float, 3>(result, dim2, bounds3);
  CHECK(result.size() == (dim2[0] + dim2[1] + 2) * 3 * 2);

  Dg::MakeLineGrid<float, 4>(result, dim2, bounds4);
  CHECK(result.size() == (dim2[0] + dim2[1] + 2) * 4 * 2);

  Dg::MakeLineGrid<float, 3>(result, dim1, bounds3);
  CHECK(result.size() == (dim1 * 2 + 2) * 3 * 2);

  Dg::MakeLineGrid<float, 4>(result, dim1, bounds3);
  CHECK(result.size() == (dim1 * 2 + 2) * 4 * 2);

  //Point grids
  Dg::MakePointGrid<float, 3>(result, dim2, bounds3);
  CHECK(result.size() == ((dim2[0]) * (dim2[1]) * 3));

  Dg::MakePointGrid<float, 4>(result, dim2, bounds4);
  CHECK(result.size() == ((dim2[0]) * (dim2[1]) * 4));

  Dg::MakePointGrid<float, 3>(result, dim1, bounds3);
  CHECK(result.size() == ((dim1 * dim1) * 3));

  Dg::MakePointGrid<float, 4>(result, dim1, bounds4);
  CHECK(result.size() == ((dim1 * dim1) * 4));

  //Mesh indices
  Dg::vector_pod<int> ind;
  Dg::MakeGridInd<int>(ind, dim2);
  CHECK(ind.size() == ((dim2[0] - 1) * (dim2[1] - 1)) * 2 * 3);

  Dg::MakeGridInd<int, false>(ind, dim2);
  CHECK(ind.size() == ((dim2[0] - 1) * (dim2[1] - 1)) * 2 * 3);

  //Point grids with unique vertices for each grid element
  int dimU[2] = { dim2[0] - 1, dim2[1] - 1 };
  Dg::MakePointGridUniqueElem<float, 3>(result, dimU, bounds3);
  CHECK(result.size() == (dimU[0] * dimU[1] * 4 * 3));

  Dg::MakeGridIndUniqueElem<int>(ind, dim2);
  CHECK(ind.size() == ((dim2[0] - 1) * (dim2[1] - 1)) * 2 * 3);

  Dg::MakeGridIndUniqueElem<int, false>(ind, dim2);
  CHECK(ind.size() == ((dim2[0] - 1) * (dim2[1] - 1)) * 2 * 3);

}