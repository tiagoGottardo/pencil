#ifndef POLYGON_H
#define POLYGON_H

#include "Drawable.h"
#include <QPainter>
#include <vector>
#include <cmath>
#include <numbers>

class Polygon : public Drawable {
public:
  std::vector<Line>* lines;
  Point ref = Point(0, 0, 0);

  std::string name;

  static bool isPolygon(std::vector<Line>* linesList){
    if(linesList->size() <= 2) return false;

    linesList->push_back((*linesList)[0]);
    for(uint i = 0; i + 1 < linesList->size(); i++) 
      if((*linesList)[i].b != (*linesList)[i + 1].a) return false;
    linesList->pop_back();

    return true;
  }

  static Polygon* createRegularPolygon(int size, int sides, Point centroid) {
    if(sides <= 0) return NULL;

    double R = size / 2;
    double initialAng;
    double ang;

    std::vector<Point*> points;

    for(int i = 0; i < sides; i++) {
      initialAng = -M_PI/2;
      if(sides % 2 == 0) initialAng = 0;
      if(sides % 4 == 0) initialAng = -M_PI/sides;
      
      ang = initialAng + (i * 2 * M_PI / sides);

      points.push_back(new Point(centroid.x + R * std::cos(ang), centroid.y + R * std::sin(ang)));
    }

    std::vector<Line>* lines = new std::vector<Line>();

    for(int i = 0; i + 1 < static_cast<int>(points.size()); i++)
      lines->push_back(Line(points[i], points[i + 1]));
    lines->push_back(Line(points[points.size() - 1], points[0]));

    return new Polygon(lines);
  }

  Polygon(std::vector<Line>* linesList) : Polygon(linesList, "Polygon") {} 
  
  Polygon(std::vector<Line>* linesList, const std::string& name) : name(name) {
    if(!Polygon::isPolygon(linesList)) {
      printf("It couldn't be polygon.");
      return;
    }

    lines = linesList;
  }
  
  void checkItself() const override {
    printf("Polygon {\n");
    for(uint i = 0; i < lines->size(); i++){
      printf("  ");
      (*lines)[i].checkItself();
      printf("\n");
    }
    printf("}\n");
  }

  std::vector<Point*> getDots(){
    std::vector<Point*> dots;

    for(uint i = 0; i < (*lines).size(); i++)
      dots.push_back((*lines)[i].a);

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

    if (size == 0) throw std::invalid_argument("There is no dots in centroid calculation.");

    for(uint i = 0; i < size; i++){
        Cx += dots[i]->x;
        Cy += dots[i]->y;
    }

    return Point(Cx/size, Cy/size, 0);
  };

  void draw(QPainter* painter) const override {
    for(uint i = 0; i < (*lines).size(); i++)
      (*lines)[i].draw(painter);
  }
};

#endif
