#include "Tester.h"
#include "../classes/Polygon.h"
#include "../classes/Point.h"

void polygon() {
  Tester suite = Tester("Polygon Suite");

  suite.add([]() -> bool { 
    Polygon triangle = Polygon::createRegularPolygon(100, 3, Point(100, 100));

    return triangle.calculateCentroid() == Point(0, 0, 0);
  }(), "it tests if centroid starts on origin");

  suite.run();
}
