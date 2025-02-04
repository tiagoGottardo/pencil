#pragma once

#include <vector>
#include <cmath>

#include "Matrix.h"
#include "Point.h"

class Transformable {
protected:
  Point ref;
  Vector rotation;
  double scaleFactor;

  void triggerRotate() { rotate(1); }

  Matrix transformationMatrix() const {
    return  
      Matrix::TranslationMatrix(ref.x, ref.y, ref.z) *
      Matrix::ScaleMatrix(scaleFactor, scaleFactor, scaleFactor) *
      Matrix::ZRotationMatrix(rotation.x) *
      Matrix::YRotationMatrix(rotation.y) *
      Matrix::XRotationMatrix(rotation.z);
  }

public:
  bool fixedXRotation;
  bool fixedYRotation;
  bool fixedZRotation;

  void rotate(double x, double y, double z) { rotation.x += x; rotation.y += y; rotation.z += z; }
  void rotate(double theta) { rotate(theta, theta, theta); }
  void rotateX(double theta) { rotation.x += theta; }
  void rotateY(double theta) { rotation.y += theta; }
  void rotateZ(double theta) { rotation.z += theta; }
  void scale(double factor) { if(scaleFactor + factor > .001) scaleFactor += factor; }
  void move(Point to) { ref += to; }

  Transformable() : 
    ref(Point("Ref")), 
    rotation(Vector("Rotation")), 
    scaleFactor(1.), 
    fixedXRotation(false),
    fixedYRotation(false),
    fixedZRotation(false) {}
};
