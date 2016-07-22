//! @file DgStringFunctions.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGSTRINGFUNCTIONS_H
#define DGSTRINGFUNCTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

//TODO Documentation
namespace Dg
{
  //! @ingroup DgUtility_functions
  //!
  //! @brief Converts a string to a number.
  //!
  //! @param[out] t Output number.
  //! @param[in] s Input string.
  //! @param[in] f The base fo the number to read. (eg std::dec)
  //!
  //! @return True if a number was loaded into the output.
  template <typename Real>
  bool StringToNumber(Real& t,
                      const std::string& s,
                      std::ios_base& (*f)(std::ios_base&))
  {
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
  }	//End: StringToNumber()

  std::vector<std::string> & Split(std::string const & a_in
                                 , char a_delim
                                 , std::vector<std::string> & a_out);

  template<typename Real>
  std::vector<Real> & StringToNumberList(std::string const & a_in
                                       , char a_delim
                                       , std::ios_base& (*a_f)(std::ios_base&)
                                       , std::vector<Real> & a_out)
  {
    a_out.clear();
    std::stringstream ss(a_in);
    std::string item;
    while (getline(ss, item, a_delim)) 
    {
      Real val(static_cast<Real>(0));
      if (StringToNumber(val, item, a_f))
      {
        a_out.push_back(val);
      }
    }
    return a_out;
  }

  //! @ingroup DgUtility_functions
  //!
  //! @brief Trims leading and trailing characters
  //!
  //! @param[in] a_str Input string.
  //!
  //! @return Trimed string
  std::string Trim(std::string const & a_str
                 , std::string const & a_chars = " \t");
}

#endif