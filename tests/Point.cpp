#include "Tester.h"
#include "../classes/Point.h"

void point() {
  Tester suite = Tester("Point Suite");

  suite.add([]() -> bool { 
    Point p = Point(0, 0, 0);

    p.applyMatrix(Matrix::TranslationMatrix(20, 10, 237));

    return p == Point(20, 10, 237);
  }(), "it tests translation multiplication on point");

  suite.run();
}
