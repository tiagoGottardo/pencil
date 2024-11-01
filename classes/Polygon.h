#ifndef POLYGON_H
#define POLYGON_H

#include "Drawable.h"
#include "Point.h"
#include "Line.h"

#include <QPainter>
#include <vector>
#include <cmath>

using sizet = std::size_t;

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

    for(sizet i = 0; i < points.size(); i++)
      lines->push_back(Line(points[i], points[(i + 1 < points.size()) ? i + 1 : 0]));

    return new Polygon(lines, name);
  }

  Polygon(const std::vector<std::vector<int>>& matrix) : Polygon(matrix, "Polygon") {}

  Polygon(const std::vector<std::vector<int>>& matrix, const std::string& name) : name(name) {
    std::vector<Point*> points;

    for(std::vector pointArray : matrix)
      points.push_back(new Point(pointArray[0], pointArray[1], (pointArray.size() == 3) ? pointArray[2] : 0));

    lines = new std::vector<Line>(); 

    ref = Point(points[0]->x, points[0]->y, 0);
    for(sizet i = 0; i < points.size(); i++) {
      lines->push_back(Line(points[i], points[(i + 1 < points.size()) ? i + 1 : 0]));

      if(points[i]->x < ref.x) ref.x = points[i]->x;
      if(points[i]->y > ref.y) ref.y = points[i]->y;
    }

    for(sizet i = 0; i < lines->size(); i++) {
      (*lines)[i].a->x -= ref.x;
      (*lines)[i].a->y -= ref.y;
    }

  } 

  Polygon(std::vector<Line>* linesList) : Polygon(linesList, "Polygon") {} 
  
  Polygon(std::vector<Line>* linesList, const std::string& name) : name(name) {
    if(!Polygon::isPolygon(linesList)) {
      printf("It couldn't be polygon.");
      return;
    }

    ref = Point((*linesList)[0].a->x, (*linesList)[0].a->y, 0);

    Point iterator = Point();
    for(sizet i = 1; i < linesList->size(); i++) {
      iterator = *((*linesList)[i].a);
      if(iterator.x < ref.x) ref.x = iterator.x;
      if(iterator.y > ref.y) ref.y = iterator.y;
    }

    for(sizet i = 0; i < linesList->size(); i++) {
      (*linesList)[i].a->x -= ref.x;
      (*linesList)[i].a->y -= ref.y;
    }

    lines = linesList;
  }
  
  void checkItself() const override {
    printf("%s {\n", name.c_str());
    printf("Ref: "); ref.checkItself(); printf("\n");
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
    Line line;
    for(uint i = 0; i < (*lines).size(); i++) {
      line = Line(new Point(*((*lines)[i].a) + ref), new Point((*(*lines)[i].b) + ref));
      line.draw(painter);
    }
  }

  void rotate(double theta_degree) {
    double theta_radian = theta_degree * M_PI / 180.0;
    Matrix rotation = Matrix({
      {std::cos(theta_radian), -std::sin(theta_radian), 0},
      {std::sin(theta_radian), std::cos(theta_radian), 0},
      {0, 0, 1}
    });

    std::vector<Point*> points = getPoints();

    Matrix iterator = Matrix(1, 3);
    for(sizet i = 0; i < points.size(); i++) {
      iterator = rotation * points[i]->toMatrix();
      points[i]->x = iterator[0][0];
      points[i]->y = iterator[1][0];
      points[i]->z = iterator[2][0];
    }
  };
  void move(Point to) {
    ref = ref + to;
  };

  void redimensionXY(double scale) {
    Matrix scaleMatrix = Matrix::IdentityMatrix(3) * scale;

    std::vector<Point*> points = getPoints();

    Matrix iterator = Matrix(1, 3);
    for(sizet i = 0; i < points.size(); i++) {
      iterator = scaleMatrix * points[i]->toMatrix();
      points[i]->x = iterator[0][0];
      points[i]->y = iterator[1][0];
      points[i]->z = iterator[2][0];
    }

  };
};

#endif
