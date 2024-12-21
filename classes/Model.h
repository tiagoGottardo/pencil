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

  vector<Line> getLines2() const {
    vector<Line> lines;  

    size_t numThreads = size_t(8); 
    size_t numPolygons = polygons.size();
    size_t chunkSize = numPolygons / numThreads;

    vector<vector<Line>> threadResults(numThreads);
    vector<std::thread> threads;

    std::mutex linesMutex;

    for (size_t t = 0; t < numThreads; ++t) {
      size_t start = t * chunkSize;
      size_t end = (t == numThreads - 1) ? numPolygons : start + chunkSize;

      threads.emplace_back([&, start, end, t]() {
        vector<Line> localLines;

        for (size_t i = start; i < end; ++i) {
          vector<Line> polygonLines = polygons[i].getLines(transformationMatrix());
          localLines.insert(localLines.end(), polygonLines.begin(), polygonLines.end());
        }

        {
          std::lock_guard<std::mutex> guard(linesMutex);
          threadResults[t] = std::move(localLines);  
        }
      });
    }

    for (auto& t : threads) if (t.joinable()) t.join();

    for (const auto& threadResult : threadResults) lines.insert(lines.end(), threadResult.begin(), threadResult.end());

    return lines; 
  }

  vector<Line> getLines() const override { 
    vector<Line> lines;

    for(Polygon polygon : polygons) {
      vector<Line> polygonLines = polygon.getLines(transformationMatrix());

      for(Line line : polygonLines) lines.push_back(line);
    }

    return lines; 
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
