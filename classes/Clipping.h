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
  LineStatus lineStatus;
  Line line;
} ClipResult;

class Clipping {
private:
  static double maxi(double arr[],int n) {
    double m = 0;
    for (int i = 0; i < n; ++i)
      if (m < arr[i])
        m = arr[i];
    return m;
  }

  static double mini(double arr[], int n) {
    double m = 1;
    for (int i = 0; i < n; ++i)
      if (m > arr[i])
        m = arr[i];
    return m;
  }

  static ClipResult liangClipper(double x1, double y1, double x2, double y2) {
    double xmin = -1;
    double xmax =  1;
    double ymin = -1;
    double ymax =  1;

    double p1 = -(x2 - x1);
    double p2 = -p1;
    double p3 = -(y2 - y1);
    double p4 = -p3;

    double q1 = x1 - xmin;
    double q2 = xmax - x1;
    double q3 = y1 - ymin;
    double q4 = ymax - y1;

    double posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) 
      return { LineStatus::OUTSIDE, Line() };

    if (p1 != 0) {
      double r1 = q1 / p1;
      double r2 = q2 / p2;
      if (p1 < 0) {
        negarr[negind++] = r1; 
        posarr[posind++] = r2;
      } else {
        negarr[negind++] = r2;
        posarr[posind++] = r1;
      }
    }
    if (p3 != 0) {
      double r3 = q3 / p3;
      double r4 = q4 / p4;
      if (p3 < 0) {
        negarr[negind++] = r3;
        posarr[posind++] = r4;
      } else {
        negarr[negind++] = r4;
        posarr[posind++] = r3;
      }
    }

    double xn1, yn1, xn2, yn2;
    double rn1, rn2;
    rn1 = Clipping::maxi(negarr, negind); 
    rn2 = Clipping::mini(posarr, posind); 
    
    if (rn1 > rn2) return { LineStatus::OUTSIDE, Line() };

    xn1 = x1 + p2 * rn1;
    yn1 = y1 + p4 * rn1; 

    xn2 = x1 + p2 * rn2;
    yn2 = y1 + p4 * rn2;

    return { LineStatus::CLIPPLED, Line(Point(xn1, yn1), Point(xn2, yn2)) };
  }

public:
  static void execute(vector<Line>* lines) {

    for (auto it = lines->begin(); it != lines->end(); ) {
      ClipResult result = liangClipper(it->a.x, it->a.y, it->b.x, it->b.y);

      bool behindWindow = it->a.z > 1. || it->b.z > 1.;

      if (result.lineStatus == LineStatus::OUTSIDE || behindWindow)
        it = lines->erase(it); 
      else 
        { *it = result.line; ++it; }
    }
  }

  static void executeParallel(vector<Line>* lines) {
    size_t numThreads = thread::hardware_concurrency() / 2;
    size_t n = lines->size();
    size_t chunkSize = n / numThreads;
    vector<future<void>> futures;

    auto worker = [&](size_t start, size_t end) {
      for (size_t i = start; i < end; ++i) {
        ClipResult result = liangClipper((*lines)[i].a.x, (*lines)[i].a.y, (*lines)[i].b.x, (*lines)[i].b.y);

        bool behindWindow = (*lines)[i].a.z > 1. || (*lines)[i].b.z > 1.;

        if (result.lineStatus == LineStatus::OUTSIDE || behindWindow)
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
      return line.a == line.b && line.a == Point();
    }), lines->end());
  }
};
