#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "./Point.h"
#include "./Polygon.h"
#include "./Model.h"

class Parser {
public:

  static Model parse(string filename) {
    vector<Point> points = vector<Point>();
    vector<Polygon> polygons = vector<Polygon>();
    points.push_back(Point());
    ifstream file(filename);

    string line;

    int i = 0;

    while (getline (file, line) && i < 30) {
      istringstream lineStream(line);
      string prefix;
      lineStream >> prefix;

      if (prefix == "v") {
        double x, y, z;
        lineStream >> x >> y >> z;
        points.push_back(Point(round(x * 1000), round(y * 1000), round(z * 1000)));
      } 

      if (prefix == "f") {
        vector<Point> polygonPoints = vector<Point>();
        string slice;
        while(lineStream >> slice) {
          istringstream sliceStream(slice);
          string sliceIndex;
          if(std::getline(sliceStream, sliceIndex, '/')) 
            polygonPoints.push_back(points[stoi(sliceIndex)]);
        }
        polygons.push_back(Polygon(polygonPoints));
      } 
    }

    size_t lastSlashPos = filename.find_last_of('/');
    string name = filename.substr(lastSlashPos + 1);
    size_t dotPos = name.find_last_of('.');
    if (dotPos != string::npos) name = name.substr(0, dotPos); 
    if (!name.empty()) name[0] = toupper(name[0]); 

    return Model(polygons, Point(), name);
  }
};
