#pragma once

#include <vector>
#include <cmath>

#include "Drawable.h"
#include "Point.h"
#include "Line.h"

class Polygon : public Drawable {
private:
  friend class PolygonFriend;

  string name;
  Point ref;
  vector<Point> points;

  double xRotation;
  double yRotation;
  double zRotation;
  double scaleFactor;

  void triggerRotate() { rotate(1); }

  Matrix transformationMatrix() const {
    return  
      Matrix::TranslationMatrix(ref.x, ref.y, ref.z) *
      Matrix::ScaleMatrix(scaleFactor, scaleFactor, scaleFactor) *
      Matrix::XRotationMatrix(xRotation) *
      Matrix::YRotationMatrix(yRotation) *
      Matrix::ZRotationMatrix(zRotation);
  }

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
  void rotate(double x, double y, double z) { 
    xRotation += x; 
    yRotation += y; 
    zRotation += z; 
  }
  void rotate(double theta) { rotate(theta, theta, theta); }
  void rotateX(double theta) { xRotation += theta; }
  void rotateY(double theta) { yRotation += theta; }
  void rotateZ(double theta) { zRotation += theta; }
  void scale(double factor) { if(scaleFactor + factor != .0) scaleFactor += factor; }
  void move(Point to) { ref = ref + to; }

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
    name(name), 
    ref(Point(ref.x, ref.y, ref.z, "Ref")), 
    points(points),
    xRotation(.0), 
    yRotation(.0), 
    zRotation(.0), 
    scaleFactor(1.) {}
  
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
