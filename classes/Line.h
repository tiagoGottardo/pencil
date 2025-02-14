#pragma once

#ifndef TEST_MODE
#   include <QPainter>
# else 
#   include "../tests/mocks/QPainterMock.h"
#endif

#include "Drawable.h"
#include "Point.h"

class Line {
private:
  string name;

public:
  Point a;
  Point b;

  Line(Point a = Point(), Point b = Point(), const string& name = "Line") : name(name), a(a), b(b) {}

  string getName() const { return name; }

  vector<Line> getLines() const { return vector<Line>{ Line(a, b) }; }

  void checkItself() const {
    printf("%s: ", name.c_str());
    printf("(%.2f, %.2f, %.2f) -> ", a.x, a.y, a.z);
    printf("(%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
  }

  Line& applyMatrix(Matrix matrix) {
    a.applyMatrix(matrix);
    b.applyMatrix(matrix);

    return *this;
  }

  void draw(QPainter* painter, int frameHeight) const { painter->drawLine(a.x, frameHeight - a.y, b.x, frameHeight - b.y); } 

  bool operator==(const Line& other) const { return (a == other.a && b == other.b); }
  bool operator!=(const Line& other) const { return !(*this == other); }
};
