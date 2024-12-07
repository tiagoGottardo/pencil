#ifndef POLYGON_H
#define POLYGON_H

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

using sizet = std::size_t;

class Polygon : public Drawable {
private:
  std::string name;
  Point ref;
  std::vector<Point> points;

public:

  std::string getName() const override { return name; }

  std::vector<Line> getLines() const override { 
    std::vector<Line> lines;

    for(sizet i = 0; i < points.size(); i++)
      lines.push_back(Line(points[i] + ref, points[(i + 1 < points.size()) ? i + 1 : 0] + ref));

    return lines; 
  }

  static Polygon* createRegularPolygon(int size = 100, int sides = 3, Point centroid = Point(), std::string name = "Polygon") {
    if(sides < 3) throw std::invalid_argument("It must has at least 3 sides.");

    double R = size / 2;
    double initialAng = -M_PI/2;
    double ang;

    std::vector<Point> points;

    for(int i = 0; i < sides; i++) {
      if(sides % 2 == 0) initialAng = 0;
      if(sides % 4 == 0) initialAng = -M_PI/sides;
      
      ang = initialAng + (i * 2 * M_PI / sides);

      points.push_back(Point(R * std::cos(ang), R * std::sin(ang)));
    }

    return new Polygon(points, centroid, name);
  }

  Polygon(std::vector<Point> points, Point ref = Point(), const std::string& name = "Polygon") : name(name), ref(Point(ref.x, ref.y, ref.z, "Ref")), points(points) {}
  
  void checkItself() const override {
    printf("%s: {\n", name.c_str());
    ref.checkItself(); 
    calculateCentroid().checkItself();
    for(Point point : points) point.checkItself();
    printf("}\n");
  }

  Point calculateCentroid() const {
    int size = points.size();
    if (size == 0) throw std::invalid_argument("There is no points in centroid calculation.");

    Point C = Point();
    for(Point point : points) C += point;

    return C / size;
  }

  void draw(QPainter*) const override { }

  void move(Point to) { ref = ref + to; }
  void rotate(double theta_degree) { this->applyMatrix(Matrix::ZRotationMatrix(theta_degree)); }
  void scale(double x, double y, double z) { this->applyMatrix(Matrix::ScaleMatrix(x, y, z)); }
  void scale(double factor) { this->scale(factor, factor, factor); }
  void applyMatrix(Matrix matrix) { for(Point& point : points) point.applyMatrix(matrix); }
};

#endif
