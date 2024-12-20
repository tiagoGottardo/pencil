#pragma once

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include "Drawable.h"
#include "Point.h"

class Line : public Drawable {
private:
  string name;

public:
  Point a;
  Point b;

  Line(Point a, Point b, const string& name = "Line") : name(name), a(a), b(b) {}

  string getName() const override { return name; }

  vector<Line> getLines() const override { return vector<Line>{ Line(a, b) }; }

  void checkItself() const override {
    printf("%s: ", name.c_str());
    printf("(%d, %d, %d) -> ", a.x, a.y, a.z);
    printf("(%d, %d, %d)\n", b.x, b.y, b.z);
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
