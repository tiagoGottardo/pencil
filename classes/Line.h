#ifndef LINE_H
#define LINE_H

#include <QPainter>

class Line {
public:
  Point* a;
  Point* b;

  std::string name = "Line";

  Line(Point* a, Point* b) : a(a), b(b) {};

  std::string self() {
    std::stringstream ss;
    ss << name << "|" << a->self() << " -> " << b->self() << "|";
    return ss.str();
  };

  void checkItself(){
    std::cout << self() << std::endl;
  };

  void draw(QPainter* painter){
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
