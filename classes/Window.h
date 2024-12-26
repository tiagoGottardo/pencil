#pragma once

#include <chrono>
#include <iostream>

#include "./Point.h"
#include "./Polygon.h"
#include "./Clipping.h"

class Window {
private:
  uint width, height, far;
  DisplayFile *displayFile;
  double rotation;
  Point centroid;

  friend class WindowFriend;

  Matrix windowMatrix() {
    return Matrix::ScaleMatrix(1./width, 1./height, 1./far) *
      Matrix::ZRotationMatrix(rotation) * 
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
  Window(uint width, uint height, uint far, DisplayFile* displayFile) : width(width), height(height), far(far), displayFile(displayFile), rotation(0), centroid(Point(0, 0)) { }

  void setSize() { width = (width == 500) ? 100 : 500; height = (height == 250) ? 100 : 250; }

  void rotate(double diffRotation) { rotation += diffRotation; }

  void move(Point to) { centroid += Point(to.x, -to.y, to.z); }

  vector<Line> transformViewport(RectangleSize frameSize, Point viewportCenter) {
    vector<Line> lines = normalizeDisplayFile(frameSize);

    Clipping::executeParallel(&lines);

    Matrix viewportMatrix = this->viewportMatrix(frameSize, viewportCenter);
    for(Line& line : lines) line.applyMatrix(viewportMatrix);
    
    return lines;
  }
};
