#include "TestHarness.h"
#include "Dg_shared_ptr.h"

TEST(Stack_DgSharedPtr, creation_DgSharedPtr)
{
  int a = 234;
  int * val = new int(a);
  Dg::shared_ptr<int> p0(val);
  Dg::shared_ptr<int> p1(p0);
  p0 = p1;

  int i = *p1;

  CHECK(i == a);
}