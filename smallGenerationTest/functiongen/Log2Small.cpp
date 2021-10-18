#include "PolyGenSampling/PolyGenFromFile.h"

// Accepts 3~4 command line arguments. Then generates a polynomial based on the
// arguments.
// Command line arguments:
// (1) intervals file: An input file containing a list of "reduced inputs" and "reduced
// intervals."
// (2) log file: An ouput file that will write various logging information while
// generating the polynomial
// (3) header file: An output file where the generated piecewise polynomials are
// stored. They will be stored as a 2-D array. Outer array contains each polynomial.
// The inner array contains the coefficients for each polynomial.
// (4) # of polynomials: A "secret" command line argument. If you know the size
// of the piecewise polynomial you want to generate, then pass it through this
// argument. If you pass "N" for this argument, then 2^N polynomials will be
// generated
int main(int argc, char** argv) {
  if (argc != 4 && argc != 5) {
      printf("Usage : %s <interval file> <log file> <header file>\n", argv[0]);
      printf("Input : <interval file> - file containing reduced constrains\n");
      printf("Output: <log file> - where we print some logging information\n");
      printf("Output: <header file> - the file containing coefficients\n");
  }
  
  FILE* f = fopen(argv[1], "r");
  FILE* log = fopen(argv[2], "w");
  FILE* header = fopen(argv[3], "w");
  
  int minN = -1;
  if (argc == 5) minN = atoi(argv[4]);
  
  // TODO : The power vector lets you customize the polynomial. It specifies the
  // terms of the polynomial. For example, the power vector for log will create a
  // polynomial of : c_1 x + c_2 x^2 + c_3 x^3 + ... + c_7 x^7.
  // You may need to set the power vector, especially to generate good polynomial.
  // Using the patterns in the taylor polynomial is a good start.
  // e.g. sin(x) -> {1, 3, 5, 7, 9}
  // e.g. cos(x) -> {0, 2, 4, 6, 8}
  std::vector<int> power({1, 2, 3, 4, 5, 6, 7, 8});
  
  // TODO : Pass the name of the arrays that store the polynomial coefficients
  // as the last two arguments. If there are both positive and negative reduced
  // inputs, we generate two polynomials. The first one corresponds to the
  // polynomial for positive reduced inputs. The second one corresponds to the
  // polynomial for the negative reduced inputs.
  GeneratePiecewiseFunction(f, log, header, power, minN, "__log2CoeffsSmall", "");
  
  fclose(f);
  fclose(log);
  fclose(header);
  
  return 0;
}
