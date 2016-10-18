# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
//# include "ReadAndWrite.h"

void initWeight(float* mat, int x, int y)
{
	for (int i = 0; i < x; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0; j < y; ++j)
		{
			mat[lineoffset+j] = ((float)rand()/(double)RAND_MAX)*0.2-0.1;
		}
	}
}

//Just using it to debug
void printmatrix(float* mat, int x, int y)
{
	for (int i = 0; i < x; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0; j < y; ++j)
		{
			printf("| %f", mat[lineoffset+j]);
		}
		printf("\n");
	}
}

void sumWeight (float* M, float* res, float* coefInput, int x, int y)
{
	int sum = 0;
	for (int i = 0 ; i < x; ++i)
	{
		int lineoffset = i*x; 
		for (int j = 0; j < y ; ++j)
		{
			sum+=coefInput[lineoffset+j]*M[lineoffset+j];
		}
		res[i] = sum;
		sum = 0;
	}
}

float ActFunc(float f)
{
	return tanh(f);
}

void actValue (float* M, float* res, int x)
{
	M[x-1] = 1 ; 
	for (int i = 0 ; i < x-1 ; ++i)
			{
				res[i] = ActFunc(M[i]);
			}
}

void probAnswer(float* V, float* U, int l)
{
	int sum = 0; 
	for (int i = 0 ; i < l ; ++i)
	{
		V[i] = exp(U[i]);
		sum += V[i] ;
	}
	for (int i = 0 ; i < l ; ++i)
	{
		V[i] /= sum;
	}
}

void outPutError(float* U, float* V, float* R, int l)
{
	for (int i = 0 ; i < l ; ++i)
	{
		R[i] = U[i] - V[i]; 
	}
}

void findGradient(float* M, float* V, float* W, int x, int y)
{
	for (int i = 0 ; i < x ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j) 
		{
			M[lineoffset+j] = W[lineoffset] * V[j];
		}
	}
}

int main (int argc,char* argv)
{
	//Learning config
	int nbComb = 4;
	int nbInput = 2;
	int nbOutput = 1;
	int nbHidden = 2;
	//Inputs matrix
	int li = 4;
	int wi = 2; 
	float inputs[] = { 0, 0, 0, 1, 1, 1, 1, 0 };
	//Outputs matix
	int lo = 1;
	int wo = 4; 
	float outputs[] = { 0, 1, 0, 1 };
	
	float learnRate = 0.2;
	int nbIter = 250;
	//input weights
	int lci = nbInput+1;
	int wci = nbHidden;
	float* coefInput = calloc(lci*wci,sizeof(float));
	initWeight(coefInput, lci, wci);
	//hidden weights
	int lco = nbHidden+1;
	int wco = nbOutput;
	float* coefOutput = calloc(lco*wco,sizeof(float));
	initWeight(coefOutput, lco, wco);
	//Vectors with input/output/hidden 
	float* x = calloc(nbInput+1, sizeof(float));
	float* y = calloc(nbOutput, sizeof(float));
	float* h = calloc(nbHidden, sizeof(float));
	//Activation values and sums
	float* hActVal = calloc(nbHidden+1, sizeof(float));
	float* WeightSum = calloc(nbOutput, sizeof(float));
	float* outActVal = calloc(nbOutput, sizeof(float));
	
	float* outError = calloc(nbOutput, sizeof(float));
	//matrix
	float* dxAdjust = calloc((nbInput+1)*nbHidden, sizeof(float));
	float* dhAdjust = calloc((nbHidden+1)*nbOutput, sizeof(float));

	float loss, sum;
	return 0;
}
