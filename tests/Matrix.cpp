#include "Tester.h"

int main() {
  Tester suite = Tester("Matrix Suite");

  suite.add(1 + 1 == 2, "First test");
  suite.add(1 + 1 == 3, "Second test");

  suite.run();

  return 0;
}
