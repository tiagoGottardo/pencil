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
public:
  Point* a;
  Point* b;

  Line(Point* a, Point* b) : Line(a, b, "Line") {}

  Line() : Line(new Point(), new Point(), "Line") {}

  Line(Point* a, Point* b, const std::string& name) : Drawable(name), a(a), b(b) {}

  void checkItself() const override {
    printf("%s |", name.c_str());
    a->checkItself();
    printf(", ");
    b->checkItself();
    printf("|");
  }

  void applyMatrix(Matrix matrix) {
    a->applyMatrix(matrix);
    b->applyMatrix(matrix);
  }

  void draw(QPainter* painter) const override { painter->drawLine(a->x, a->y, b->x, b->y); } };

#endif
