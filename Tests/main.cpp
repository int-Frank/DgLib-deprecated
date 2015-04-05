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

  std::ifstream fs(RESULTS_FILE);
  std::string file((std::istreambuf_iterator<char>(fs)),
    std::istreambuf_iterator<char>());
  std::cout << file;

  char c;
  std::cin >> c;

  std::clog.rdbuf(CLOG_OLD_BUF);
  return 0;
}