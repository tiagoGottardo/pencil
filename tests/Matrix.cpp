#include "Tester.h"
#include "../classes/Matrix.h"

#define MATRIX_SIZE 4

void matrix() {
  Tester suite = Tester("Matrix Suite");

  suite.add([]() -> bool {
    Matrix m({
      {0, 1, 2, 3},
      {1, 2, 3, 4},
      {2, 3, 4, 5},
      {3, 4, 5, 6}
    });

    for(int i = 0; i < MATRIX_SIZE; i++)
      for(int j = 0; j < MATRIX_SIZE; j++)
        if(m[j][i] != i + j) return false;

    return true;
  }(), "it tests declarative matrix constructor");

  suite.add([]() -> bool {
    Matrix m1({
      {12, 123, 43, 5},
      {4, 23, 52, 4},
      {4, 4, 52, 4},
      {56, 6, 7, 3},
    });

    Matrix m2({
      {3, 1, 23, 0},
      {23, 2, 0, 0},
      {23, 6, 34, 9},
      {54, 45, 8, 34},
    });

    Matrix result({
      {4124,	741,	1778,	557},
      {1953,	542,	1892,	604},
      {1516,	504,	1892,	604},
      {629,	245,	1550,	165}
    });

    if(result == m1 * m2) return true;

    return true;
  }(), "it tests multiplication matrix operation");

  suite.add([]() -> bool {
    Matrix m1({
      {12, 123, 43, 5},
      {4, 23, 52, 4},
      {4, 4, 52, 4},
      {56, 6, 7, 3},
    });

    Matrix m2({
      {3, 1, 23, 0},
      {23, 2, 0, 0},
      {23, 6, 34, 9},
      {54, 45, 8, 34},
    });

    Matrix result({
      {15,	124,	66,	5},
      {27,	25,	52,	4},
      {27,	10,	86,	13},
      {110,	51,	15,	37}
    });

    return result == m1 + m2;
  }(), "it tests plus matrix operation");

  suite.add([]() -> bool {
    Matrix m;

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        if(m[i][j] != 0) return false;

    return true;
  }(), "it tests matrix contructor");

  suite.run();
}
