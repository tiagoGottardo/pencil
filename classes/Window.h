#pragma once

#include "./Point.h"
#include "./Polygon.h"
#include "./Clipping.h"

class Window {
private:
  uint width, height;
  DisplayFile *displayFile;
  double rotation;
  Point centroid;

  friend class WindowFriend;

  Matrix normalizationMatrix() {
    return Matrix::ZRotationMatrix(rotation) * 
    Matrix::TranslationMatrix(-centroid.x, -centroid.y, -centroid.z);
  }

  vector<Line> normalizeDisplayFile() {
    vector<Line> result = vector<Line>();

    for(unique_ptr<Drawable>& drawable : *displayFile) {
      vector<Line> lines = drawable->getLines();

      for(Line& line : lines) 
        result.push_back(line.applyMatrix(normalizationMatrix()));
    }

    return result;
  }

  Matrix transformationMatrix(RectangleSize frameSize, Point viewportCenter) {
    double scaleX = (double) frameSize.width / (double) width;
    double scaleY = (double) frameSize.height / (double) height;

    return Matrix::TranslationMatrix(viewportCenter.x, viewportCenter.y) * 
           Matrix::ScaleMatrix(scaleX, scaleY);
  }

public:
  Window(uint width, uint height, DisplayFile* displayFile) : width(width), height(height), displayFile(displayFile), rotation(0), centroid(Point(0, 0)) { }

  void setSize() { width = (width == 500) ? 100 : 500; height = (height == 250) ? 100 : 250; }

  void rotate(double diffRotation) { rotation += diffRotation; }

  void move(Point to) { centroid += Point(to.x, -to.y, to.z); }

  vector<Line> transformViewport(RectangleSize frameSize, Point viewportCenter) {
    vector<Line> lines = normalizeDisplayFile();

    Clipping clipping({width, height});

    clipping.execute(&lines);

    for(Line& line : lines)
      line.applyMatrix(transformationMatrix(frameSize, viewportCenter));
    
    return lines;
  }
};
