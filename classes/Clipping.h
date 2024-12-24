#pragma once

#include <optional>
#include <stdio.h>
#include <thread>
#include <future>
#include <vector>
#include <algorithm>

#include "./Point.h"
#include "./Polygon.h"

enum LineStatus {
  CLIPPLED,
  INSIDE,
  OUTSIDE,
};

typedef struct {
  uint width, height;
} RectangleSize;

typedef struct {
  LineStatus lineStatus;
  Line line;
} ClipResult;

class Clipping {
private:
  static float maxi(float arr[],int n) {
    float m = 0;
    for (int i = 0; i < n; ++i)
      if (m < arr[i])
        m = arr[i];
    return m;
  }

  static float mini(float arr[], int n) {
    float m = 1;
    for (int i = 0; i < n; ++i)
      if (m > arr[i])
        m = arr[i];
    return m;
  }

  static ClipResult liangClipper(float xmin, float ymin, float xmax, float ymax, float x1, float y1, float x2, float y2) {
    float p1 = -(x2 - x1);
    float p2 = -p1;
    float p3 = -(y2 - y1);
    float p4 = -p3;

    float q1 = x1 - xmin;
    float q2 = xmax - x1;
    float q3 = y1 - ymin;
    float q4 = ymax - y1;

    float posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) 
      return { LineStatus::OUTSIDE, Line() };

    if (p1 != 0) {
      float r1 = q1 / p1;
      float r2 = q2 / p2;
      if (p1 < 0) {
        negarr[negind++] = r1; 
        posarr[posind++] = r2;
      } else {
        negarr[negind++] = r2;
        posarr[posind++] = r1;
      }
    }
    if (p3 != 0) {
      float r3 = q3 / p3;
      float r4 = q4 / p4;
      if (p3 < 0) {
        negarr[negind++] = r3;
        posarr[posind++] = r4;
      } else {
        negarr[negind++] = r4;
        posarr[posind++] = r3;
      }
    }

    float xn1, yn1, xn2, yn2;
    float rn1, rn2;
    rn1 = Clipping::maxi(negarr, negind); 
    rn2 = Clipping::mini(posarr, posind); 
    
    if (rn1 > rn2) return { LineStatus::OUTSIDE, Line() };

    xn1 = x1 + p2 * rn1;
    yn1 = y1 + p4 * rn1; 

    xn2 = x1 + p2 * rn2;
    yn2 = y1 + p4 * rn2;

    return { LineStatus::CLIPPLED, Line(Point(xn1, yn1), Point(xn2, yn2)) };
  }

  static Point maxPoint(RectangleSize windowSize) { return Point((int) windowSize.width / 2, (int) windowSize.height / 2); }
  static Point minPoint(RectangleSize windowSize) { return -Clipping::maxPoint(windowSize); }

public:
  static void execute(RectangleSize windowSize, vector<Line>* lines) {
    Point maxPoint = Clipping::maxPoint(windowSize);
    Point minPoint = Clipping::minPoint(windowSize);

    for (auto it = lines->begin(); it != lines->end(); ) {
      ClipResult result = liangClipper(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y, it->a.x, it->a.y, it->b.x, it->b.y);

      if (result.lineStatus == LineStatus::OUTSIDE) {
        it = lines->erase(it); 
      } else {
        *it = result.line; 
        ++it;
      }
    }
  }

  static void executeParallel(RectangleSize windowSize, vector<Line>* lines) {
    Point maxPoint = Clipping::maxPoint(windowSize);
    Point minPoint = Clipping::minPoint(windowSize);

    size_t numThreads = thread::hardware_concurrency() / 2;
    size_t n = lines->size();
    size_t chunkSize = n / numThreads;
    vector<future<void>> futures;

    auto worker = [&](size_t start, size_t end) {
      for (size_t i = start; i < end; ++i) {
        ClipResult result = liangClipper(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y, (*lines)[i].a.x, (*lines)[i].a.y, (*lines)[i].b.x, (*lines)[i].b.y);

        if (result.lineStatus == LineStatus::OUTSIDE)
          (*lines)[i] = Line();
         else 
          (*lines)[i] = result.line;
      }
    };

    for (size_t i = 0; i < numThreads; ++i) {
      size_t start = i * chunkSize;
      size_t end = (i == numThreads - 1) ? n : start + chunkSize;

      futures.push_back(std::async(std::launch::async, worker, start, end));
    }

    for (auto& fut : futures) fut.get();

    lines->erase(std::remove_if(lines->begin(), lines->end(), [](const Line& line) {
      return line.a == line.b;
    }), lines->end());
  }
};
