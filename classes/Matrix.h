#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include <type_traits>
#include <stdexcept>
#include <vector>
#include <cmath>

using sizet = std::size_t;

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

  static bool areNearlyEqual(double a, double b, double epsilon = 1e-2) { return std::fabs(a - b) < epsilon; }

  Matrix static IdentityMatrix(int size){
    std::vector<PointInitializer> points;
    for(sizet i = 0; i < (sizet) size; i++)
      points.push_back({i, i, 1});
    
    return Matrix(size, size, points);
  }

  Matrix(sizet w, sizet h) : Matrix(w, h, {}) {}

  Matrix(int w, int h, std::vector<PointInitializer> points) {
    width = w;
    height = h;

    matrix = (double **) calloc(height, sizeof(double*));

    for(sizet i = 0; i < height; i++)
      matrix[i] = (double*) calloc(width, sizeof(double));

    for(PointInitializer point : points)
      matrix[point.y][point.x] = point.value;
  }

  Matrix(const std::vector<std::vector<double>>& input) {
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
      throw std::invalid_argument("Matrix dimensions must match for addition.");

    std::vector<std::vector<double>> result(height, std::vector<double>(width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        result[i][j] = matrix[i][j] + other[i][j];   

    return Matrix(result);
  }

  Matrix& operator-=(const Matrix& other) { *this = *this - other; return *this; }

  Matrix operator-(const Matrix& other) const { return *this + (-other); }

  Matrix operator-() const {
    std::vector<std::vector<double>> negated(height, std::vector<double>(width, 0));

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
      throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    
    std::vector<std::vector<double>> result(height, std::vector<double>(other.width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < other.width; j++) 
        for(sizet k = 0; k < width; k++) 
          result[i][j] += matrix[i][k] * other[k][j];   

    return Matrix(result);
  }
  Matrix& operator*=(const Matrix& other) { *this = *this * other; return *this; }

  Matrix operator*(const double& value) const {
    std::vector<std::vector<double>> result(height, std::vector<double>(width, 0));
    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        result[i][j] = matrix[i][j] * value;   

    return Matrix(result);
  }

  bool operator!=(const Matrix& other) const { return !(*this == other); }

  double determinant() { 
    if(width != height) throw std::invalid_argument("Matrix need to be squared for this operation.");
    if(width == 1) return matrix[0][0];
    sizet size = width;
    double result = 0;

    Matrix iterator = Matrix(size - 1, size - 1);
    for(sizet i = 0; i < size; i++) {
      for(sizet j = 1; j < size; j++) 
        for(sizet k = 0; k < size; k++) {
          if(k == i) continue; 
          iterator[j - 1][(k < i) ? k : k - 1] = matrix[j][k];
        }
      
      result += matrix[0][i] * iterator.determinant() * pow(-1, i);
    }

    return result; 
  } 

  Matrix adjoint() {
    if(width != height) throw std::invalid_argument("Matrix need to be squared for this operation.");

    sizet size = width;
    Matrix adjoint = Matrix(size, size);

    Matrix iterator = Matrix(size - 1, size - 1);
    for(sizet i = 0; i < size; i++)
      for(sizet j = 0; j < size; j++) {
        for(sizet il = 0; il < size; il++) {
          if (il == i) continue;
          for(sizet jl = 0; jl < size; jl++) {
            if (jl == j) continue;
            iterator[(il > i) ? il - 1 : il][(jl > j) ? jl - 1 : jl] = matrix[il][jl];
          }
        }
        adjoint[j][i] = pow(-1, i + j) * iterator.determinant();
      }

    return adjoint;
  }

  Matrix operator!() {
    if(width != height) throw std::invalid_argument("Matrix need to be squared to invert it.");
    double factor = this->determinant(); 
    Matrix result = this->adjoint();

    for(sizet i = 0; i < height; i++)
      for(sizet j = 0; j < width; j++)
        result[i][j] /= factor; 

    return result; 
  }

  void checkItself() const {
    for(sizet i = 0; i < height; i++) {
      for(sizet j = 0; j < width; j++) 
          printf("%.2f ", matrix[i][j]);
      printf("\n");
    }
    printf("\n");
  }

  class Row {
  public:
    Row(double* row, sizet width) : row(row), width(width) {}

    double& operator[](sizet col) {
      if (col >= width) throw std::out_of_range("Column index out of range");
      return row[col];
    }

    const double& operator[](std::size_t col) const {
      if (col >= width) throw std::out_of_range("Column index out of range");
      return row[col];
    }

  private:
    double* row;
    sizet width;
  };

  Row operator[](sizet row) {
    if (row >= height) throw std::out_of_range("Row index out of range");
    return Row(matrix[row], width);
  }

  const Row operator[](sizet row) const {
    if (row >= height) throw std::out_of_range("Row index out of range");
    return Row(matrix[row], width);
  }

  int getWidth() const { return (int) width; }
  int getHeight() const { return (int) height; }
};

#endif
