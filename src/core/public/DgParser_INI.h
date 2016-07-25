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

namespace Dg
{
  //! @ingroup DgUtility
  //!
  //! @class Parser_INI
  //!
  //! Basic parser for .ini files. An ini file is in the format 
  //!
  //!   token=value
  //!
  //! Comments are preceeded by a either a '#' or ';'
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  class Parser_INI
  {
  public:

    //! Loads and parses an ini file.
    //!
    //! @return ErrorCode::None on success.
    ErrorCode Parse(std::string const & file);

    //! Get the token/value items successfully extracted from the ini file.
    Dg::map<std::string, std::string> const & GetItems() const { return m_items; }

  private:

    Dg::map<std::string, std::string> m_items;

  private:

    static char const s_delim = '=';
    static char const s_comment[];

  private:

    //! Is this line a comment?
    bool IsComment(std::string const & a_str);

    //! Break the current line in to a token and value.
    bool GetNameValue(std::string const & a_str
                    , std::string & a_name
                    , std::string & a_value);

  };


}

#endif