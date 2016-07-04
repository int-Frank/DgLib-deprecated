#include "TestHarness.h"
#include "DgParser_INI.h"

TEST(Stack_DgParser_INI, creation_DgParser_INI)
{
  Dg::Parser_INI parser;
  parser.Parse("testini.ini");
  CHECK(parser.GetItems().size() == 2);
  if (parser.GetItems().size() == 2)
  {
    CHECK(parser.GetItems().query_key(0) == "n0");
    CHECK(parser.GetItems()[0] == "v0");
    CHECK(parser.GetItems().query_key(1) == "n1");
    CHECK(parser.GetItems()[1] == "v1");
  }
}