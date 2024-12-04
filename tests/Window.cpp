#include "Tester.h"
#include "../classes/Window.h"

class WindowFriend {
public:
  static std::vector<Line> testNormalizeDisplayFile(Window window) {
    return window.normalizeDisplayFile();
  }

  static Matrix testCalculateTransformationMatrix(Window window, RectangleSize viewportSize) {
    return window.calculateTransformationMatrix(viewportSize);
  }
};

void window() {
  Tester suite = Tester("Window Suite");

  suite.add([]() -> bool { 
    std::vector<Drawable*>* displayFile = new std::vector<Drawable*>();
    displayFile->push_back(Polygon::createRegularPolygon(100, 4, Point(10, 10)));

    Window window = Window(100, 100, displayFile);
    window.move(Point(40, 40));

    std::vector<Line> lines = WindowFriend::testNormalizeDisplayFile(window);

    Line correctLines[] = {
      Line(new Point(5, -65), new Point(5, 5)),
      Line(new Point(5, 5), new Point(-65, 5)),
      Line(new Point(-65, 5), new Point(-65, -65)),
      Line(new Point(-65, -65), new Point(5, -65))
    };

    for(int i = 0; i < (int) lines.size(); i++)
      if(*lines[i].a != *correctLines[i].a || *lines[i].b != *correctLines[i].b) return false;

    return true;
  }(), "it tests window normalization on displayFile items");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());

    RectangleSize viewportSize = {500, 250};
    Matrix matrix = WindowFriend::testCalculateTransformationMatrix(window, viewportSize);

    Matrix correctMatrix = Matrix({
      {5.0, 0, 0, 250.0},
      {0, 2.5, 0, 125.0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    });

    return matrix == correctMatrix;
  }(), "it tests matrix transformation without rotation");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    window.rotate(10);

    RectangleSize viewportSize = {500, 250};
    Matrix matrix = WindowFriend::testCalculateTransformationMatrix(window, viewportSize);

    Matrix correctMatrix = Matrix({
      {4.92, 0.86, 0, 250},
      {-0.43, 2.46, 0, 125},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    });

    return matrix == correctMatrix;
  }(), "it tests matrix transformation with rotation");

  suite.run();
}
