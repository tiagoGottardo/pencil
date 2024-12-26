#include "Tester.h"
#include "../classes/Clipping.h"

void clipping() {
  Tester suite = Tester("Clipping Suite");

  suite.add([]() -> bool { 
    std::vector<Line> lines = std::vector<Line>();

    lines.push_back(Line(Point(0, 0), Point(0, 0.2)));
    lines.push_back(Line(Point(0, 0), Point(2, 0)));
    lines.push_back(Line(Point(2, 2), Point(4, 4)));

    Clipping::execute(&lines);

    return lines.size() == 2 && lines[1].b == Point(1, 0);
  }(), "it tests clip");

  suite.add([]() -> bool { 
    std::vector<Line> lines = std::vector<Line>();

    lines.push_back(Line(Point(0, 0), Point(0, 0.2)));
    lines.push_back(Line(Point(0, 0), Point(2, 0)));
    lines.push_back(Line(Point(2, 2), Point(4, 4)));

    Clipping::executeParallel(&lines);

    return lines.size() == 2 && lines[1].b == Point(1, 0);
  }(), "it tests clip parallel execution");

  suite.run();
}
