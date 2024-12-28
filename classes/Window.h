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
  FRONT,
  BACK,
};

class Window {
private:
  uint width, height, far;
  DisplayFile *displayFile;
  double xRotation;
  double yRotation;
  double zRotation;
  Point centroid;
  friend class WindowFriend;

  double climp(double value) {
    if(value > 360.) return climp(value - 360.);
    if(value < 0.) return climp(value + 360.);
    return value;
  }

  Matrix windowMatrix() {
    return Matrix::ScaleMatrix(1./width, 1./height, 1./far) *
    Matrix::ZRotationMatrix(zRotation) *
    Matrix::XRotationMatrix(xRotation) *
    Matrix::YRotationMatrix(yRotation) *
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

public:
  Window(uint width, uint height, uint far, DisplayFile* displayFile) : 
    width(width), 
    height(height), 
    far(far), 
    displayFile(displayFile), 
    xRotation(0), 
    yRotation(0), 
    zRotation(0), 
    centroid(Point(0, 0)) {}

  void setSize() { width = (width == 500) ? 100 : 500; height = (height == 250) ? 100 : 250; }

  void rotateX(double theta) { xRotation = climp(xRotation + theta); }
  void rotateY(double theta) { yRotation = climp(yRotation + theta); }
  void rotateZ(double theta) { zRotation = climp(zRotation + theta); }

  Point forwardPoint() {
    Point forward(0, 0, -1);
    Matrix rotationMatrix = Matrix::XRotationMatrix(xRotation) * Matrix::YRotationMatrix(yRotation);
    forward.applyMatrix(rotationMatrix);
    return Point(-forward.x, -forward.y, forward.z);
  }

  Point rightPoint() {
    Point forward = forwardPoint();
    Point result = Point(-forward.z, 0, forward.x);
    
    return result.normalize(); 
  }

  string interface() {
    ostringstream stream;
    stream << "Window | Position: (" << std::fixed << std::setprecision(2) << centroid.x << "; " << centroid.y << "; " << centroid.z <<
    ") | Width: " << width << " | Height: " << height << " | Far: " << far << " | Angle: (" << xRotation << "; " << yRotation << "; " << zRotation << ")";
    return stream.str();    
  }

  void move(Direction direction, double speed = 5.) { 
    Point move;
    switch(direction) {
      case FRONT: move = (forwardPoint() * speed); break;
      case RIGHT: move = (rightPoint() * speed); break;
      case BACK: move = (-forwardPoint() * speed); break;
      case LEFT: move = (-rightPoint() * speed); break;
    }

    centroid += move;
  } 

  vector<Line> transformViewport(RectangleSize frameSize, Point viewportCenter) {
    vector<Line> lines = normalizeDisplayFile(frameSize);

    Clipping::executeParallel(&lines);

    Matrix viewportMatrix = this->viewportMatrix(frameSize, viewportCenter);
    for(Line& line : lines) line.applyMatrix(viewportMatrix);
    
    return lines;
  }
};
