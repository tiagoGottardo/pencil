#include <chrono>
#include <iostream>

#include "../classes/Factory.h"

extern void matrix();
extern void polygon();
extern void point();
extern void window();
extern void clipping();
extern void model();

void performance() {
  Model model = Factory::import("./assets/psyduckGigante-.obj");
  // Model model = Factory::import("./assets/m4-.obj");
  // Model model = Factory::import("./assets/charizard.obj");

  auto start = std::chrono::high_resolution_clock::now();

  model.getLines();

  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Function executed in " << duration.count() << " milliseconds." << std::endl;
}

int main() {
  matrix();
  point();
  polygon();
  window();
  clipping();
  model();
  performance();
  return 0;
}
