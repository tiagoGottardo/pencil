#include "Tester.h"
#include "../classes/Point.h"

void point() {
  Tester suite = Tester("Point Suite");

  suite.add([]() -> bool { 
    Point p = Point(0, 0);

    Matrix translationMatrix({
      {1, 0, 0, 20},
      {0, 1, 0, 10},
      {0, 0, 1, 0},
      {0, 0, 1, 1}
    });

    p = translationMatrix * p.toMatrix();

    return p == Point(20, 10);
  }(), "it tests translation multiplication on point");

  suite.run();
}
