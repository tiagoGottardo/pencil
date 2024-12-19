#include "Tester.h"
#include "../classes/Window.h"
#include "../classes/Factory.h"

class WindowFriend {
public:
  static std::vector<Line> testNormalizeDisplayFile(Window window) {
    return window.normalizeDisplayFile();
  }

  static Matrix testCalculateTransformationMatrix(Window window, RectangleSize viewportSize, Point frameCenter) {
    return window.transformationMatrix(viewportSize, frameCenter);
  }
};

void window() {
  Tester suite = Tester("Window Suite");

  suite.add([]() -> bool { 
    DisplayFile* displayFile = new DisplayFile();
    displayFile->push_back(make_unique<Polygon>(Factory::createRegularPolygon(100, 4, Point(10, 10))));

    Window window = Window(100, 100, displayFile);
    window.move(Point(40, 40));

    std::vector<Line> lines = WindowFriend::testNormalizeDisplayFile(window);

    Line correctLines[] = {
      Line(Point(5, 15), Point(5, 85)),
      Line(Point(5, 85), Point(-65, 85)),
      Line(Point(-65, 85), Point(-65, 15)),
      Line(Point(-65, 15), Point(5, 15))
    };

    for(int i = 0; i < (int) lines.size(); i++)  
      if(lines[i] != correctLines[i]) return false;

    return true;
  }(), "it tests window normalization on displayFile items");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new DisplayFile());

    RectangleSize viewportSize = {500, 250};
    Matrix matrix = WindowFriend::testCalculateTransformationMatrix(window, viewportSize, Point(viewportSize.width / 2, viewportSize.height / 2));

    Matrix correctMatrix = Matrix({
      {5.0, 0, 0, 250.0},
      {0, 2.5, 0, 125.0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    });

    return matrix == correctMatrix;
  }(), "it tests matrix transformation");

  suite.run();
}
