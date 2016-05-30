#include "TestHarness.h"

#include <iostream>
#include <string>
#include <fstream>

#define RESULTS_FILE "results.txt"

int main()
{
  //Redirect std::cerr to file
  std::filebuf CLOG_NEW_BUF;
  CLOG_NEW_BUF.open(RESULTS_FILE, std::ios::out);
  std::streambuf* CLOG_OLD_BUF = std::clog.rdbuf(&CLOG_NEW_BUF);

  TestResult tr;
  TestRegistry::runAllTests(tr);
  
  char * buf = 0;
  size_t sze = 0;
  _dupenv_s(&buf, &sze, "SUPPRESS_OUTPUT");
  if (sze == 0)
  {
    std::ifstream fs(RESULTS_FILE);
    std::string file((std::istreambuf_iterator<char>(fs)),
      std::istreambuf_iterator<char>());
    std::cout << file;

    char brk;
    std::cin >> brk;
  }

  std::clog.rdbuf(CLOG_OLD_BUF);
  return tr.FailureCount();
}