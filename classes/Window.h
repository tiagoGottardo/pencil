#ifndef WINDOW_H
#define WINDOW_H

#include "./Point.h"
#include "./Polygon.h"

typedef struct RectangleSize {
  uint width, height;
} RectangleSize;

class Window {
private:
  uint width, height;
  std::vector<Drawable*> *displayFile;
  double rotation;
  Point centroid;

  friend class WindowFriend;

  std::vector<Line> normalizeDisplayFile() {
    std::vector<Line> result = std::vector<Line>();

    for(Drawable* drawable : *this->displayFile) {
      Polygon* iterator = dynamic_cast<Polygon*>(drawable);

      Point diffToOrigin = iterator->ref - this->centroid;
      Matrix normalizationMatrix = Matrix::ZRotationMatrix(-this->rotation) * Matrix::TranslationMatrix(diffToOrigin.x, diffToOrigin.y);

      for(Line oldLine : *iterator->lines) {
        Line newLine = Line(new Point(*oldLine.a), new Point(*oldLine.b));

        newLine.applyMatrix(normalizationMatrix);

        result.push_back(newLine);
      }
    }

    return result;
  }

  Matrix calculateTransformationMatrix(RectangleSize viewportSize) {
    double scaleX = (double) viewportSize.width / (double) this->width;
    double scaleY = (double) viewportSize.height / (double) this->height;

    double halfViewportWidth = ((double)(viewportSize.width))/2.0;
    double halfViewportHeight = ((double)(viewportSize.height))/2.0;

    return Matrix::TranslationMatrix(halfViewportWidth, halfViewportHeight) * Matrix::ScaleMatrix(scaleX, scaleY);
  }

public:
  Window(uint width, uint height, std::vector<Drawable*>* displayFile) : width(width), height(height), displayFile(displayFile), rotation(0), centroid(Point(0, 0)) { }

  void setSize(){
    this->width = (this->width == 500) ? 100 : 500;
    this->height = (this->height == 250) ? 100 : 250;
  }

  void rotate(double diffRotation) { this->rotation += diffRotation; }

  void move(Point to) { this->centroid += to; }

  std::vector<Line> transformViewport(RectangleSize viewportSize) {
    std::vector<Line> lines = this->normalizeDisplayFile();

    // Clipping

    Matrix transformationMatrix = calculateTransformationMatrix(viewportSize);

    for(Line line : lines)
      line.applyMatrix(transformationMatrix);
    
    return lines;
  }
};

#endif
