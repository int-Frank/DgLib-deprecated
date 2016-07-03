//! @file DgStringFunctions.cpp
//!
//! @author Frank Hart
//! @date 3/07/2016

#include "DgStringFunctions.h"

namespace Dg
{
  std::string Trim(std::string const & a_str
                 , std::string const & a_chars)
  {
    size_t strBegin = a_str.find_first_not_of(a_chars);
    if (strBegin == std::string::npos)
    {
      return ""; // no content
    }

    size_t strEnd = a_str.find_last_not_of(a_chars);
    size_t strRange = strEnd - strBegin + 1;

    return a_str.substr(strBegin, strRange);
  }	//End: Trim()
}