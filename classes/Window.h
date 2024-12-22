#pragma once

#include <chrono>
#include <iostream>

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
      vector<Point> points = drawable->getPoints();

      Matrix transformationMatrix = normalizationMatrix() * drawable->getMatrix();
      
      for(Point& point : points) point.applyMatrix(transformationMatrix);

      for(size_t i = 0; i < points.size(); i++) 
        result.push_back(Line(points[i], points[(i + 1 < points.size()) ? i + 1 : 0]));
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
    auto start = std::chrono::high_resolution_clock::now();

    vector<Line> lines = normalizeDisplayFile();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Normalization: " << duration.count() << endl;





    Clipping clipping({width, height});

    start = std::chrono::high_resolution_clock::now();

    clipping.execute(&lines);

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Clipping: " << duration.count() << endl;





    start = std::chrono::high_resolution_clock::now();

    for(Line& line : lines) line.applyMatrix(transformationMatrix(frameSize, viewportCenter));

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Viewport Transformation: " << duration.count() << endl;
    
    return lines;
  }
};
