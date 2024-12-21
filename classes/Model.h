#pragma once

#include <vector>
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

#include "Drawable.h"
#include "Transformable.h"
#include "Polygon.h"

class Model : public Drawable, public Transformable {
private:
  string name;
  vector<Polygon> polygons;

public:
  string getName() const override { return name; }

  Matrix getMatrix() const override {
    return transformationMatrix();
  }

  vector<Point> getPoints() const override { 
    vector<Point> points;

    for(Polygon polygon : polygons) {
      vector<Point> polygonPoints = polygon.getPoints();

      for(Point point : polygonPoints) points.push_back(point);
    }

    return points; 
  }

  Model(vector<Polygon> polygons, Point ref = Point(), const string& name = "Model") : 
    Transformable(ref),
    name(name), 
    polygons(polygons) {}
  
  void checkItself() const override {
    printf("Model %s\n", name.c_str());
    ref.checkItself(); 
    printf("  X rotation: %.2f\n", xRotation);
    printf("  Y rotation: %.2f\n", yRotation);
    printf("  Z rotation: %.2f\n\n", zRotation);
    for(Polygon polygon : polygons) polygon.checkItself();
    printf("}\n");
  }
};
