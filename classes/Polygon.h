#pragma once

#include <vector>
#include <cmath>

#include "Drawable.h"
#include "Transformable.h"
#include "Point.h"
#include "Line.h"

class Polygon : public Drawable, public Transformable {
private:
  friend class PolygonFriend;

  string name;
  vector<Point> points;

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

  Polygon(vector<Point> points, Point ref = Point(), const string& name = "Polygon") : 
    Transformable(ref),
    name(name), 
    points(points) {}
  
  void checkItself() const override {
    printf("%s: {\n", name.c_str());
    ref.checkItself(); 
    printf("  X rotation: %.2f\n", xRotation);
    printf("  Y rotation: %.2f\n", yRotation);
    printf("  Z rotation: %.2f\n\n", zRotation);
    for(Point point : points) point.checkItself();
    printf("}\n");
  }
};
