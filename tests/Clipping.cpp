#include "Tester.h"
#include "../classes/Clipping.h"

void clipping() {
  Tester suite = Tester("Clipping Suite");

  suite.add([]() -> bool { 
    std::vector<Line> lines = std::vector<Line>();

    lines.push_back(Line(Point(0, 0), Point(0, 20)));
    lines.push_back(Line(Point(0, 0), Point(200, 0)));
    lines.push_back(Line(Point(200, 200), Point(400, 400)));

    Clipping clipping({100, 100});

    clipping.execute(&lines);

    return lines.size() == 2 && lines[1].b == Point(50, 0);
  }(), "it tests clip");

  suite.add([]() -> bool { 
    std::vector<Line> lines = std::vector<Line>();

    lines.push_back(Line(Point(0, 0), Point(0, 20)));
    lines.push_back(Line(Point(0, 0), Point(200, 0)));
    lines.push_back(Line(Point(200, 200), Point(400, 400)));

    Clipping clipping({100, 100});

    clipping.executeParallel(&lines, 4);

    return lines.size() == 2 && lines[1].b == Point(50, 0);
  }(), "it tests clip parallel execution");

  suite.run();
}
