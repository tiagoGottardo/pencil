#include <chrono>
#include <iostream>

#include "../classes/Factory.h"

extern void matrix();
extern void polygon();
extern void point();
extern void window();
extern void clipping();
extern void model();

int main() {
  matrix();
  point();
  polygon();
  window();
  clipping();
  model();

  return 0;
}
