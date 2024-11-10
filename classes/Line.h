#ifndef LINE_H
#define LINE_H

#include "Drawable.h"
#include "Point.h"

#include <QPainter>

class Line : public Drawable {
public:
  Point* a;
  Point* b;

  Line(Point* a, Point* b) : Line(a, b, "Line") {}

  Line() : Line(new Point(), new Point(), "Line") {}

  Line(Point* a, Point* b, const std::string& name) : a(a), b(b), Drawable(name) {}

  void checkItself() const override {
    printf("%s |", name.c_str());
    a->checkItself();
    printf(", ");
    b->checkItself();
    printf("|");
  }

  void draw(QPainter* painter) const override {
    painter->drawLine(a->x, a->y, b->x, b->y);
  }

  double calculateAngular(){
    int32_t dy = b->y - a->y;
    int32_t dx = b->x - a->x;

    if(dx == 0) {
        return std::numeric_limits<double>::infinity();
    }

    return dy/dx;
  }
};

#endif
