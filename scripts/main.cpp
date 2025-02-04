#include <fstream> 
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  if(argc != 2) return 1;

  string arg = argv[1];

  string filepath = "./assets/" + arg + ".obj";

  ifstream file(filepath);

  if(!file) {
    printf("%s not found.\n", filepath.c_str());
    return 1;
  }

  string line;

  int v = 0;
  int f = 0;

  while (getline(file, line)) {
    istringstream lineStream(line);
    string prefix;
    lineStream >> prefix;

    if (prefix == "v") v++;
    if (prefix == "f") f++;
  }

  printf("Amount of vertices: %d\n", v);
  printf("Amount of faces: %d\n", f);

  return 0;
}
