//# include "ReadAndWrite.h"
# include <math.h>
# include <assert.h>
# include <stdio.h>
# include <stdlib.h>

/* 
		SIGMOID FUNCTION : ACTIVATION FUNCTIONS	
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

float* sigmoid(float* weightMatrix, int r, int l)
{
	float* res = malloc(r*l* sizeof(float));
	double sum = sumExp(weightMatrix, r, l);
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = j * r ;
		for (int i = 0 ; i < r ; ++i)
		{
			res[lineoffset+i] = 1 / (1+exp(-weightMatrix[lineoffset+i]));
		}
	}

	return res;
}

float* derivative_sigmoid(float* weightMatrix, int r, int l)
{
	float* res = malloc(r*l* sizeof(float));
	double sum = sumExp(weightMatrix, r, l);
	float sigmoid;
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = j * r ;
		for (int i = 0 ; i < r ; ++i)
		{
			sigmoid =  1 / (1-exp(-weightMatrix[lineoffset+i]));
			res[lineoffset+i] = sigmoid *(1.0-sigmoid);
		}
	}

	return res;
}
/*
		END SIGMOID
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
//	printf("Matrix %i x %i\n", r, L);
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
		//		printf("res[%i][%i]=%f\n",nOffset, i, res[nOffset+i]);
		}
	}
	return res;
}

void swap (float* a, float* b)
{
	int c;
	c = *a; 
	*a = *b;
	*b = c;
}

void transposeMatrix(float* M, int r, int l)
{
	
	for (int j = 0 ; j < l ; ++j)
	{
		for (int i = 0; i < r ; ++i)
		{
				printf("swapping %f with %f \n", M[j*l+i] , M[i*r+j]);
				swap(&M[j*l+i], &M[i*r+j] );
		}
	}
}

void initWeight( float* weightMatrix ,int r, int l)
{
	
	for (int j = 0 ; j < l ; ++j)
		{
			int lineoffset = (r>1) ? j*l : j ;
			for (int i = 0 ; i < r ; ++i)
			{
				float rnd = ((float)rand()/(double)RAND_MAX);
				weightMatrix[lineoffset+i] = rnd;
				printf("Rand = %f in %d \n", rnd, lineoffset+i);
			}
		}
}

float computingError(float* expect, float* actual, int r, int l)
{
	float res = 0;
	for (size_t j = 0 ; j < l ; ++ j)
		{
			int lineoffset = (r>1) ? j*l : j;
			for (size_t i = 0 ; i < r ; ++ i)
			{
				float tmp = 0.5 * (expect[lineoffset+i] - actual[lineoffset+i])* (expect[lineoffset+i] - actual[lineoffset+i]);
				res += tmp ;
				printf("tmp = %f\n", tmp);
			}
		}
	return res;
}

/* DECLARATIONS OF MATRIX HERE: MATRIX; ROWS(Y); LINES(X); */
int main(int arc, char* argv)
{
/*	
	float inputs[] = {0,0,0,1,1,0,1,1};
	float outputs[] = { 0 , 1 , 1 , 0};
	int nbInputs = 2; 
	int nbOutputs = 1; 
	int nbHidden = 2;
	
	float* weight1= malloc(nbHidden * nbInputs * sizeof(float));
	initWeight(weight1, nbHidden, nbInputs);
	printf("Weight1: \n");
	printmatrix(weight1, nbHidden, nbInputs);
	float* weight2= malloc(nbOutputs * nbHidden * sizeof(float));

	initWeight(weight2, nbOutputs, nbHidden);
	printf("weight2 : \n");
	printmatrix(weight2, nbOutputs, nbHidden);
	printf("Input matrix:\n");
	printmatrix(inputs, 2, 4);
	printf("Coefficients from input to hidden1:\n");
	printmatrix(weight1, nbHidden, nbInputs);
	printf("Current weights from input to hidden1 \n");
	float* W1 = matProduct(inputs, 2, 4, weight1, 2, 2);
//	printf("\n");
	printmatrix(W1, 2, 4);
	printf("Activation of weights needed:\n");
	float* r1 = sigmoid(W1, 2, 4);
	printmatrix(r1, 2, 4);
	printf("Current weight from h1 to output: \n");
	float* W2 = matProduct(r1, 2, 4, weight2, 1, 2);
	printmatrix(W2, 1,4);
	printf("Activation of output \n");
	float* r2 = sigmoid(W2, 1, 4);
	printmatrix(r2, 1, 4);
	float error = computingError(r2, outputs, 1, 4);
	printf("Error = %f \n", error);
*/
	float test[] = {1,2,3,4,5,6};
	printmatrix(test, 3, 2);
	printf("\n");
	transposeMatrix( test, 3, 2 );
	printmatrix(test, 2, 3);
	
	return 0;
}
