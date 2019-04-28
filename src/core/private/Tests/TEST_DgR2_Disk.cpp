#include "TestHarness.h"
#include "DgR2Disk.h"
#include "DgR2Line.h"
#include "DgR2Vector.h"
#include "query/DgR2QueryDiskLine.h"
#include "query/DgR2QueryPointDisk.h"
#include "query/DgR2QueryDiskDisk.h"

typedef Dg::R2::Vector<float>      vec;
typedef Dg::R2::Line<float>        line;
typedef Dg::R2::Disk<float>        disk;

TEST(Stack_DgDisk0, DgDisk0)
{

  disk d0(vec(10.0f, 0.0f, 1.0f), 1.0f);
  disk d1(vec(-10.0f, 0.0f, 1.0f), 1.0f);

  vec v0(-1.0f, 0.0f, 0.0f);
  vec v1(1.0f, 0.0f, 0.0f);

  Dg::R2::FPCDiskDisk<float> fpcQuery;
  Dg::R2::FPCDiskDisk<float>::Result result = fpcQuery(d0, v0, d1, v1);

  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.t = 9.0f);
}

TEST(Stack_DgDisk2, DgDisk2)
{

  disk d0(vec(10.0f, 0.0f, 1.0f), 1.0f);
  vec p(-10.0f, 0.0f, 1.0f);
  vec v0(-1.0f, 1.0f, 0.0f);
  vec v1(1.0f, 1.0f, 0.0f);

  Dg::R2::FPCPointDisk<float> fpcQuery;
  Dg::R2::FPCPointDisk<float>::Result result = fpcQuery(d0, v0, p, v1);

  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.t = 9.5f);
}

TEST(Stack_DgDisk3, DgDisk3)
{

  disk d0(vec(10.0f, 0.0f, 1.0f), 1.0f);
  line l(vec(0.0f, 0.0f, 1.0f), vec(0.0f, 1.0f, 0.0f));
  vec v0(-1.0f, 1.0f, 0.0f);
  vec vl(0.0f, 0.0f, 0.0f);

  Dg::R2::FPCDiskLine<float> fpcQuery;
  Dg::R2::FPCDiskLine<float>::Result result = fpcQuery(d0, v0, l, vl);

  CHECK(result.code == Dg::QC_Intersecting);
  CHECK(result.t = 9.0f);
}