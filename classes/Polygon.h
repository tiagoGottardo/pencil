#pragma once

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

class Polygon : public Drawable {
private:
  friend class PolygonFriend;

  string name;
  Point ref;
  vector<Point> points;

  double xRotation;
  double yRotation;
  double zRotation;

  void triggerRotate() { rotate(1); }
public:
  double validTheta(double theta) {
    while(theta > 360.0 || theta < .0) {
      if(theta > 360.0) theta -= 360.0;
      if(theta < 360.0) theta += 360.0;
    } 
    return theta;
  }

  void rotate(double x, double y, double z) { 
    xRotation += validTheta(x); 
    yRotation += validTheta(y); 
    zRotation += validTheta(z); 
  }

  void rotate(double theta) { rotate(theta, theta, theta); }
  void rotateX(double theta) { xRotation += validTheta(theta); }
  void rotateY(double theta) { yRotation += validTheta(theta); }
  void rotateZ(double theta) { zRotation += validTheta(theta); }

  string getName() const override { return name; }

  vector<Point> getTransformedPoints() const {  
    vector<Point> result;

    for(Point point : points) {
      point.applyMatrix(transformationMatrix());
      result.push_back(point);
    }

    return result;
  }

  vector<Line> getLines() const override { 
    vector<Line> lines;

    vector<Point> transformedPoints = getTransformedPoints();

    for(size_t i = 0; i < transformedPoints.size(); i++)
      lines.push_back(Line(transformedPoints[i], transformedPoints[(i + 1 < transformedPoints.size()) ? i + 1 : 0]));

    return lines; 
  }

  static Polygon createRegularPolygon(int size = 100, int sides = 3, Point centroid = Point(), string name = "Polygon") {
    if(sides < 3) throw invalid_argument("It must has at least 3 sides.");

    double R = size / 2;
    double initialAng = -M_PI/2;
    double ang;

    vector<Point> points;

    for(int i = 0; i < sides; i++) {
      if(sides % 2 == 0) initialAng = 0;
      if(sides % 4 == 0) initialAng = -M_PI/sides;
      
      ang = initialAng + (i * 2 * M_PI / sides);

      points.push_back(Point(R * cos(ang), R * sin(ang)));
    }

    return Polygon(points, centroid, name);
  }

  static Polygon createRectangle(uint width, uint height, Point centroid, string name = "Rectangle") {
    vector<Point> points;

    points.push_back(Point());
    points.push_back(Point(0, height));
    points.push_back(Point(width, height));
    points.push_back(Point(width));

    for(Point& point : points) point -= Point(width / 2, height / 2);

    return Polygon(points, centroid, name);
  }

  static vector<Polygon> createDonut(int sides) {
    std::vector<Polygon> polygons;

    int R = 60; 
    int smallR = 35; 

    for (int i = 0; i < sides; i++) {
      double ang = i * 2 * M_PI / sides;
      double nextAng = (i + 1) * 2 * M_PI / sides;

      for (int j = 0; j < sides; j++) {
        double ang2 = j * 2 * M_PI / sides;
        double nextAng2 = (j + 1) * 2 * M_PI / sides;

        Point p1(
          (R + smallR * cos(ang2)) * cos(ang),
          (R + smallR * cos(ang2)) * sin(ang),
          smallR * sin(ang2)
        );
        Point p2(
          (R + smallR * cos(nextAng2)) * cos(ang),
          (R + smallR * cos(nextAng2)) * sin(ang),
          smallR * sin(nextAng2)
        );
        Point p3(
          (R + smallR * cos(nextAng2)) * cos(nextAng),
          (R + smallR * cos(nextAng2)) * sin(nextAng),
          smallR * sin(nextAng2)
        );
        Point p4(
          (R + smallR * cos(ang2)) * cos(nextAng),
          (R + smallR * cos(ang2)) * sin(nextAng),
          smallR * sin(ang2)
        );

        polygons.push_back(Polygon({p1, p2, p3, p4}));
      }
    }

    return polygons;
  };

  static Polygon createSquare(int size, int distance, Point rotation) {
    vector<Point> points;

    int half = size / 2;

    points.push_back(Point(-half, -half, distance));
    points.push_back(Point(half, -half, distance));
    points.push_back(Point(half, half, distance));
    points.push_back(Point(-half, half, distance));

    Polygon p = Polygon(points, Point());

    p.applyMatrix(
      Matrix::XRotationMatrix(rotation.x) *
      Matrix::YRotationMatrix(rotation.y) *
      Matrix::ZRotationMatrix(rotation.z)
    );

    return p;
  };

  Polygon(vector<Point> points, Point ref = Point(), const string& name = "Polygon") : name(name), ref(Point(ref.x, ref.y, ref.z, "Ref")), points(points) { 
    xRotation = .0;
    yRotation = .0;
    zRotation = .0;
  }
  
  void checkItself() const override {
    printf("%s: {\n", name.c_str());
    ref.checkItself(); 
    calculateCentroid().checkItself();
    printf("  X rotation: %.2f\n", xRotation);
    printf("  Y rotation: %.2f\n", yRotation);
    printf("  Z rotation: %.2f\n\n", zRotation);
    for(Point point : points) point.checkItself();
    printf("}\n");
  }

  Point calculateCentroid() const {
    int size = points.size();
    if (size == 0) throw invalid_argument("There is no points in centroid calculation.");

    Point C = Point();
    for(Point point : points) C += point;

    return C / size;
  }

  void move(Point to) { ref = ref + to; }

  Matrix transformationMatrix() const {
    return  
      Matrix::TranslationMatrix(ref.x, ref.y, ref.z) *
      Matrix::XRotationMatrix(xRotation) *
      Matrix::YRotationMatrix(yRotation) *
      Matrix::ZRotationMatrix(zRotation);
  }

  void scale(double x, double y, double z) { this->applyMatrix(Matrix::ScaleMatrix(x, y, z)); }
  void scale(double factor) { this->scale(factor, factor, factor); }
  void applyMatrix(Matrix matrix) { for(Point& point : points) point.applyMatrix(matrix); }
};
