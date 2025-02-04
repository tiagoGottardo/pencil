#pragma once

#include <chrono>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "./Point.h"
#include "./Polygon.h"
#include "./Clipping.h"


enum Direction {
  RIGHT,
  LEFT,
  FORWARD,
  BACK,
  UP,
  DOWN
};

class Window {
private:
  uint width, height, far;
  DisplayFile *displayFile;
  Vector rotation;
  Point centroid;

  friend class WindowFriend;

  Matrix windowMatrix() {
    return Matrix::ScaleMatrix(1./width, 1./height, 1./far) *
    Matrix::XRotationMatrix(rotation.x) *
    Matrix::YRotationMatrix(rotation.y) *
    Matrix::ZRotationMatrix(rotation.z) *
    Matrix::TranslationMatrix(-centroid.x, -centroid.y, -centroid.z);
  }

  Matrix viewportMatrix(RectangleSize frameSize, Point viewportCenter) {
    return Matrix::TranslationMatrix(viewportCenter.x, viewportCenter.y) * 
    Matrix::ScaleMatrix(frameSize.width / 2, frameSize.height / 2);
  }

  vector<Line> normalizeDisplayFile(RectangleSize frameSize) {
    vector<Line> result = vector<Line>();

    for(unique_ptr<Drawable>& drawable : *displayFile) {
      Matrix normalizationMatrix = Matrix::PerspectiveMatrix(frameSize, far) * windowMatrix() * drawable->getMatrix();

      for(Polygon polygon : drawable->getPolygons()) 
        for(Line line : polygon.normalize(normalizationMatrix)) 
          result.push_back(line);
    }

    return result;
  }

  Vector forwardVector() {
    Vector result(0, 0, -1);
    Matrix rotationMatrix = Matrix::XRotationMatrix(rotation.x) * Matrix::YRotationMatrix(rotation.y);
    result.applyMatrix(rotationMatrix);

    result.x *= -1;
    result.y *= -1;

    return result;
  }

  Vector rightVector() { return (Point(-forwardVector().z, 0, forwardVector().x)).normalize(); }

  Vector getMoveVector(Direction direction) {
    switch(direction) {
      case FORWARD: return forwardVector(); 
      case RIGHT: return rightVector();
      case BACK: return -forwardVector();
      case LEFT: return -rightVector();
      case UP: return Point(0, 1, 0);
      default: return Point(0, -1, 0);
    }
  }

public:
  Window(uint width, uint height, uint far, DisplayFile* displayFile) : 
    width(width), 
    height(height), 
    far(far), 
    displayFile(displayFile), 
    rotation(Vector("Rotation")), 
    centroid(Point("Ref")) {}

  void rotateX(double theta) { rotation.x += theta; }
  void rotateY(double theta) { rotation.y += theta; }
  void rotateZ(double theta) { rotation.z += theta; }

  string interface() {
    return (ostringstream()
      << " Window" 
      << " | Position: " << centroid.interface()  
      << " | Width: " << width  
      << " | Height: " << height  
      << " | Far: " << far  
      << " | Angle: " << rotation.interface()).str();
  }

  void move(Direction direction, double speed = 5) { centroid += getMoveVector(direction) * speed; } 

  vector<Line> transformViewport(RectangleSize frameSize, Point viewportCenter) {
    vector<Line> lines = normalizeDisplayFile(frameSize);

    Clipping::executeParallel(&lines);

    Matrix viewportMatrix = this->viewportMatrix(frameSize, viewportCenter);
    for(Line& line : lines) line.applyMatrix(viewportMatrix);
    
    return lines;
  }
};
