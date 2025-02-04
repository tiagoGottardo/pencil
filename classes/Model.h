#pragma once

#include <sstream>

#include "Drawable.h"
#include "Transformable.h"
#include "Polygon.h"

class Model : public Drawable, public Transformable {
private:
  string name;
  vector<Polygon> polygons;

public:
  string getName() const override { return name; }

  Matrix getMatrix() const override { return transformationMatrix(); }

  vector<Polygon> getPolygons() const override { return polygons; }

  Model(vector<Polygon> polygons, const string& name = "Model") : 
    Transformable(),
    name(name), 
    polygons(polygons) {}
  
  void checkItself() const override {
    printf("Model %s\n", name.c_str());
    ref.checkItself(); 
    rotation.checkItself(); 
    printf("}\n");
  }

  string interface() const override {
    return (ostringstream()
      << " " << name 
      << " | Position: " << ref.interface()  
      << " | Angle: " << rotation.interface()).str();
  }
};
