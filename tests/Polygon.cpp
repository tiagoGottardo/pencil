#include "Tester.h"
#include "../classes/Factory.h"
#include "../classes/Point.h"

class PolygonFriend {
public:
  static Point getRef(Polygon polygon) {
    return polygon.ref;
  }

  static vector<Point> getPoints(Polygon polygon) {
    return polygon.points;
  }

  static vector<Point> getTransformedPoints(Polygon polygon) {
    return polygon.getTransformedPoints(polygon.transformationMatrix());
  }
};

void polygon() {
  Tester suite = Tester("Polygon Suite");

  suite.add([]() -> bool { 
    Polygon p = Factory::createRegularPolygon(100, 3, Point(50, 50));

    Point correctPoints[] = { 
      Point(0, -50),
      Point(43, 24),
      Point(-43, 25)
    };

    if(PolygonFriend::getRef(p) != Point(50, 50, 0)) return false;

    vector<Point> polygonPoints = PolygonFriend::getPoints(p);

    if(polygonPoints.size() != 3) return false;

    for(size_t i = 0; i < polygonPoints.size(); i++) 
      if(polygonPoints[i] != correctPoints[i]) return false;

    return true;
  }(), "it tests create regular polygon method");

  suite.add([]() -> bool { 
    Polygon p = Factory::createRegularPolygon(200, 4);

    vector<Point> transformedPoints = PolygonFriend::getTransformedPoints(p);

    Point correctPoints[] = { 
      Point(70, -70),
      Point(70, 70),
      Point(-70, 70),
      Point(-70, -70)
    };

    if(transformedPoints.size() != 4);

    for(size_t i = 0; i < transformedPoints.size(); i++) 
      if(correctPoints[i] != transformedPoints[i]) return false;

    return true;
  }(), "it tests polygon points transformation");

  suite.add([]() -> bool { 
    Polygon p = Factory::createRegularPolygon(200, 4);
    p.rotate(20);
    p.scale(1);

    vector<Point> points = p.getPoints();

    Point correctPoints[] = { 
      Point(70, -70),
      Point(70, 70),
      Point(-70, 70),
      Point(-70, -70)
    };

    if(points.size() != 4);

    for(size_t i = 0; i < points.size(); i++) 
      if(correctPoints[i] != points[i]) return false;

    return true;
  }(), "it tests polygon getPoints method");

  suite.run();
}
