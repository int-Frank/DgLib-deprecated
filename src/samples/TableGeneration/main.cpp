#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

// ------------------------------------------------------------------------ -
//	Generate constants for the maclaculin expansion of inverf
//-------------------------------------------------------------------------
void Generate_erf(std::vector<double>& a_out, unsigned number)
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


// ------------------------------------------------------------------------ -
//	Generate constants for the maclaculin expansion of inverf
//-------------------------------------------------------------------------
void Generate_inverf(std::vector<double>& a_out, unsigned a_number)
{
  double * c = new double[a_number]();
  c[0] = 1.0;
  for (unsigned k = 1; k < a_number; ++k)
  {
    for (unsigned m = 0; m <= k - 1; m++)
    {
      c[k] += (c[m] * c[k - 1 - m]) / ((m + 1) * (2 * m + 1));
    }
  }

  double a = 0.88622692545275801364908374167057; //sqrt(pi) / 2
  double a_sq = a * a;
  double b = 1.0; //2k + 1

  for (unsigned k = 0; k < a_number; ++k)
  {
    a_out.push_back((c[k] * a / b));

    //Prepare values for next iteration
    a *= a_sq;
    b += 2.0;
  }

  delete[] c;
}

void OutputConstants(const char * a_fileName,
                     const char * a_header,
                     unsigned a_number, 
                     void (*a_funct)(std::vector<double>&, unsigned))
{
  const unsigned numbers_per_row = 4;
  const unsigned decimal_accuracy = 20;

  std::vector<double> results;
  a_funct(results, a_number);

  //Open file
  std::ofstream os(a_fileName);
  os << std::setprecision(decimal_accuracy);

  os << a_header << std::endl << std::endl;

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
  OutputConstants("tables.txt", "inverf", 512, Generate_inverf);
}