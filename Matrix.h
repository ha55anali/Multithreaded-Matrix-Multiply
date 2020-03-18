#pragma once

#include <iostream>

class Matrix
{
public:
  //constructor
	Matrix(int x, int y);
  ~Matrix();

  //get value of cell by reference
	float& getVal(int x, int y);
  int getXSize();
  int getYSize();

  friend std::ostream& operator<<(std::ostream& os, Matrix const & m);
private:
	float* mat;
	int xSize;
	int ySize;
};
