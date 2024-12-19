#pragma once

#include <vector>
#include <cmath>

#include "Drawable.h"
#include "Polygon.h"

class ModelFriend;

class Model : public Drawable {
private:
  friend class ModelFriend;

  string name;
  Point ref;
  vector<Polygon> polygons;

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

public:
  bool fixedXRotation;
  bool fixedYRotation;
  bool fixedZRotation;

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

  vector<Line> getLines() const override { 
    vector<Line> lines;

    for(Polygon polygon : polygons) {
      vector<Line> polygonLines = polygon.getLines(transformationMatrix());

      for(Line line : polygonLines) 
        lines.push_back(line);
    }

    return lines; 
  }

  static Model createDonut(int sides) {
    std::vector<Polygon> polygons;

    int R = 5000; 
    int smallR = 2500; 

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

  Model(vector<Polygon> polygons, Point ref = Point(), const string& name = "Model") : 
    name(name), 
    ref(Point(ref.x, ref.y, ref.z, "Ref")), 
    polygons(polygons),
    xRotation(.0),  
    yRotation(.0),  
    zRotation(.0),
    scaleFactor(1.),
    fixedXRotation(false),
    fixedYRotation(false),
    fixedZRotation(false) {}
  
  void checkItself() const override {
    printf("Model %s\n", name.c_str());
    ref.checkItself(); 
    printf("  X rotation: %.2f\n", xRotation);
    printf("  Y rotation: %.2f\n", yRotation);
    printf("  Z rotation: %.2f\n\n", zRotation);
    for(Polygon polygon : polygons) polygon.checkItself();
    printf("}\n");
  }
};
