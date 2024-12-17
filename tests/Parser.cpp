#include "Tester.h"
#include "../classes/Parser.h"

void parser() {
  Tester suite = Tester("Parser Suite");

  suite.add([]() -> bool { 
    vector<Polygon> polygons = Parser::parse("./assets/object.obj");

    for(Polygon polygon : polygons) polygon.checkItself();

    return true;
  }(), "it tests parser");

  suite.run();
}
