#ifndef POLYGON_H
#define POLYGON_H

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include "Drawable.h"
#include "Point.h"
#include "Line.h"

#include <vector>
#include <cmath>

using sizet = std::size_t;

class Polygon : public Drawable {
private:
  std::string name;

public:
  std::vector<Point> points;
  std::vector<Line>* lines;
  Point ref;

  std::string getName() const override { return name; }

  Polygon clone() {
    std::vector<Point*> points = getPoints();
    std::vector<Point*> newPoints = std::vector<Point*>();
    std::vector<Line>* newLines = new std::vector<Line>(); 

    newPoints.push_back(new Point(*points[0]));
    for(sizet i = 0; i < points.size(); i++) {
      if(i + 1 < points.size()) newPoints.push_back(new Point(*points[i + 1]));
      newLines->push_back(Line(newPoints[i], newPoints[(i + 1 < points.size()) ? i + 1 : 0]));
    }

    return Polygon(this->name, this->ref, newLines);
  }

  static bool isPolygon(std::vector<Line>* linesList){
    if(linesList->size() <= 2) return false;

    linesList->push_back((*linesList)[0]);
    for(uint i = 0; i + 1 < linesList->size(); i++) 
      if((*linesList)[i].b != (*linesList)[i + 1].a) return false;
    linesList->pop_back();

    return true;
  }

  static Polygon* createRegularPolygon(int size, int sides, Point centroid, std::string name = "Polygon") {
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

    return new Polygon(name, centroid, lines);
  }

  Polygon(std::vector<Line>* linesList, Point ref) : Polygon("Polygon", ref, linesList) {} 
  Polygon(const std::string& name, Point ref, std::vector<Line>* linesList) {
    if(!Polygon::isPolygon(linesList)) 
      throw std::invalid_argument("It couldn't be polygon.");

    this->name = name;
    this->ref = ref;
    this->lines = linesList;

    Point centroid = this->calculateCentroid();

    for(sizet i = 0; i < linesList->size(); i++) {
      (*this->lines)[i].a->x -= centroid.x;
      (*this->lines)[i].a->y -= centroid.y;
    }
  }
  
  void checkItself() const override {
    printf("%s {\n", name.c_str());
    printf("Ref: "); ref.checkItself(); printf("\n");
    printf("Centroid: "); calculateCentroid().checkItself(); printf("\n");
    for(uint i = 0; i < lines->size(); i++){
      printf("  ");
      (*lines)[i].checkItself();
      printf("\n");
    }
    printf("}\n");
  }

  std::vector<Point*> getPoints() {
    std::vector<Point*> points;

    for(uint i = 0; i < (*lines).size(); i++)
      points.push_back((*lines)[i].a);

    return points;
  }

  Point calculateCentroid() const {
    int size = lines->size();
    if (size == 0) throw std::invalid_argument("There is no points in centroid calculation.");

    int Cx = 0, Cy = 0;
    Point* iterator;

    for(int i = 0; i < size; i++){
      iterator = (*lines)[i].a;
      Cx += iterator->x;
      Cy += iterator->y;
    }

    return Point(Cx/size, Cy/size, 0);
  }

  void draw(QPainter* painter) const override {
    for(uint i = 0; i < (*lines).size(); i++) {
      Line line = Line(new Point(*((*lines)[i].a) + ref), new Point((*(*lines)[i].b) + ref));
      line.draw(painter);
    }
  }

  void rotate(double theta_degree) {
    double theta_radian = theta_degree * M_PI / 180.0;
    Matrix rotationMatrix = Matrix({
      {std::cos(theta_radian), -std::sin(theta_radian), 0, 0},
      {std::sin(theta_radian), std::cos(theta_radian), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    });

    std::vector<Point*> points = getPoints();

    Matrix iterator = Matrix(1, 4);
    for(sizet i = 0; i < points.size(); i++) {
      iterator = rotationMatrix * points[i]->toMatrix();
      *points[i] = iterator;
    }
  }

  void setRef() {
    Point centroid = this->calculateCentroid();
    if(centroid == Point(0, 0)) return;

    this->ref += centroid;

    std::vector<Point*> points = getPoints();

    for(int i = 0; i < (int) points.size(); i++)
      *points[i] -= centroid;
  }

  void move(Point to) { ref = ref + to; }

  void scale(double scaleX, double scaleY) {
    this->applyMatrix(Matrix::ScaleMatrix(scaleX, scaleY));
  }

  void applyMatrix(Matrix matrix) {
    std::vector<Point*> points = getPoints();

    for(Point* point: points) 
      point->applyMatrix(matrix); 
  }

  void scale(double scale) { this->scale(scale, scale); }
};

#endif
