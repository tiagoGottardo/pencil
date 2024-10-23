#ifndef POLYGON_H
#define POLYGON_H

#include "Drawable.h"
#include <QPainter>
#include <iterator>
#include <vector>
#include <cmath>
#include <numbers>
#include <initializer_list>

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
    return createRegularPolygon(size, sides, centroid, "Polygon");
  }

  static Polygon* createRegularPolygon(int size, int sides, Point centroid, std::string name) {
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

    return new Polygon(lines, name);
  }

  Polygon(const std::vector<std::vector<int>>& matrix) : Polygon(matrix, "Polygon") {}

  Polygon(const std::vector<std::vector<int>>& matrix, const std::string& name) : name(name) {
    std::vector<Point*> points;

    for(std::vector pointArray : matrix)
      points.push_back(new Point(pointArray[0], pointArray[1], (pointArray.size() == 3) ? pointArray[2] : 0));

    lines = new std::vector<Line>(); 

    for(int i = 0; i + 1 < static_cast<int>(points.size()); i++)
      lines->push_back(Line(points[i], points[i + 1]));
    lines->push_back(Line(points[points.size() - 1], points[0]));
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
    printf("%s {\n", name.c_str());
    for(uint i = 0; i < lines->size(); i++){
      printf("  ");
      (*lines)[i].checkItself();
      printf("\n");
    }
    printf("}\n");
  }

  std::vector<Point*> getPoints(){
    std::vector<Point*> points;

    for(uint i = 0; i < (*lines).size(); i++)
      points.push_back((*lines)[i].a);

    return points;
  }

  void redimensionXY(float s){
    std::vector<Point*> points = getPoints();

    Point centroid = calculateCentroid();

    for(uint i = 0; i < points.size(); i++){
      points[i]->y = centroid.y + s * (points[i]->y - centroid.y);
      points[i]->x = centroid.x + s * (points[i]->x - centroid.x);
    }
  }

  Point calculateCentroid(){
    int Cx = 0, Cy = 0;
    std::vector<Point*> points = getPoints();

    uint size = points.size();

    if (size == 0) throw std::invalid_argument("There is no points in centroid calculation.");

    for(uint i = 0; i < size; i++){
        Cx += points[i]->x;
        Cy += points[i]->y;
    }

    return Point(Cx/size, Cy/size, 0);
  }

  void draw(QPainter* painter) const override {
    for(uint i = 0; i < (*lines).size(); i++)
      (*lines)[i].draw(painter);
  }
};

#endif
