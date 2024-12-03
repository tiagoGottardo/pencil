#include "Tester.h"
#include "../classes/Polygon.h"
#include "../classes/Point.h"

int main() {
  Tester suite = Tester("Polygon Suite");

  suite.add([]() -> bool { 
    Polygon* triangle = Polygon::createRegularPolygon(100, 3, Point(100, 100));

    Point centroid = triangle->calculateCentroid();
    
    return centroid == Point(0, 0);
  }(), "it tests if centroid starts on origin");

  suite.add([]() -> bool {
    Polygon* triangle = Polygon::createRegularPolygon(10, 3, Point(100, 100));

    Point beforePoint = *(triangle->getPoints()[0]);

    triangle->rotate(180);
    triangle->rotate(180);

    Point afterPoint = *(triangle->getPoints()[0]);

    return beforePoint == afterPoint;
  }(), "it tests rotation precision on polygon");

  suite.add([]() -> bool {
    Polygon* triangle = Polygon::createRegularPolygon(10, 4, Point(100, 100));
    std::vector<Point*> points = triangle->getPoints();

    for(int i = 0; i < points.size(); i++) 
      *points[i] += Point(10, 10);

    triangle->setRef();

    return 
      triangle->ref == Point(110, 110) &&
      triangle->calculateCentroid() == Point(0, 0);
  }(), "it tests setRef method");

  suite.run();

  return 0;
}
