#pragma once
using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include <type_traits>
#include <stdexcept>
#include <vector>
#include <cmath>

using sizet = size_t;

class Matrix {
private:
  double** matrix;
  sizet width, height;

public:
  typedef struct {
    sizet x;
    sizet y;
    double value;
  } PointInitializer;

  ~Matrix() {
    for(sizet i = 0; i < height; ++i) 
      free(matrix[i]);  
    free(matrix); 
  }

  Matrix(const Matrix& other) {
    width = other.width;
    height = other.height;

    matrix = (double**) calloc(height, sizeof(double*));

    for (sizet i = 0; i < height; ++i) {
      matrix[i] = (double*) calloc(width, sizeof(double));
      for (sizet j = 0; j < width; ++j) 
        matrix[i][j] = other.matrix[i][j];
    }
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

  Matrix(int w, int h) {
    width = w;
    height = h;

    matrix = (double **) calloc(height, sizeof(double*));

    for(sizet i = 0; i < height; i++)
      matrix[i] = (double*) calloc(width, sizeof(double));
  }

  Matrix(const vector<vector<double>>& input) {
    if(input.size() == 0) return; 

    width = input[0].size();
    height = input.size();

    matrix = (double **) calloc(height, sizeof(double*));

    for(sizet i = 0; i < height; i++)
      matrix[i] = (double*) calloc(width, sizeof(double));

    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        matrix[i][j] = input[i][j];
  }

  Matrix& operator+=(const Matrix& other) { *this = *this + other; return *this; }

  Matrix operator+(const Matrix& other) const {
    if(height != other.height || width != other.width)
      throw invalid_argument("Matrix dimensions must match for addition.");

    vector<vector<double>> result(height, vector<double>(width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        result[i][j] = matrix[i][j] + other[i][j];   

    return Matrix(result);
  }

  Matrix& operator-=(const Matrix& other) { *this = *this - other; return *this; }

  Matrix operator-(const Matrix& other) const { return *this + (-other); }

  Matrix operator-() const {
    vector<vector<double>> negated(height, vector<double>(width, 0));

    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        negated[i][j] = -matrix[i][j];

    return Matrix(negated);
  }

  Matrix& operator=(const Matrix& other) {
    if (this == &other) return *this;

    for(sizet i = 0; i < height; i++)
      delete[] matrix[i];
    delete[] matrix;

    height = other.height;
    width = other.width;

    matrix = new double*[height];
    for(sizet i = 0; i < height; i++) {
      matrix[i] = new double[width];
      for(sizet j = 0; j < width; j++)
        matrix[i][j] = other[i][j];
    }
    
    return *this;
  }

  bool operator==(const Matrix& other) const {
    if (height != other.height || width != other.width) return false;

    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        if(!Matrix::areNearlyEqual(matrix[i][j], other[i][j])) return false;

    return true;
  }

  Matrix operator*(const Matrix& other) const {
    if(width != other.height) 
      throw invalid_argument("Matrix dimensions must match for multiplication.");
    
    vector<vector<double>> result(height, vector<double>(other.width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < other.width; j++) 
        for(sizet k = 0; k < width; k++) 
          result[i][j] += matrix[i][k] * other[k][j];   

    return Matrix(result);
  }
  Matrix& operator*=(const Matrix& other) { *this = *this * other; return *this; }

  Matrix operator*(const double& value) const {
    vector<vector<double>> result(height, vector<double>(width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        result[i][j] = matrix[i][j] * value;   

    return Matrix(result);
  }

  bool operator!=(const Matrix& other) const { return !(*this == other); }

  void checkItself() const {
      printf("Matrix: {\n");
      for(sizet i = 0; i < height; i++) {
        printf("  ");
        for(sizet j = 0; j < width; j++) 
            printf("%.2f ", matrix[i][j]);
        printf("\n");
      }
      printf("}\n");
    }

    class Row {
    public:
      Row(double* row, sizet width) : row(row), width(width) {}

      double& operator[](sizet col) {
        if (col >= width) throw out_of_range("Column index out of range");
        return row[col];
      }

      const double& operator[](size_t col) const {
        if (col >= width) throw out_of_range("Column index out of range");
        return row[col];
      }

    private:
      double* row;
      sizet width;
    };

    Row operator[](sizet row) {
      if (row >= height) throw out_of_range("Row index out of range");
      return Row(matrix[row], width);
    }

    const Row operator[](sizet row) const {
      if (row >= height) throw out_of_range("Row index out of range");
    return Row(matrix[row], width);
  }

  int getWidth() const { return (int) width; }
  int getHeight() const { return (int) height; }
};
