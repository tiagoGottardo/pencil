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

  vector<Point> getTransformedPoints(Matrix transformationMatrix) const {  
    vector<Point> result;

    for(Point point : points) 
      result.push_back(point.applyMatrix(transformationMatrix));

    return result;
  }

public:
  string getName() const override { return name; }

  Matrix getMatrix() const override {
    return transformationMatrix();
  }

  vector<Point> getPoints() const override { 
    vector<Point> copyPoints = points;
    return copyPoints; 
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
