#pragma once

using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include <type_traits>
#include <stdexcept>
#include <vector>
#include <cmath>

#define MATRIX_SIZE 4

class Matrix {
private:
  double matrix[MATRIX_SIZE][MATRIX_SIZE];

public:
  Matrix() {
    for (size_t i = 0; i < MATRIX_SIZE; ++i) 
      for (size_t j = 0; j < MATRIX_SIZE; ++j) 
        matrix[i][j] = 0; 
  }

  Matrix(const Matrix& other) {
    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        matrix[i][j] = other[i][j];
  }

  Matrix(const vector<vector<double>>& input) {
    if(input.size() == 0) return; 

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        matrix[i][j] = input[i][j];
  }

  static bool areNearlyEqual(double a, double b, double epsilon = 1e-2) { return fabs(a - b) < epsilon; }

  Matrix static ScaleMatrix(double x = 1, double y = 1, double z = 1) {
    return Matrix({
      {x, 0, 0, 0},
      {0, y, 0, 0},
      {0, 0, z, 0},
      {0, 0, 0, 1}
    });
  }

  Matrix static XRotationMatrix(double theta_degree) {
    double theta_radian = theta_degree * M_PI / 180.0;
    return Matrix({
      {1,                 0,                  0, 0},
      {0, cos(theta_radian), -sin(theta_radian), 0},
      {0, sin(theta_radian),  cos(theta_radian), 0},
      {0,                 0,                  0, 1}
    });
  }

  Matrix static YRotationMatrix(double theta_degree) {
    double theta_radian = theta_degree * M_PI / 180.0;
    return Matrix({
      { cos(theta_radian), 0, sin(theta_radian), 0},
      {                 0, 1,                 0, 0},
      {-sin(theta_radian), 0, cos(theta_radian), 0},
      {                 0, 0,                  0, 1}
    });
  }

  Matrix static ZRotationMatrix(double theta_degree) {
    double theta_radian = theta_degree * M_PI / 180.0;
    return Matrix({
      {cos(theta_radian), -sin(theta_radian), 0, 0},
      {sin(theta_radian),  cos(theta_radian), 0, 0},
      {                0,                  0, 1, 0},
      {                0,                  0, 0, 1} 
    });
  }

  Matrix static TranslationMatrix(double x = 0, double y = 0, double z = 0) {
    return Matrix({
      {1, 0, 0, x},
      {0, 1, 0, y},
      {0, 0, 1, z},
      {0, 0, 0, 1},
    });
  }

  Matrix static PerspectiveMatrix(double fov = 45.0, double aspect = 1331./761., double near = 0.1, double far = 1300) {
    double r = 1.0 / tan(fov * 0.5 * M_PI / 180.0);  // Convert FOV to radians
    double t = r / aspect;  // Top bound derived from aspect ratio
    
    return Matrix({
      {r,   0,                             0,                                0},
      {0,   t,                             0,                                0},
      {0,   0,  -(far + near) / (far - near), -(2 * far * near) / (far - near)},
      {0,   0,                            -1,                                0},
    });
  }

  void checkItself() const {
    printf("Matrix: {\n");
    for(size_t i = 0; i < MATRIX_SIZE; i++) {
      printf("  ");
      for(size_t j = 0; j < MATRIX_SIZE; j++) 
        printf("%.2f ", matrix[i][j]);
      printf("\n");
    }
    printf("}\n");
  }

  Matrix& operator+=(const Matrix& other) { *this = *this + other; return *this; }

  Matrix operator+(const Matrix& other) const {
    Matrix result;

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++) 
        result[i][j] = matrix[i][j] + other[i][j];   

    return result;
  }

  Matrix& operator=(const Matrix& other) {
    if (this == &other) return *this;
    
    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        matrix[i][j] = other[i][j];
    
    return *this;
  }

  bool operator==(const Matrix& other) const {
    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        if(!Matrix::areNearlyEqual(matrix[i][j], other[i][j])) return false;

    return true;
  }

  Matrix operator*(const Matrix& other) const {
    Matrix result;

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++) 
        for(size_t k = 0; k < MATRIX_SIZE; k++) 
          result[i][j] += matrix[i][k] * other[k][j];   

    return result;
  }

  Matrix& operator*=(const Matrix& other) { *this = *this * other; return *this; }

  Matrix operator*(const double& value) const {
    Matrix result;

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++)
        result[i][j] = matrix[i][j] * value;   

    return result;
  }

  bool operator!=(const Matrix& other) const { return !(*this == other); }

  double* operator[](size_t row) {
    if (row >= MATRIX_SIZE) throw out_of_range("Row index out of range");
    return matrix[row];
  }

  const double* operator[](size_t row) const {
    if (row >= MATRIX_SIZE) throw out_of_range("Row index out of range");
    return matrix[row];
  }
};
