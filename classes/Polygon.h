#ifndef POLYGON_H
#define POLYGON_H

#include "Thing.h"
#include <QPainter>

class Polygon : public Thing {
public:
  std::vector<Line>* lines;
  Point ref = Point(0, 0, 0);

  std::string name = "Polygon";

  Polygon(std::vector<Line>* linesList) {
    if(linesList->size() <= 2) {
      printf("You need 3 dots or more to create a polygon.\n");
      return;
    }

    bool couldBePolygon = true;
    for(uint i = 0; i < linesList->size(); i++){
      if(i + 1 == linesList->size()){
        if((*linesList)[i].b != (*linesList)[0].a){
          couldBePolygon = false;
        }
        if((*linesList)[i].calculateAngular() == (*linesList)[i + 1].calculateAngular()){
          printf("There are two lines in place of one.\n");
          couldBePolygon = false;
        }
        break;
      }

      if((*linesList)[i].b != (*linesList)[i + 1].a){
        couldBePolygon = false;
        break;
      }

      if((*linesList)[i].calculateAngular() == (*linesList)[i + 1].calculateAngular()){
        printf("There are two lines in place of one.\n");
        break;
      }
    }

    if(!couldBePolygon) {
      printf("The lines doesn't makes a possible polygon.\n");
      return;
    }

    lines = linesList;
  }

  void checkItself() const override{
    printf("Polygon {\n");
    // for(uint i = 0; i < lines->size(); i++){
    //   (*lines)[i].checkItself();
    // }
    printf("}\n");
  }

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

  void draw(QPainter* painter){
    for(uint i = 0; i < (*lines).size(); i++) {
      (*lines)[i].draw(painter);
    }
  }
};

#endif
