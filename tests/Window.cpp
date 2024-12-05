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

  static char testCalculateRC(Window window, Point point) {
    return window.calculateRC(point);
  }

  // static std::optional<Point> testCalculateIntersectionPoint(Window window, Line line, double clipBoard, bool isVertical) {
  //   return window.calculateIntersectionPoint(line, clipBoard, isVertical);
  // }

  static LineStatus testResolveIntersection(Window window, Line line) {
    return window.resolveIntersection(line);
  }

  static LineStatus testCalculateRCStatus(Window window, Line line) {
    return window.calculateRCStatus(line);
  }

  static void testClip(Window window, std::vector<Line>* lines) {
    return window.clip(lines);
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
  }(), "it tests matrix transformation");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());

    return WindowFriend::testCalculateRC(window, Point(10, 10)) == 0;
  }(), "it tests calculateRC method case 1");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());

    return WindowFriend::testCalculateRC(window, Point(-60, 10)) == 1; 
  }(), "it tests calculateRC method case 2");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());

    return WindowFriend::testCalculateRC(window, Point(200, 200)) == 10;
  }(), "it tests calculateRC method case 3");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());

    return WindowFriend::testResolveIntersection(window, Line(new Point(400, 400), new Point(500, 500))) == LineStatus::COMPLETELY_OUTSIDE;
  }(), "it tests intersection solving");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    Line line = Line(new Point(0, 0), new Point(100, 0));

    return WindowFriend::testResolveIntersection(window, line) == LineStatus::HAS_INTERSECTION &&
    *line.a == Point(0, 0) && *line.b == Point(50, 0);
  }(), "it tests intersection solving");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    Line line = Line(new Point(0, 0), new Point(30, 0));

    return WindowFriend::testCalculateRCStatus(window, line) == LineStatus::COMPLETELY_INSIDE;
  }(), "it tests RC status of line case 1");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    Line line = Line(new Point(0, 0), new Point(200, 0));

    return WindowFriend::testCalculateRCStatus(window, line) == LineStatus::HAS_INTERSECTION;
  }(), "it tests RC status of line case 2");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    Line line = Line(new Point(200, 0), new Point(0, 200));

    return WindowFriend::testCalculateRCStatus(window, line) == LineStatus::INTERSECTION_CHECK_NEEDED;
  }(), "it tests RC status of line case 3");

  suite.add([]() -> bool { 
    Window window = Window(100, 100, new std::vector<Drawable*>());
    Line line = Line(new Point(200, 200), new Point(200, 200));

    return WindowFriend::testCalculateRCStatus(window, line) == LineStatus::COMPLETELY_OUTSIDE;
  }(), "it tests RC status of line case 4");

  suite.add([]() -> bool { 
    std::vector<Line> lines = std::vector<Line>();

    lines.push_back(Line(new Point(0, 0), new Point(0, 20)));
    lines.push_back(Line(new Point(0, 0), new Point(200, 0)));
    lines.push_back(Line(new Point(200, 200), new Point(400, 400)));

    Window window = Window(100, 100, new std::vector<Drawable*>());

    WindowFriend::testClip(window, &lines);
    return lines.size() == 2 && *lines[1].b == Point(50, 0);
  }(), "it tests clip");

  suite.run();
}
