#include "Matrix.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
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

Matrix* Multiply(Matrix A, Matrix B, int NoThread)
{
  if (A.getXSize() != B.getYSize())
    throw invalid_argument("invalid dimensions");

  Matrix* Out=new Matrix(A.getYSize(),B.getXSize());

  //get row dimensions
  int* yDivisions=new int[NoThread+1];
  int yDiv=Out->getYSize()/NoThread;
  int yDivCount=1;
  yDivisions[0]=0;
  while (yDiv*yDivCount < Out->getYSize())
    {
      yDivisions[yDivCount]=yDiv;
      ++yDivCount;
    }
  yDivisions[yDivCount]=Out->getYSize();

  pthread_t* ptid=new pthread_t[NoThread];
  MulParam* m=new MulParam[NoThread];
  for (int c=0;c<NoThread;++c)
    {
    ptid[c]=ftok("source.cpp",c);

    m[c].Out=Out;
    m[c].A=&A;
    m[c].B=&B;
    m[c].startX=0;
    m[c].endX=Out->getXSize();
    m[c].startY=yDivisions[c];
    m[c].endY=yDivisions[c+1];

    pthread_create(&ptid[c],NULL,matMul,&m[c]);
    }

  void* a;
  for (int c=0;c<NoThread;++c)
    pthread_join(ptid[c],&a);

  return Out;
}

int main()
{
}
