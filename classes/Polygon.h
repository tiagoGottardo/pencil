#pragma once

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include "Drawable.h"
#include "Point.h"
#include "Line.h"

#include <vector>
#include <cmath>

class Polygon : public Drawable {
private:
  string name;
  Point ref;
  vector<Point> points;

public:

  string getName() const override { return name; }

  vector<Line> getLines() const override { 
    vector<Line> lines;

    for(size_t i = 0; i < points.size(); i++)
      lines.push_back(Line(points[i] + ref, points[(i + 1 < points.size()) ? i + 1 : 0] + ref));

    return lines; 
  }

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

  Polygon(vector<Point> points, Point ref = Point(), const string& name = "Polygon") : name(name), ref(Point(ref.x, ref.y, ref.z, "Ref")), points(points) {}
  
  void checkItself() const override {
    printf("%s: {\n", name.c_str());
    ref.checkItself(); 
    calculateCentroid().checkItself();
    for(Point point : points) point.checkItself();
    printf("}\n");
  }

  Point calculateCentroid() const {
    int size = points.size();
    if (size == 0) throw invalid_argument("There is no points in centroid calculation.");

    Point C = Point();
    for(Point point : points) C += point;

    return C / size;
  }

  void move(Point to) { ref = ref + to; }
  void rotate(double theta_degree) { this->applyMatrix(Matrix::ZRotationMatrix(theta_degree)); }
  void scale(double x, double y, double z) { this->applyMatrix(Matrix::ScaleMatrix(x, y, z)); }
  void scale(double factor) { this->scale(factor, factor, factor); }
  void applyMatrix(Matrix matrix) { for(Point& point : points) point.applyMatrix(matrix); }
};
