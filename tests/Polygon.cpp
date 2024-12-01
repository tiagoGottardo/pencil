#include "Tester.h"

int main() {
  Tester suite = Tester("Polygon Suite");

  suite.add(1 + 1 == 2, "First test");
  suite.add(1 + 1 == 3, "Second test");

  suite.add([]() -> bool { 

  }(), "Third test");

  suite.run();

  return 0;
}
