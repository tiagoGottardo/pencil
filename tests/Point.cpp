#include "Tester.h"
#include "../classes/Point.h"

void point() {
  Tester suite = Tester("Point Suite");

  // suite.add([]() -> bool { 
  //   return true;
  // }(), "");

  // suite.add([]() -> bool {
  //   return false;
  // }(), "");

  suite.run();
}
