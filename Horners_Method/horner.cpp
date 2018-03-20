#include <iostream>

template<typename iter_Container>
 double Horner( iter_Container begin_Coefficient /* a_0 (the address of first coefficient) */,
                iter_Container end_Coefficient   /* a_n (the address of the container) */,
                double t                         /* independent variable */)
{
  double result_Polynomial = 0;
  while (end_Coefficient != begin_Coefficient) {
    result_Polynomial = result_Polynomial * t + *(--end_Coefficient);
  }
  return result_Polynomial;
}

int main()
{
  int degree_Polynomial = 0;
  std::cout << "Set the degree of polynomial(e.g. n): ";
  std::cin >>  degree_Polynomial;

  float* pCoefficients = new float[degree_Polynomial];
  std::cout << "Set coefficients in order of increasing degree(e.g. a_0, a_1, ... , a_n)\n: ";
  for(auto ii = 0; ii < degree_Polynomial; ii++) {
    std::cin >> pCoefficients[ii];
  }
  std::cout << "Your coefficients: ";
  for(auto ii = 0; ii < degree_Polynomial; ii++) {
    std::cout << pCoefficients[ii] << ", ";
  }
  std::cout << "\n";

  float variable = 0;
  std::cout << "Set variable(e.g. t): ";
  std::cin >> variable;
  std::cout << "The result of Horner's Method is "
            << Horner(pCoefficients, pCoefficients + n, variable)
            << std::endl;
  delete[] pCoefficients;
}
