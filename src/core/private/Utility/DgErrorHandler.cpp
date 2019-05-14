
#include <fstream>
#include <Windows.h>

#include "DgErrorHandler.h"

namespace Dg
{
  namespace impl
  {
    void __DG_ASSERT(bool a_val
      , char const * a_message
      , int a_lineNumber
      , char const * a_fileName
      , char const * a_functionName)
    {
      if (!a_val)
      {
        const char fileName[] = "Dg-crash-report.txt";
        std::ofstream fs;
        fs.open(fileName);
        if (fs.good())
        {
          fs << "Assert failed!\n" << a_message
            << "\nFile: " << a_fileName
            << "\nLine: " << a_lineNumber
            << "\nFunction: " << a_functionName;
          fs.close();
        }

        DebugBreak();
      }
    }
  }
}