#include "Matrix.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

Matrix::Matrix(int x, int y)
  :xSize(x),ySize(y)
{
  mat=new float[x*y];
};

Matrix::~Matrix()
{
  delete[] mat;
}

float& Matrix::getVal(int x,int y)
{
  //check if cell valid
  if (x>=xSize || y>=ySize || x<0 || y<0)
    throw invalid_argument("matrix out of bounds");

  return mat[ x*ySize + y];
}

 std::ostream& operator<<(std::ostream& os, Matrix const & m)
 {
   cout<<endl;
   for (int j=0;j<m.ySize;++j){
     cout<<'|';
     for (int i=0;i<m.xSize;++i){
       std::cout << std::setw(4);
       cout<< m.mat[i*m.ySize + j] <<"  ";
     }
     cout<<'|';
     cout<<endl;
   }

   return os;
 }

 int Matrix::getXSize()
 {
	 return xSize;
 }
 int Matrix::getYSize()
 {
	 return ySize;
 }
