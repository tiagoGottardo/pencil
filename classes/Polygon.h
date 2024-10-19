#ifndef POLYGON_H
#define POLYGON_H

#include "Drawable.h"
#include <QPainter>
#include <vector>

class Polygon : public Drawable {
public:
  std::vector<Line>* lines;
  Point ref = Point(0, 0, 0);

  std::string name = "Polygon";

  Polygon(std::vector<Line>* linesList) {
    if(linesList->size() <= 2) {
      printf("It need 3 points or more to be polygon.\n");
      return;
    }

    linesList->push_back((*linesList)[0]);
    for(uint i = 0; i + 1 < linesList->size(); i++){
      if((*linesList)[i].b != (*linesList)[i + 1].a){
        printf("It couldn't be polygon.\n");
        return;
      }
    }
    linesList->pop_back();

    lines = linesList;
  }

  void checkItself() const override {
    printf("Polygon {\n");
    for(uint i = 0; i < lines->size(); i++){
      (*lines)[i].checkItself();
    }
    printf("}\n");
  }

  std::vector<Point*> getDots(){
    std::vector<Point*> dots;

    for(uint i = 0; i < (*lines).size(); i++){
      dots.push_back((*lines)[i].a);
    }

    return dots;
  };

  void redimensionXY(float s){
    std::vector<Point*> dots = getDots();

    Point centroid = calculateCentroid();

    for(uint i = 0; i < dots.size(); i++){
      dots[i]->y = centroid.y + s * (dots[i]->y - centroid.y);
      dots[i]->x = centroid.x + s * (dots[i]->x - centroid.x);
    }
  };

  Point calculateCentroid(){
    int Cx = 0, Cy = 0;
    std::vector<Point*> dots = getDots();

    uint size = dots.size();

    if (size == 0){
        throw std::invalid_argument("There is no dots in centroid calculation.");
    };

    for(uint i = 0; i < size; i++){
        Cx += dots[i]->x;
        Cy += dots[i]->y;
    }

    return Point(Cx/size, Cy/size, 0);
  };

  void draw(QPainter* painter) const override {
    for(uint i = 0; i < (*lines).size(); i++) {
      (*lines)[i].draw(painter);
    }
  }
};

#endif
