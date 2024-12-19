#pragma once

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

class Clipping {

private:
  Point minPoint;
  Point maxPoint;

  char calculateRC(Point point) {
    char result = 0;

    if(point.x < minPoint.x) result += 1;
    if(point.x > maxPoint.x) result += 2;
    if(point.y < minPoint.y) result += 4;
    if(point.y > maxPoint.y) result += 8;

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

  bool isBetweenLeftAndRight(Point point) { return point.x > minPoint.x && point.x < maxPoint.x; }
  bool isBetweenTopAndBottom(Point point) { return point.y > minPoint.y && point.y < maxPoint.y; }

  void resolveIntersection(Line *line) {
    optional<Point> intersection;

    auto handleIntersection = [&](optional<Point> intersection, bool condition, Point* pointToUpdate) {
      if (intersection && condition) *pointToUpdate = *intersection;
    };

    handleIntersection(
        findVerticalIntersection(line, maxPoint.x),
        isBetweenTopAndBottom(*intersection),
        (line->a.x > line->b.x) ? &line->a : &line->b
    );

    handleIntersection(
        findVerticalIntersection(line, minPoint.x),
        isBetweenTopAndBottom(*intersection),
        (line->a.x < line->b.x) ? &line->a : &line->b
    );
    
    handleIntersection(
        findHorizontalIntersection(line, maxPoint.y),
        isBetweenLeftAndRight(*intersection),
        (line->a.y > line->b.y) ? &line->a : &line->b
    );

    handleIntersection(
        findHorizontalIntersection(line, minPoint.y),
        isBetweenLeftAndRight(*intersection),
        (line->a.y < line->b.y) ? &line->a : &line->b
    );
  }

  LineStatus calculateRCStatus(Line line) {
    char aRC = calculateRC(line.a);
    char bRC = calculateRC(line.b);
    if(aRC == 0 && bRC == 0) return COMPLETELY_INSIDE;
    if(aRC == 0 || bRC == 0) return HAS_INTERSECTION; 
    if((aRC & bRC) == 0) return INTERSECTION_CHECK_NEEDED;
    
    return COMPLETELY_OUTSIDE;
  }

public:

  Clipping(RectangleSize windowSize) { 
    maxPoint = Point((int) windowSize.width / 2, (int) windowSize.height / 2);
    minPoint = -maxPoint;
  }

  void execute(vector<Line>* lines) {
    for(int i = 0; i < (int) lines->size(); i++) {
      LineStatus RCStatus = calculateRCStatus((*lines)[i]);

      if(RCStatus == COMPLETELY_OUTSIDE) {
        lines->erase(lines->begin() + i); i--; continue;
      }

      if(RCStatus != COMPLETELY_INSIDE) {
        resolveIntersection(&(*lines)[i]);

        if(calculateRCStatus((*lines)[i]) == COMPLETELY_OUTSIDE) {
          lines->erase(lines->begin() + i); i--;
        }
      }
    }
  }
};
