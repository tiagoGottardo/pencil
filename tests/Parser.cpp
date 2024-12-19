#include "Tester.h"
#include "../classes/Parser.h"

class ModelFriend {
public:
  static vector<Polygon> getPolygons(Model model) {
    return model.polygons;
  }
};

void parser() {
  Tester suite = Tester("Parser Suite");

  suite.add([]() -> bool { 
    Model model = Parser::parse("./assets/object.obj");

    for(Polygon polygon : ModelFriend::getPolygons(model)) polygon.checkItself();

    return true;
  }(), "it tests parser");

  suite.run();
}
