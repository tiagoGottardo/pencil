#include "Tester.h"
#include "../classes/Factory.h"

void model() {
  Tester suite = Tester("Model Suite");

  suite.add([]() -> bool { 
    Model model = Factory::import("./tests/assets/object.obj");

    vector<Point> points = model.getPoints();

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

    for(size_t i = 0; i < correctPoints.size(); i++) 
      if(points[i] != correctPoints[i]) return false;

    if(model.getName() != "Object") return false;

    return true;
  }(), "it tests parser");

  suite.run();
}
