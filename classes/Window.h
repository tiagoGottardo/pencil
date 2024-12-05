#ifndef WINDOW_H
#define WINDOW_H

#include <optional>

#include "./Point.h"
#include "./Polygon.h"

enum LineStatus {
  COMPLETELY_INSIDE,
  COMPLETELY_OUTSIDE,
  INTERSECTION_CHECK_NEEDED,
  HAS_INTERSECTION,
};

typedef struct RectangleSize {
  uint width, height;
} RectangleSize;

class Window {
private:
  uint width, height;
  std::vector<Drawable*> *displayFile;
  double rotation;
  Point centroid;

  Point maxPoint() { return Point((int) this->width / 2, (int) this->height / 2); }
  Point minPoint() { return -this->maxPoint(); }

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

  char calculateRC(Point point) {
    char result = 0;

    if(point.x < this->minPoint().x) result += 1;
    if(point.x > this->maxPoint().x) result += 2;
    if(point.y < this->minPoint().y) result += 4;
    if(point.y > this->maxPoint().y) result += 8;

    return result;
  }

  std::optional<Point> calculateIntersection(Point p1, Point p2, double clipCoord, bool isVertical) {
    double t;

    if (isVertical) {
      if (p1.x == p2.x) return std::nullopt;
      t = (clipCoord - (double) p1.x) / ((double) p2.x - (double) p1.x);
    } else {
      if (p1.y == p2.y) return std::nullopt; 
      t = (clipCoord - (double) p1.y) / ((double) p2.y - (double) p1.y);
    }

    if (t < 0 || t > 1) return std::nullopt; 

    double x = p1.x + t * (p2.x - p1.x);
    double y = p1.y + t * (p2.y - p1.y);
    return Point(x, y);
  }

  LineStatus resolveIntersection(Line line) {
    std::optional<Point> iterator;
    LineStatus lineStatus = LineStatus::COMPLETELY_OUTSIDE;
    
    iterator = calculateIntersection(*line.a, *line.b, (double) this->maxPoint().y, false);
    if(iterator && iterator->x >= this->minPoint().x && iterator->x <= this->maxPoint().x) {
      (line.a->y > line.b->y) ? *line.a = *iterator : *line.b = *iterator;
      lineStatus = LineStatus::HAS_INTERSECTION;
    }

    iterator = calculateIntersection(*line.a, *line.b, (double) this->minPoint().y, false);
    if(iterator && iterator->x >= this->minPoint().x && iterator->x <= this->maxPoint().x) {
      (line.a->y < line.b->y) ? *line.a = *iterator : *line.b = *iterator;
      lineStatus = LineStatus::HAS_INTERSECTION;
    }

    iterator = calculateIntersection(*line.a, *line.b, (double) this->maxPoint().x, true);
    if(iterator && iterator->y >= this->minPoint().y && iterator->y <= this->maxPoint().y) {
      (line.a->x > line.b->x) ? *line.a = *iterator : *line.b = *iterator;
      lineStatus = LineStatus::HAS_INTERSECTION;
    }

    iterator = calculateIntersection(*line.a, *line.b, (double) this->minPoint().x, true);
    if(iterator && iterator->y >= this->minPoint().y && iterator->y <= this->maxPoint().y) {
      (line.a->x < line.b->x) ? *line.a = *iterator : *line.b = *iterator;
      lineStatus = LineStatus::HAS_INTERSECTION;
    }
    
    return lineStatus;
  }

  LineStatus calculateRCStatus(Line line) {
    char aRC = calculateRC(*line.a);
    char bRC = calculateRC(*line.b);
    if(aRC == 0 && bRC == 0) return COMPLETELY_INSIDE;
    if(aRC == 0 || bRC == 0) return HAS_INTERSECTION; 
    if((aRC & bRC) == 0) return INTERSECTION_CHECK_NEEDED;
    
    return COMPLETELY_OUTSIDE;
  }

  void clip(std::vector<Line>* lines) {
    for(int i = 0; i < (int) lines->size(); i++) {
      LineStatus RCStatus = calculateRCStatus((*lines)[i]);

      if(RCStatus == LineStatus::COMPLETELY_OUTSIDE) {
        lines->erase(lines->begin() + i); 
        i--; continue;
      }

      if(RCStatus != LineStatus::COMPLETELY_INSIDE && resolveIntersection((*lines)[i]) != LineStatus::HAS_INTERSECTION)
        lines->erase(lines->begin() + i);
    }
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

    this->clip(&lines);

    Matrix transformationMatrix = calculateTransformationMatrix(viewportSize);

    for(Line line : lines)
      line.applyMatrix(transformationMatrix);
    
    return lines;
  }
};

#endif
