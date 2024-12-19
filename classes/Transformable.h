#pragma once

#include <vector>
#include <cmath>

#include "Matrix.h"

class Transformable {
protected:
  Point ref;

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

  Transformable(Point ref) : 
    ref(Point(ref.x, ref.y, ref.z, "Ref")), 
    xRotation(.0), 
    yRotation(.0), 
    zRotation(.0), 
    scaleFactor(1.), 
    fixedXRotation(false),
    fixedYRotation(false),
    fixedZRotation(false) {}
};
