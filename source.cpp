#include "Matrix.h"
#include <iostream>

using namespace std;

struct MulParam
{
	Matrix* Out;
	Matrix* A;
	Matrix* B;

	int startX;
	int endX;

	int startY;
	int endY;
};

float getCell(Matrix A, Matrix B, int x, int y)
{
  float a=0;
  for (int c=0;c< A.getXSize(); ++c){
    a+=A.getVal(c,y)*B.getVal(x,c);
  }

  return a;
}

void* matMul(void* param)
{
  MulParam* P=(MulParam*) param;

  for (int i=P->startX ; i<P->endX; ++i)
    {
      for (int j=P->startY; j<P->endY; ++j){
        P->Out->getVal(i,j)=getCell(*(P->A),*(P->B),i,j);
      }
    }

  return NULL;
}

int main()
{
}
