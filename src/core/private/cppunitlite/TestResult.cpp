
#include "TestResult.h"
#include "Failure.h"

#include <stdio.h>
#include <iostream>


TestResult::TestResult ()
	: failureCount (0)
{
}


void TestResult::testsStarted () 
{
}


void TestResult::addFailure (const Failure& failure) 
{
  std::clog << "Failure: \""
    << failure.message.asCharString()
    << "\" line "
    << failure.lineNumber
    << " in "
    << failure.fileName.asCharString()
    << std::endl;

	failureCount++;
}


void TestResult::testsEnded () 
{
  if (failureCount > 0)
    std::clog << "There were " << failureCount << " failures" << std::endl;
  else
    std::clog << "There were no test failures" << std::endl;
}
