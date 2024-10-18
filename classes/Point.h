#ifndef POINT_H
#define POINT_H

#include "Thing.h"

#include <QPainter>

#include <iostream>
#include <sstream>

class Point : public Thing {
public:
  int32_t x;
  int32_t y;
  int32_t z;

  std::string name = "Dot";

  Point(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {};

  std::string self(){
    std::stringstream ss;
    ss << name << "(" << x << ", " << y << ", " << z << ")";
    return ss.str();
  }

  void checkItself() const override {
    printf("Dot : \n");
    // std::cout << self() << std::endl;
  };

  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }
};

#endif
