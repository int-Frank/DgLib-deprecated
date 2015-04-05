#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

// ------------------------------------------------------------------------ -
//	Generate an array of the denominators
//-------------------------------------------------------------------------
void GenerateERFDenominators(std::vector<double>& a_out, unsigned number)
{
  a_out.clear();

  double sign = 1.0;
  double val = 1.0;
  double factorial = 1.0;
  double twoPowN = 1.0;
  double invSqrt2Pi = 0.39894228040143267793994605993438;

  for (unsigned i = 0; i < number; ++i)
  {
    a_out.push_back((sign * val * factorial * twoPowN * invSqrt2Pi));

    //Prepare values for next iteration.
    sign *= -1.0;
    val += 2.0;
    factorial *= (static_cast<double>(i) + 1.0 );
    twoPowN *= 2;
  }
}

void OutputDenoms(unsigned a_number)
{
  const unsigned numbers_per_row = 4;
  const unsigned decimal_accuracy = 20;
  std::string file = "tnd_table.txt";

  std::vector<double> results;
  GenerateERFDenominators(results, a_number);

  //Open file
  std::ofstream os(file);
  os << std::setprecision(decimal_accuracy);

  os << "{";

  for (size_t i = 0; i < results.size(); ++i)
  {
    if (i % (numbers_per_row) == 0)
    {
      os << std::endl;
    }

    os << results[i];

    if (i != (results.size() - 1))
    {
      os << ", ";
    }
  }

  os << std::endl << "}";
}

int main()
{
  OutputDenoms(150);
}