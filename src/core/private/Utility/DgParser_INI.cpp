
#include "DgParser_INI.h"
#include "DgStringFunctions.h"

namespace Dg
{
  bool Parser_INI::IsComment(std::string const & a_str)
  {
    std::string str(Trim(a_str));
    std::string CommentDelim = "#;";
    size_t strBegin = str.find_first_of(CommentDelim);
    return strBegin == 0;
  }

  bool Parser_INI::GetNameValue(std::string const & a_str
                              , std::string & a_name
                              , std::string & a_value)
  {
    std::string str(Trim(a_str));
    const size_t delimPos = str.find_first_of(s_delim);
    if (delimPos == std::string::npos)
    {
      a_name = str;
      a_value = "";
      return false;
    }

    a_name = str.substr(0, delimPos);
    a_value = str.substr(delimPos + 1, str.size() - 1);
    return (a_name != "") && (a_value != "");
  }

  ErrorCode Parser_INI::Parse(std::string const & a_file)
  {
    m_items.clear();

    std::ifstream file(a_file);
    if (!file.good())
    {
      return ErrorCode::FailedToOpenFile;
    }

    std::string line;
    while (std::getline(file, line))
    {
      line = Trim(line);
      if (IsComment(line))
      {
        continue;
      }

      std::string name, value;
      if (GetNameValue(line, name, value))
      {
        m_items.insert(std::pair<std::string, std::string>(Trim(name), Trim(value)));
      }
    }
    return ErrorCode::None;
  }
}