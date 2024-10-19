#ifndef LINE_H
#define LINE_H

#include "Drawable.h"

#include <QPainter>

#include <iostream>

class Line : public Drawable {
public:
  Point* a;
  Point* b;

  std::string name;

  Line(Point* a, Point* b, std::string name) : a(a), b(b), name(name) {};
  Line(Point* a, Point* b) : a(a), b(b) { name = "Line"; };

  void checkItself() const override {
    printf("%s |", name.c_str());
    a->checkItself();
    printf(", ");
    b->checkItself();
    printf("|");
  };

  void draw(QPainter* painter) const override {
    painter->drawLine(a->x, a->y, b->x, b->y);
  };

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
