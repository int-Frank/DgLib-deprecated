#include "TestHarness.h"
#include "DgParser_INI.h"

TEST(Stack_DgParser_INI, creation_DgParser_INI)
{
  Dg::Parser_INI parser;
  parser.Parse("testini.ini");
  CHECK(parser.GetItems().size() == 2);

  std::map<std::string, std::string>::const_iterator cit = parser.GetItems().find("n0");
  CHECK(cit != parser.GetItems().end()
    && cit->second == "v0");

  cit = parser.GetItems().find("n1");
  CHECK(cit != parser.GetItems().end()
    && cit->second == "v1");
}