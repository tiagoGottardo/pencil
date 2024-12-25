#pragma once

#include <chrono>
#include <iostream>

#include "./Point.h"
#include "./Polygon.h"
#include "./Clipping.h"

#define NEAR 0.1 
#define FAR 1300 

class Window {
private:
  uint width, height;
  DisplayFile *displayFile;
  double rotation;
  Point centroid;

  friend class WindowFriend;

  Matrix normalizationMatrix() {
    return Matrix::ScaleMatrix(1./width, 1./height, 1./1300.) *
    Matrix::ZRotationMatrix(rotation) * 
    Matrix::TranslationMatrix(-centroid.x, -centroid.y, -centroid.z);
  }

  void normalizeDrawable(unique_ptr<Drawable>& drawable, vector<Line>* result) {
    Matrix transformationMatrix = normalizationMatrix() * drawable->getMatrix();

    for(Polygon polygon : drawable->getPolygons()) {
      vector<Point> points = polygon.applyTransformations(transformationMatrix);

      for(size_t i = 0; i < points.size(); i++) 
        result->push_back(Line(points[i], points[(i + 1 < points.size()) ? i + 1 : 0]));
    }
  }

  vector<Line> normalizeDisplayFile() {
    vector<Line> result = vector<Line>();

    for(unique_ptr<Drawable>& drawable : *displayFile)
      normalizeDrawable(drawable, &result);

    return result;
  }

  Matrix transformationMatrix(RectangleSize frameSize, Point viewportCenter) {
    return Matrix::TranslationMatrix(viewportCenter.x, viewportCenter.y) * 
    Matrix::ScaleMatrix(frameSize.width / 2, frameSize.height / 2);
  }

public:
  Window(uint width, uint height, DisplayFile* displayFile) : width(width), height(height), displayFile(displayFile), rotation(0), centroid(Point(0, 0)) { }

  void setSize() { width = (width == 500) ? 100 : 500; height = (height == 250) ? 100 : 250; }

  void rotate(double diffRotation) { rotation += diffRotation; }

  void move(Point to) { centroid += Point(to.x, -to.y, to.z); }

  vector<Line> transformViewport(RectangleSize frameSize, Point viewportCenter) {
    vector<Line> lines = normalizeDisplayFile();

    Clipping::executeParallel(&lines);

    Matrix transformationMatrix = this->transformationMatrix(frameSize, viewportCenter);
    for(Line& line : lines) line.applyMatrix(transformationMatrix);
    
    return lines;
  }
};
