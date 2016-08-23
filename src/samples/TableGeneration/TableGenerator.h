#pragma once

#include <ios>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <string>

template<typename T>
struct Suffix
{
  static char const * value;
};

template<typename T>
char const * Suffix<T>::value = "";

template<> struct Suffix<float> { static char const * value; };
char const * Suffix<float>::value = "f";

template<> struct Suffix<long double> { static char const * value; };
char const * Suffix<long double>::value = "L";

template<> struct Suffix<uint64_t> { static char const * value; };
char const * Suffix<uint64_t>::value = "ull";

template<> struct Suffix<int64_t> { static char const * value; };
char const * Suffix<int64_t>::value = "ll";


std::string Prefix(std::ios_base& (*a_base)(std::ios_base&))
{
  if (a_base == std::hex)
  {
    return "0x";
  }
  return "";
}

template<typename T>
class TableGenerator
{
public:

  virtual void Generate(unsigned nValues) = 0;

  virtual void Output(std::ios_base& (*base)(std::ios_base&)
                    , int resultsPerLine
                   ,  int precision
                   ,  char const * fileName = "table.txt")
  {
    //Open file
    std::ofstream os(fileName);
    if (!os.good())
    {
      return;
    }

    os << std::setprecision(precision);

    for (size_t i = 0; i < m_values.size(); ++i)
    {
      os << base << Prefix(base) << m_values[i] << Suffix<T>::value;

      if (i != (m_values.size() - 1))
      {
        if ((i + 1) % (resultsPerLine) == 0)
        {
          os << ",\n";
        }
        else
        {
          os << ", ";
        }
      }
    }
  }

protected:

  std::vector<T> m_values;
};