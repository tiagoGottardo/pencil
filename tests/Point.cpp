#include "Tester.h"
#include "../classes/Point.h"

void point() {
  Tester suite = Tester("Point Suite");

  suite.add([]() -> bool { 
    Point p = Point(0, 0, 0);

    p.applyMatrix(Matrix::TranslationMatrix(20, 10, 237));

    return p == Point(20, 10, 237);
  }(), "it tests translation multiplication on point");

  suite.add([]() -> bool { 
    Matrix m({ {3.43, 0}, {2.9, 0}, {0.1, 0} });
    Point p = Point();

    p = m;

    return p == Point(3, 3, 0);
  }(), "it tests matrix to point conversion");

  suite.add([]() -> bool { 
    Matrix m = Matrix({ {3.43, 0}, {2.9, 0} });
    Point p = Point();

    try {
      p = m;
    } catch(std::exception e) {
      return true;
    }

    return false;
  }(), "it tests invalid matrix to point conversion");

  suite.run();
}
