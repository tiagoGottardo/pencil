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
  std::string name;

public:

  Point* a;
  Point* b;

  Line(Point* a, Point* b, const std::string& name = "Line") : name(name), a(a), b(b) {}

  std::string getName() const override { return name; }

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
