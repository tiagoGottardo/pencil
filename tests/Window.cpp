#include "Tester.h"
#include "../classes/Window.h"
#include "../classes/Factory.h"

class WindowFriend {
public:
  static std::vector<Line> testNormalizeDisplayFile(RectangleSize frameSize, Window window) { return window.normalizeDisplayFile(frameSize); }
  static Matrix testCalculateViewportMatrix(Window window, RectangleSize frameSize, Point viewportCenter) { return window.viewportMatrix(frameSize, viewportCenter); }
  static Point getCentroid(Window window) { return window.centroid; }
};

void window() {
  Tester suite = Tester("Window Suite");

  suite.add([]() -> bool { 
    DisplayFile* displayFile = new DisplayFile();
    Polygon p = Factory::createRegularPolygon(100, 4);

    p.move(Point(10, 10, -50));
    displayFile->push_back(make_unique<Polygon>(p));


    Window window = Window(100, 100, 1300, displayFile);
    window.move(RIGHT, 40);
    window.move(UP, 40);

    vector<Line> lines = WindowFriend::testNormalizeDisplayFile({ 1311, 761 }, window);

    Line correctLines[] = {
      Line(Point(3.36, -41.02, -4.20), Point(3.36, 3.36, -4.20)),
      Line(Point(3.36, 3.36, -4.20), Point(-41.02, 3.36, -4.20)),
      Line(Point(-41.02, 3.36, -4.20), Point(-41.02, -41.02, -4.20)),
      Line(Point(-41.02, -41.02, -4.20), Point(3.36, -41.02, -4.20)),
    };

    for(int i = 0; i < (int) lines.size(); i++)  
      if(lines[i] != correctLines[i]) return false;

    return true;
  }(), "it tests window normalization on displayFile items");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    RectangleSize viewportSize = {500, 250};
    Matrix matrix = WindowFriend::testCalculateViewportMatrix(window, viewportSize, Point(viewportSize.width / 2, viewportSize.height / 2));

    Matrix correctMatrix = Matrix({
      {250,   0, 0, 250},
      {  0, 125, 0, 125},
      {  0,   0, 1,   0},
      {  0,   0, 0,   1}
    });

    return matrix == correctMatrix;
  }(), "it tests matrix transformation");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    window.move(FORWARD, 50);

    return WindowFriend::getCentroid(window) == Point(0, 0, -50);
  }(), "it should move to front 50 units");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    window.move(FORWARD, 50);
    window.move(RIGHT, 50);


    return WindowFriend::getCentroid(window) == Point(50, 0, -50);
  }(), "it should move to front and right 50 units");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    window.move(BACK, 50);
    window.move(LEFT, 50);

    return WindowFriend::getCentroid(window) == Point(-50, 0, 50);
  }(), "it should move to back and left 50 units");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    window.rotateY(270);
    window.move(RIGHT, 50);

    return WindowFriend::getCentroid(window) == Point(0, 0, -50);
  }(), "it should turn to left and move to right 50 units");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, 1300, new DisplayFile());

    window.rotateY(90);
    window.move(RIGHT, 50);

    return WindowFriend::getCentroid(window) == Point(0, 0, 50);
  }(), "it should turn to right and move to right 50 units");

  suite.run();
}
