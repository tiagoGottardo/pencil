#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "./Point.h"
#include "./Polygon.h"
#include "./Model.h"

class Factory {
public:

  static Polygon createRegularPolygon(int size = 100, int sides = 3, Point centroid = Point(), string name = "Polygon") {
    if(sides < 3) throw invalid_argument("It must has at least 3 sides.");

    double R = size / 2;
    double initialAng = -M_PI/2;
    double ang;

    vector<Point> points;

    for(int i = 0; i < sides; i++) {
      if(sides % 2 == 0) initialAng = 0;
      if(sides % 4 == 0) initialAng = -M_PI/sides;
      
      ang = initialAng + (i * 2 * M_PI / sides);

      points.push_back(Point(R * cos(ang), R * sin(ang)));
    }

    return Polygon(points, centroid, name);
  }

  static Polygon createRectangle(uint width, uint height, Point centroid, string name = "Rectangle") {
    vector<Point> points;

    points.push_back(Point());
    points.push_back(Point(0, height));
    points.push_back(Point(width, height));
    points.push_back(Point(width));

    for(Point& point : points) point -= Point(width / 2, height / 2);

    return Polygon(points, centroid, name);
  }

  static Model createDonut(int sides, int size = 10000) {
    std::vector<Polygon> polygons;

    int R = size / 2; 
    int smallR = R / 2; 

    for (int i = 0; i < sides; i++) {
      double ang = i * 2 * M_PI / sides;
      double nextAng = (i + 1) * 2 * M_PI / sides;

      for (int j = 0; j < sides; j++) {
        double ang2 = j * 2 * M_PI / sides;
        double nextAng2 = (j + 1) * 2 * M_PI / sides;

        Point p1(
          (R + smallR * cos(ang2)) * cos(ang),
          (R + smallR * cos(ang2)) * sin(ang),
          smallR * sin(ang2)
        );
        Point p2(
          (R + smallR * cos(nextAng2)) * cos(ang),
          (R + smallR * cos(nextAng2)) * sin(ang),
          smallR * sin(nextAng2)
        );
        Point p3(
          (R + smallR * cos(nextAng2)) * cos(nextAng),
          (R + smallR * cos(nextAng2)) * sin(nextAng),
          smallR * sin(nextAng2)
        );
        Point p4(
          (R + smallR * cos(ang2)) * cos(nextAng),
          (R + smallR * cos(ang2)) * sin(nextAng),
          smallR * sin(ang2)
        );

        polygons.push_back(Polygon({p1, p2, p3, p4}));
      }
    }

    return Model(polygons, Point(), "Donut");
  };

  static Model import(string filename) {
    vector<Point> points = vector<Point>();
    vector<Polygon> polygons = vector<Polygon>();

    points.push_back(Point());

    ifstream file(filename);

    string line;

    while(getline (file, line)) {
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
