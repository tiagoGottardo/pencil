// g++ -DTEST_MODE ./scripts/performance.cpp -o ./scripts/perf
#include <chrono>
#include <iostream>

#include "../classes/Factory.h"
#include "../classes/Window.h"

#define FRAME_WIDTH 1311
#define FRAME_HEIGHT 761 

using namespace std;

void perf(string filepath, int window_size, double scale = 0.) {
  Model model = Factory::import(filepath);
  model.scale(scale);

  DisplayFile* displayFile = new DisplayFile();
  displayFile->push_back(make_unique<Model>(model));

  Window window = Window(window_size, window_size, 1300, displayFile);

  auto start = std::chrono::high_resolution_clock::now();

  vector<Line> lines = window.transformViewport({ FRAME_WIDTH, FRAME_HEIGHT }, Point(FRAME_WIDTH / 2, FRAME_HEIGHT / 2));

  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  cout << "Draw: " << duration.count() << endl << endl;
}

int main() {
  perf("./assets/charizard.obj", 10000);         // Charizard              4
  perf("./assets/psyduck.obj", 10000);           // Psyduck               17
  perf("./assets/m4-.obj", 10000);               // M4                    58
  perf("./assets/m4-.obj", 10000, 1.5);          // M4 with intersection  45
  perf("./assets/psyduckGigante-.obj", 1000000); // Psyduck Gigante      656

  return 0;
}
