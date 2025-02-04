#pragma once

#include <vector>
#include <cmath>
#include <sstream>

#include "Drawable.h"
#include "Transformable.h"
#include "Point.h"
#include "Line.h"

class Polygon : public Drawable, public Transformable {
private:
  string name;
  vector<Point> points;

  friend class PolygonFriend;

public:
  string getName() const override { return name; }

  Matrix getMatrix() const override { return transformationMatrix(); }

  vector<Polygon> getPolygons() const override { return vector<Polygon>{*this}; }

  vector<Point> getPoints() const { return points; }

  vector<Line> normalize(Matrix matrix) { 
    vector<Line> result;

    for(Point& point : points) point.applyMatrix(matrix, true); 
    for(size_t i = 0; i < points.size(); i++) result.push_back(Line(points[i], points[(i + 1 < points.size()) ? i + 1 : 0]));

    return result;
  }

  Polygon(vector<Point> points, const string& name = "Polygon") : 
    Transformable(),
    name(name), 
    points(points) {}
  
  void checkItself() const override {
    printf("%s: {\n", name.c_str());
    ref.checkItself(); 
    rotation.checkItself();
    for(Point point : points) point.checkItself();
    printf("}\n");
  }

  string interface() const override {
    return (ostringstream()
      << " " << name 
      << " | Position: " << ref.interface()  
      << " | Angle: " << rotation.interface()).str();
  }
};
