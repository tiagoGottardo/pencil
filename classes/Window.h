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
  vector<Drawable*> *displayFile;
  double rotation;
  Point centroid;

  Point maxPoint() { return Point((int) width / 2, (int) height / 2); }
  Point minPoint() { return -maxPoint(); }

  friend class WindowFriend;

  vector<Line> normalizeDisplayFile() {
    vector<Line> result = vector<Line>();

    Matrix normalizationMatrix = Matrix::ZRotationMatrix(-rotation) * Matrix::TranslationMatrix(-centroid.x, -centroid.y, -centroid.z);

    for(Drawable* drawable : *displayFile) {
      vector<Line> lines = drawable->getLines();

      for(Line line : lines) {
        line.applyMatrix(normalizationMatrix);
        result.push_back(line);
      }
    }

    return result;
  }

  Matrix calculateTransformationMatrix(RectangleSize viewportSize) {
    double scaleX = (double) viewportSize.width / (double) width;
    double scaleY = (double) viewportSize.height / (double) height;

    double halfViewportWidth = (double) viewportSize.width / 2.0;
    double halfViewportHeight = (double) viewportSize.height / 2.0;

    return Matrix::TranslationMatrix(halfViewportWidth, halfViewportHeight) * Matrix::ScaleMatrix(scaleX, scaleY);
  }

  char calculateRC(Point point) {
    char result = 0;

    if(point.x < minPoint().x) result += 1;
    if(point.x > maxPoint().x) result += 2;
    if(point.y < minPoint().y) result += 4;
    if(point.y > maxPoint().y) result += 8;

    return result;
  }

  Point getIntersectionPoint(Line* line, double t) {
    return Point(line->a.x + t * (line->b.x - line->a.x), line->a.y + t * (line->b.y - line->a.y));
  }

  optional<Point> findHorizontalIntersection(Line* line, double coordinate) {
    if(line->a.y == line->b.y) return nullopt; 
    double t = (coordinate - (double) line->a.y) / ((double) line->b.y - (double) line->a.y);

    if (t < 0 || t > 1) return nullopt; 
    return getIntersectionPoint(line, t);
  }

  optional<Point> findVerticalIntersection(Line* line, double coordinate) {
    if(line->a.x == line->b.x) return nullopt;
    double t = (coordinate - (double) line->a.x) / ((double) line->b.x - (double) line->a.x);

    if (t < 0 || t > 1) return nullopt; 
    return getIntersectionPoint(line, t);
  }

  bool isBetweenLeftAndRight(Point point) { return point.x > minPoint().x && point.x < maxPoint().x; }
  bool isBetweenTopAndBottom(Point point) { return point.y > minPoint().y && point.y < maxPoint().y; }

  LineStatus resolveIntersection(Line *line) {
    optional<Point> intersection;
    LineStatus result = COMPLETELY_OUTSIDE;

    auto handleIntersection = [&](optional<Point> intersection, bool condition, Point pointToUpdate) {
      if (intersection && condition) {
        pointToUpdate = *intersection;
        result = HAS_INTERSECTION;
      }
    };

    handleIntersection(
        findHorizontalIntersection(line, maxPoint().y),
        isBetweenLeftAndRight(*intersection),
        (line->a.y > line->b.y) ? line->a : line->b
    );

    handleIntersection(
        findHorizontalIntersection(line, minPoint().y),
        isBetweenLeftAndRight(*intersection),
        (line->a.y < line->b.y) ? line->a : line->b
    );

    handleIntersection(
        findVerticalIntersection(line, maxPoint().x),
        isBetweenTopAndBottom(*intersection),
        (line->a.x > line->b.x) ? line->a : line->b
    );

    handleIntersection(
        findVerticalIntersection(line, minPoint().x),
        isBetweenTopAndBottom(*intersection),
        (line->a.x < line->b.x) ? line->a : line->b
    );
    
    return result;
  }

  LineStatus calculateRCStatus(Line line) {
    char aRC = calculateRC(line.a);
    char bRC = calculateRC(line.b);
    if(aRC == 0 && bRC == 0) return COMPLETELY_INSIDE;
    if(aRC == 0 || bRC == 0) return HAS_INTERSECTION; 
    if((aRC & bRC) == 0) return INTERSECTION_CHECK_NEEDED;
    
    return COMPLETELY_OUTSIDE;
  }

  void clip(vector<Line>* lines) {
    for(int i = 0; i < (int) lines->size(); i++) {
      LineStatus RCStatus = calculateRCStatus((*lines)[i]);

      if(RCStatus == COMPLETELY_OUTSIDE) {
        lines->erase(lines->begin() + i); i--; continue;
      }

      if(RCStatus != COMPLETELY_INSIDE && resolveIntersection(&(*lines)[i]) != HAS_INTERSECTION) {
        lines->erase(lines->begin() + i); i--;
      }
    }
  }

public:
  Window(uint width, uint height, vector<Drawable*>* displayFile) : width(width), height(height), displayFile(displayFile), rotation(0), centroid(Point(0, 0)) { }

  void setSize() { width = (width == 500) ? 100 : 500; height = (height == 250) ? 100 : 250; }

  void rotate(double diffRotation) { rotation += diffRotation; }

  void move(Point to) { centroid += to; }

  vector<Line> transformViewport(RectangleSize viewportSize) {
    vector<Line> lines = normalizeDisplayFile();

    // clip(&lines);

    Matrix transformationMatrix = calculateTransformationMatrix(viewportSize);

    for(Line& line : lines)
      line.applyMatrix(transformationMatrix);
    
    return lines;
  }
};

#endif
