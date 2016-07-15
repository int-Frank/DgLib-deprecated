#include "TestHarness.h"
#include "DgIDManager.h"

TEST(Stack_DgIDManager, creation_DgIDManager)
{
  int lower = 1;
  int upper = 11;
  Dg::IDManager<int> idm(1, 11);
  for (int i = lower; i <= upper + 1; ++i)
  {
    idm.GetID();
  }

  idm.ReturnID(4);
  idm.ReturnID(2);
  idm.ReturnID(1);
  idm.ReturnID(3);
  idm.ReturnID(8);
  idm.ReturnID(6);
  idm.ReturnID(7);
  idm.ReturnID(5);
  idm.ReturnID(9);
  idm.ReturnID(11);
  idm.ReturnID(10);

  CHECK(!idm.IsUsed(3));
  CHECK(!idm.IsUsed(0));
  CHECK(!idm.IsUsed(40));

  CHECK(idm.MarkAsUsed(4));
  CHECK(idm.MarkAsUsed(2));
  CHECK(idm.MarkAsUsed(1));
  CHECK(idm.MarkAsUsed(3));
  CHECK(idm.MarkAsUsed(8));
  CHECK(idm.MarkAsUsed(6));
  CHECK(!idm.MarkAsUsed(2));
  CHECK(idm.MarkAsUsed(7));
  CHECK(idm.MarkAsUsed(5));
  CHECK(idm.MarkAsUsed(9));
  CHECK(!idm.MarkAsUsed(9));
  CHECK(idm.MarkAsUsed(11));
  CHECK(idm.MarkAsUsed(10));

  for (int i = lower; i <= upper; ++i)
  {
    CHECK(idm.IsUsed(i));
  }
}