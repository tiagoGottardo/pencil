#ifndef LINE_H
#define LINE_H

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

  void applyMatrix(Matrix matrix) {
    a.applyMatrix(matrix);
    b.applyMatrix(matrix);
  }

  void draw(QPainter* painter) const { painter->drawLine(a.x, a.y, b.x, b.y); } };

#endif
