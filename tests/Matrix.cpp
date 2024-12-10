#include "Tester.h"
#include "../classes/Matrix.h"

void matrix() {
  Tester suite = Tester("Matrix Suite");

  suite.add([]() -> bool { 
    Matrix identity4 = Matrix::IdentityMatrix(4);

    for(int i = 0; i < 4; i++)
      if(identity4[i][i] != 1) return false;

    return true;
  }(), "it tests identity matrix constructor");

  // suite.add([]() -> bool {
  //   Matrix m({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   for(int i = 0; i < m.getWidth(); i++)
  //     for(int j = 0; j < m.getHeight(); j++)
  //       if(m[j][i] != i + j) return false;

  //   if(m.getWidth() != 3) return false;
  //   if(m.getHeight() != 2) return false;

  //   return true;
  // }(), "it tests declarative matrix constructor");

  // suite.add([]() -> bool {
  //   Matrix m1({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   Matrix m2({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   Matrix result({
  //     {0, 2, 4},
  //     {2, 4, 6},
  //   });

  //   if(result == m1 + m2) return true;

  //   return true;
  // }(), "it tests plus matrix operation");

  // suite.add([]() -> bool {
  //   Matrix m1({
  //     {0, 1, 2},
  //   });

  //   Matrix m2({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   try {
  //     Matrix m3 = m1 + m2;
  //   } catch(const std::exception&) {
  //     return true;
  //   }

  //   return false;
  // }(), "it tests plus matrix operation gets error on invalid input");

  // suite.add([]() -> bool {
  //   Matrix m1({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   Matrix m2({
  //     {0, -1, -2},
  //     {-1, -2, -3}
  //   });

  //   return m1 == -m2;
  // }(), "it tests invert signal matrix operation");

  // suite.add([]() -> bool {
  //   Matrix m1({
  //     {0, 1, 2},
  //     {1, 2, 3}
  //   });

  //   Matrix m2({
  //     {3, 5, 10},
  //     {0, 2, 7},
  //     {1, 2, 3}
  //   });

  //   Matrix result({
  //     {2, 6, 13},
  //     {6, 15, 33},
  //   });

  //   return result == m1 * m2;
  // }(), "it tests multiplication matrix operation");

  suite.run();
}
