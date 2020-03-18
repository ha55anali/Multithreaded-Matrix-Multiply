#include "Matrix.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

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

  if (NoThread > A.getYSize())
    NoThread=A.getYSize();

  Matrix* Out=new Matrix(B.getXSize(),A.getYSize());

  //get row dimensions
  int* yDivisions=new int[NoThread+1];
  int yDiv=Out->getYSize()/NoThread;
  int yDivCount=1;
  yDivisions[0]=0;
  while (yDiv*yDivCount <= Out->getYSize())
    {
      yDivisions[yDivCount]=yDiv*yDivCount;
      ++yDivCount;
    }
  yDivisions[yDivCount-1]=Out->getYSize();

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

    try{
      pthread_create(&ptid[c],NULL,matMul,&m[c]);
    }
    catch(invalid_argument& e)
      {
        delete[] ptid;
        delete[] yDivisions;
        delete[] m;
        delete Out;
        throw e;
      }
    }

  void* a;
  for (int c=0;c<NoThread;++c)
    pthread_join(ptid[c],&a);

  delete[] ptid;
  delete[] yDivisions;
  delete[] m;

  return Out;
}

void getParameters(int argc, char* argv[], char*& MAFile, char*& MBFile, int& ThreadCount)
{
	int opt;

	while((opt = getopt(argc, argv, "a:b:t:")) != -1)
	{
		switch(opt)
		{
		case 'a':
			MAFile = new char[strlen(optarg) + 1];
			strcpy(MAFile, optarg);
			break;
		case 'b':
			MBFile = new char[strlen(optarg) + 1];
			strcpy(MBFile, optarg);
			break;
		case 't':
			ThreadCount = atoi(optarg);
			break;
		}
	}

	return;
}

Matrix* ReadMatrixFile(char* file)
{
  int f = open(file, O_RDONLY);

  if (f<0)
    throw invalid_argument("cannot open file");

  const int buffS=1000;
  char* buff=new char[buffS];
  char* buffTok=buff;

  read(f,buff, buffS);
  //tokenize on new line
  char* line=strtok_r(buff,"\n",&buffTok);

  //get matrix size
  int xSize=0;
  int ySize=0;
  char* token=strtok(line," ");
  ySize=atof(token);
  token=strtok(NULL," ");
  xSize=atof(token);
  Matrix* M=new Matrix(xSize,ySize);

  line=strtok_r(NULL,"\n",&buffTok);
  int xCount=0;
  int yCount=0;
  while (line != NULL)
    {
      if (line[0] != '#')
      {
        try
          {
          M->getVal(xCount,yCount)=atof(line);
          }
        catch(invalid_argument& e)
          {
            delete M;
            delete[] buff;
            delete[] line;
            throw e;
          }

        ++xCount;
        if (xCount == xSize)
          {
            xCount=0;
            ++yCount;
          }
        
      }

      line=strtok_r(NULL,"\n",&buffTok);
    }

  delete[] buff;
  delete[] line;
  return M;
}

int main(int argc, char* argv[])
{
  char* MA=NULL,* MB=NULL;
  int ThreadCount=1;
  getParameters(argc,argv,MA,MB,ThreadCount);

  if (MA == NULL | MB == NULL | ThreadCount <= 0)
  {
    cout<<"invalid arguments";
    return 1;
  }

  try{
    Matrix* A=ReadMatrixFile(MA);
    Matrix* B=ReadMatrixFile(MB);

    cout<<*A<<*B;

    Matrix* Z=Multiply(*A,*B,ThreadCount);

    cout<<*Z;

    delete A;
    delete B;
    delete Z;
  }
  catch(invalid_argument& e)
    {
      delete MA;
      delete MB;
      cout<< e.what();
    }

  delete MA;
  delete MB;
}
