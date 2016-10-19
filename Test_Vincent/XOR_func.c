# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
//# include "ReadAndWrite.h"

void initWeight(float* mat, int y, int x)
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

void sumWeight (float* res, float* M, float* coefInput, int y, int x)
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
		U[i] = V[i] - R[i]; 
	}
}

void findGradient(float* M, float* V, float* W, int y, int x)
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
/*

		BACKWARD PROPAGATION

*/
void hActValue(float* U, float* V, float* M, int y, int x)
{
	for (int i = 0 ; i < x-1 ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j)
		{
			U[lineoffset] = V[lineoffset+j] * M[j];
		}
	}
}

void partDeri(float* U, float* V, int x)
{
	for (int i = 0 ; i < x-1 ; ++i) 
	{
		V[i] = U[i+1] * (1-pow(tanh(V[i]),2));
	}
}

void changeXH (float* M, float* U, float* V, int y, int x)
{
	for (int i = 0 ; i < x ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j)
		{
			M[lineoffset+j] = U[i] * V[j];
		}
	}
}

void changeWeight(float* M, float* N, int y, int x, int learningrate)
{
	for (int i = 0 ; i < x+1 ; ++i)
	{
		int lineoffset = i * x; 
		for (int j = 0 ; j < y ; ++j)
		{
			M[lineoffset+j] -= learningrate * N[lineoffset+j]; 
		}
	}
}
/*

		END BACKPROP

*/

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
	float inputs[] = { 1, 1, 0, 0, 0, 1, 1, 0 };
	//Outputs matix
	int lo = 1;
	int wo = 4; 
	float outputs[] = { 0, 0, 1, 1 };
	
	float learnRate = 0.01;
	int nbIter = 10000;
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
	
	for(int t = 0 ; t < 10001 ; ++t)
	{
		x[2] = 1; 
		
		x[0] = inputs[2*(t%4)];
		x[1] = inputs[2*(t%4)+1];
		
		y[0] = outputs[t%4];
		
		sumWeight(h, x, coefInput, lco, wco);
		sumWeight(WeightSum, hActVal, coefOutput, nbOutput, nbHidden);
		probAnswer(outActVal, WeightSum, nbOutput);
		outPutError(outError, outActVal, y, nbOutput);
		findGradient(dhAdjust, hActVal, outError, nbHidden+1, nbOutput);
		hActValue(hActVal, coefOutput, outError, nbHidden, nbOutput);
		partDeri(hActVal, WeightSum, nbHidden);
		changeXH(dxAdjust, x, h, nbInput, nbHidden);
		changeWeight( coefOutput, dhAdjust, nbHidden, nbOutput, learnRate);	
		changeWeight( coefInput, dxAdjust, nbInput, nbHidden, learnRate);

	}

	printf("%f XOR %f = %f\n", x[0], x[1], y[0]); 
//	printmatrix(coefInput,lci,wci);
//	printf("\n");
//	printmatrix(coefOutput,lco,wco);
//	printf("\n");
//	printmatrix(x,nbOutput+1,1);
//	printf("\n");	
	printf("%f|",outActVal[0]);
//	printf("%f|",outActVal[1]);	
	printf("\n");
	/* TRAINING LOOP */

//		printmatrix(x, 1, 3);		
	return 0;
}
