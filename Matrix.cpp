#include "Matrix.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

Matrix::Matrix(int x, int y)
  :xSize(x),ySize(y)
{
  mat=new float*[x];
  for (int c=0;c<x;++c){
    mat[c]=new float[y];
  }
};

float& Matrix::getVal(int x,int y)
{
  //check if cell valid
  if (x>=xSize || y>=ySize || x<0 || y<0)
    throw invalid_argument("matrix out of bounds");

  return mat[x][y];
}

 std::ostream& operator<<(std::ostream& os, Matrix const & m)
 {
   cout<<endl;
   for (int j=0;j<m.ySize;++j){
     cout<<'|';
     for (int i=0;i<m.xSize;++i){
       std::cout << std::setw(4);
       cout<< m.mat[i][j] <<"  ";
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
