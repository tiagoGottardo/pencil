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

  static vector<Line> createFrame(uint width, uint height, Point centroid, string name = "Rectangle") {
    vector<Point> points;

    points.push_back(Point());
    points.push_back(Point(0, height));
    points.push_back(Point(width, height));
    points.push_back(Point(width));

    for(Point& point : points) point -= Point(width / 2, height / 2);

    Polygon frame = Polygon(points, centroid, name);

    points = frame.getPoints();

    for(Point& point : points) point.applyMatrix(frame.getMatrix());

    vector<Line> lines;

    for(size_t i = 0; i < points.size(); i++) lines.push_back(Line(points[i], points[(i + 1 != points.size()) ? i + 1 : 0]));
    
    return lines;
  }

  static Model createDonut(int sides, int size = 500) {
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

  static Model import(const std::string& filename) {
    vector<Point> points;
    vector<Polygon> polygons;
    points.emplace_back(Point());

    ifstream file(filename, std::ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    vector<char> buffer(size);
    file.read(buffer.data(), size);
    istringstream fileContent(buffer.data());

    string line;
    while (std::getline(fileContent, line)) {
      istringstream lineStream(line);
      string prefix;
      lineStream >> prefix;

      if (prefix == "v") {
        double x, y, z;
        lineStream >> x >> y >> z;
        points.emplace_back(x, y, z);
      } else if (prefix == "f") {
        vector<Point> polygonPoints;
        string slice;
        while (lineStream >> slice) {
          size_t slashPos = slice.find('/');
          int index = std::stoi(slice.substr(0, slashPos));
          polygonPoints.push_back(points[index]);
        }
        polygons.emplace_back(std::move(polygonPoints));
      }
    }

    string_view nameView(filename);
    size_t lastSlashPos = nameView.find_last_of('/');
    if (lastSlashPos != std::string_view::npos) nameView = nameView.substr(lastSlashPos + 1);

    size_t dotPos = nameView.find_last_of('.');
    if (dotPos != std::string_view::npos) nameView = nameView.substr(0, dotPos);

    std::string name(nameView);
    if (!name.empty()) name[0] = toupper(name[0]);

    return Model(std::move(polygons), Point(), name);
  }
};
