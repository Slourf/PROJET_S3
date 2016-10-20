//# include "ReadAndWrite.h"
# include <math.h>
# include <assert.h>
# include <stdio.h>
# include <stdlib.h>

/* 
		SOFTMAX FUNCTION : ACTIVATION FUNCTIONS	
*/
double sumExp(float* weightMatrix, int r, int l)
{
	double res = 0 ; 
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = j * r;
		for (int i = 0 ; i < r ; ++i)
		{
			res+=(exp(weightMatrix[lineoffset+i]));
		}
	}
	return res;
}

float* softmax(float* weightMatrix, int r, int l)
{
	float* res = malloc(r*l* sizeof(float));
	double sum = sumExp(weightMatrix, r, l);
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = j * r ;
		for (int i = 0 ; i < r ; ++i)
		{
			res[lineoffset+i] = exp(weightMatrix[lineoffset+i]) / sum;
		}
	}

	return res;
}
/*
		END SOFTMAX
*/
void printmatrix(float* M, int r, int l)
{
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = j*r; 
		for (int i = 0 ; i < r ; ++i)
		{
			printf(" %f |", M[lineoffset+i]);
		}
		printf("\n");
	}
}

float* matProduct(float* M, int R, int L, float* m, int r, int l)
{
	float* res = calloc(L*r ,sizeof(float));
	printf("Matrix %i x %i\n", r, L);
	for (int j = 0 ; j < L ; ++j)
	{
		int lineoffset = j * R;
		int nOffset = j*r;
		for (int i = 0 ; i < r ; ++i)
		{
			for (int k = 0 ; k < l ; ++k)
			{
				res[nOffset+i] += M[lineoffset+k] * m[k*r+i];
			}
				printf("res[%i][%i]=%f\n",nOffset, i, res[nOffset+i]);
		}
	}
	return res;
}

void initWeight( float* weightMatrix ,int r, int l)
{
	for (int j = 0 ; j < l ; ++j)
		{
			int lineoffset = j * l;
			for (int i = 0 ; i < r ; ++i)
			{
				weightMatrix[lineoffset+i] = ((float)rand()/(double)RAND_MAX);
			}
		}
}

int main(int arc, char* argv)
{
	
	float inputs[] = {0,0,0,1,1,0,1,1};
	float outpus[] = { 0 , 1 , 1 , 0};
	int nbInputs = 2; 
	int nbOutputs = 1; 
	int nbHidden = 2;
	
	float* weight1= malloc(nbHidden*nbInputs * sizeof(float));
	initWeight(weight1, nbHidden, nbInputs);

	float* weight2= malloc(nbHidden * sizeof(float));

	initWeight(weight2, nbHidden, 1);
	printmatrix(inputs, 2, 4);
	printf("\n");
	printmatrix(weight1, nbHidden, nbInputs);
	printf("\n");
	float* W1 = matProduct(inputs, 2, 4, weight1, 2, 2);
	printf("\n");
	printmatrix(W1, 2, 4);
	printf("\n");
	float* r1 = softmax(W1, 2, 4);
	printmatrix(r1, 2, 4);
	printf("\n");
	float* W2 = matProduct(r1, 2, 4, weight2, 1, 2);
	printmatrix(W2, 1,4);
	printf("\n");
	float* r2 = softmax(W2, 1, 4);
	printmatrix(r2, 1, 4);
/*	
	float test1[] = { 2,5,3,6,4,7 };
	float test2[] = { 2,5, 4,6};
	float* test = matProduct(test1, 2, 3, test2, 2, 2);
	printmatrix(test, 2, 3);
*/

	return 0;
}
