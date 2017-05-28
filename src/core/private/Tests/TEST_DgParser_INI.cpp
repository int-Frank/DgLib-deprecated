#include "TestHarness.h"
#include "DgParser_INI.h"

TEST(Stack_DgParser_INI, creation_DgParser_INI)
{
  Dg::Parser_INI parser;
  parser.Parse("testini.ini");

  std::map<std::string, std::string> items = parser.GetItems();

  CHECK(items.size() == 2);
  CHECK(items.at("n0") == "v0");
  CHECK(items.at("n1") == "v1");
}