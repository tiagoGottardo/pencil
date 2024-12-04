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
    std::vector<Line> lines = std::vector<Line>();

    Polygon* iterator;
    for(sizet i = 0; i < this->displayFile->size(); i++) {
      iterator = dynamic_cast<Polygon*>((*this->displayFile)[i]);

      for(sizet j = 0; j < iterator->lines->size(); j++) {
        Line line = (*iterator->lines)[j];

        lines.push_back(Line(
          new Point((*line.a) + iterator->ref - this->centroid),
          new Point((*line.b) + iterator->ref - this->centroid))
        );
      }
    }

    return lines;
  }

  Matrix calculateTransformationMatrix(RectangleSize viewportSize) {
    double theta_radian = -(this->rotation * M_PI / 180.0);
    Matrix rotationMatrix = Matrix({
      {std::cos(theta_radian), -std::sin(theta_radian), 0, 0},
      {std::sin(theta_radian), std::cos(theta_radian), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    });

    Matrix scaleMatrix = Matrix::IdentityMatrix(4);
    scaleMatrix[0][0] = (double) viewportSize.width / (double) this->width;
    scaleMatrix[1][1] = (double) viewportSize.height / (double) this->height;

    Matrix translationMatrix = Matrix::IdentityMatrix(4);
    translationMatrix[0][3] = ((double)(viewportSize.width))/2.0;
    translationMatrix[1][3] = ((double)(viewportSize.height))/2.0;

    if(this->rotation == 0) 
      return translationMatrix * scaleMatrix;
     else 
      return translationMatrix * scaleMatrix * rotationMatrix;
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

    Matrix transformationMatrix = calculateTransformationMatrix(viewportSize);

    for(sizet i = 0; i < lines.size(); i++) {
      *lines[i].a = transformationMatrix * lines[i].a->toMatrix();
      *lines[i].b = transformationMatrix * lines[i].b->toMatrix();
    }

    return lines;
  }
};

#endif
