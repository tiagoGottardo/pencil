#include "Tester.h"
#include "../classes/Factory.h"

void model() {
  Tester suite = Tester("Model Suite");

  suite.add([]() -> bool { 
    Model model = Factory::import("./tests/assets/object.obj");

    vector<Point> correctPoints = {
      Point(2374, 2374, 2374),
      Point(-2374, 2374, 2374),
      Point(2374, -2374, 2374),
      Point(-2374, -2374, 2374),
      Point(2374, 2374, -2374),
      Point(-2374, 2374, -2374),
      Point(2374, -2374, -2374),
      Point(-2374, -2374, -2374),
      Point(2374, 2374, 2374),
      Point(-2374, 2374, 2374),
      Point(2374, 2374, -2374),
      Point(-2374, 2374, -2374)
    };

    vector<Point> points; 
    for(Polygon polygon : model.getPolygons()) {
       vector<Point> newPoints = polygon.getPoints();
       points.insert(points.end(), newPoints.begin(), newPoints.end());
    } 

    for(size_t i = 0; i < points.size(); i++) 
      if(points[i] != correctPoints[i]) return false;

    if(model.getName() != "Object") return false;

    return true;
  }(), "it tests parser");

  suite.run();
}
