//! @file DgParser_INI.h
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class declaration: Parser_INI

#ifndef DGPARSER_INI_H
#define DGPARSER_INI_H

#include <string>
#include <fstream>

#include "Dg_map.h"
#include "DgTypes.h"

//TODO Documentation
namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class Parser_INI
  //!
  //! @brief Parser_INI
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  class Parser_INI
  {
  public:

    //! Get pointer to the static instance of the object
    ErrorCode Parse(std::string const & file);

    Dg::map<std::string, std::string> const & GetItems() const { return m_items; }

  private:

    Dg::map<std::string, std::string> m_items;

  private:

    static char const s_delim = '=';

  private:

    //! Prevent construction
    bool IsComment(std::string const & a_str);

    //! Prevent construction by copying
    bool GetNameValue(std::string const & a_str
                    , std::string & a_name
                    , std::string & a_value);

  };


}

#endif