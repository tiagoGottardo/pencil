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

    for(Point point : points) {
      point.applyMatrix(transformationMatrix);
      result.push_back(point);
    }

    return result;
  }

  void applyMatrix(Matrix matrix) { for(Point& point : points) point.applyMatrix(matrix); }

public:
  string getName() const override { return name; }

  vector<Line> getLines() const override { return getLines(transformationMatrix()); }

  vector<Line> getLines(Matrix transformationMatrix) const { 
    vector<Line> lines;

    vector<Point> transformedPoints = getTransformedPoints(transformationMatrix);

    for(size_t i = 0; i < transformedPoints.size(); i++)
      lines.push_back(Line(transformedPoints[i], transformedPoints[(i + 1 < transformedPoints.size()) ? i + 1 : 0]));

    return lines; 
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
