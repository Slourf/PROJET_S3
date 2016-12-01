# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>
# include "exploreFiles.h"

/*========== Read&Write matrix into file =========*/

/*----------Save a matrix into a file------------*/
void writeFile(char* path, float *a, size_t len)
{
		FILE* file = fopen(path, "w");
		if (file != NULL)
		{
				for (size_t i = 0; i < len; i++)
				{
						fprintf(file, "%f\n", *a);
						a++;
				}
				fclose(file);
		}
		else
				errx(1, "Invalid path");
}

/*---------Read a matrix from a file-----------*/
void readFile(char *path, float *b, size_t len)
{
		FILE* file = fopen(path, "r");
		if (file != NULL)
		{
				for (size_t i = 0; i < len; i++)
				{
						fscanf(file, "%f", (b+i));
						
				}
				fclose(file);
		}
		else
				errx(1, "Invalid path");
}

/*==============End Read&Write file=============*/


/*=============Forward propagation============*/


/*--------------Initialize weights--------------*/
void initWeight( float* weight ,int r, int l)
{
		for (int j = 0 ; j < l ; ++j)
		{
				int lineoffset = j*r ;
				for (int i = 0 ; i < r ; ++i)
				{
						float rnd = ((float)rand()/(double)RAND_MAX) * 0.2-0.1;
						weight[lineoffset+i] = rnd;
				}
		}
}

/*SIGMOID FUNCTION : ACTIVATION FUNCTION*/
float sigmoid(double z)
{
		return 1.0/(1.0 + exp(-z));
}

/*--------------layer matrix * weight matrix------------*/

void product(float *in, float *w, float *o, int r, int l)
{
		float sum = 0;
		for (int j = 0; j < r; ++j)
		{
				sum += w[j];
				for (int i = 1; i < l; ++i)
				{
						sum += in[i-1] * w[j+i*r];
				}
				o[j] = sum;
				o[j] = sigmoid(sum);
				sum = 0.0;
		}
}


/*=============End of forward propagation===============*/


/*=================Backward propagation=================*/
/*
 *dst est vecteur et non matrice
 *Actual est vecteur et non matrice
 *Output est vecteur et non matrice
 *This function is done
 */
/*This function compute the delta value for the output layer*/
/*
   void DeltaOutput(float *dst,float *Actual, float *Expect, int l)
   {
   for (int j = 0 ; j < l ; ++j)
   {
   dst[j] = (Actual[j] - Expect[j])*
   Actual[j] * (1 - Actual[j]);
   }
   }
 */

void DeltaOutput(float *dst,float *Actual, float *Expect, int r, int l)
{
		for (int j = 0 ; j < l ; ++j)
		{
				int lineoffset = j * r;
				for (int i = 0 ; i < r ; ++i)
				{
						dst[lineoffset + i] = (Actual[lineoffset+i] - Expect[lineoffset+i])*
								Actual[lineoffset+i] * (1 - Actual[lineoffset+i]);
				}
		}
}


/*This fuction compute the product of the delta of output layer and weight
  between hidden layer and output layer*/

void deltaproduct(float *dst, float *W, float* delta, int c, int l)
{
		float sum = 0.0;
		for (int j = 1 ; j < l ; ++j)
		{
				int lineoffset = j*c;
				for (int i = 0 ; i < c ; ++i)
				{
						sum += W[lineoffset+i] * delta[i];
				}
				dst[j-1] = sum;
				sum = 0.0;
		}
}


/*
   void deltaproduct(float *dst, float *W, float* delta, int c, int l)
   {
   for (int j = 0 ; j < l ; ++j)
   {
   int lineoffset = (j)*c;
   for (int i = 0 ; i < c ; ++i)
   {
   dst[lineoffset+i] = W[lineoffset+i+c] * delta[i%l];
   }
   }
   }
 */

/*This function compute the delta value for the hidden layer*/
/*
   void DeltaHidden(float *dst, float *delta, float *hidden, int l)
   {
   for (int i = 0; i < l; ++i)
   {
   dst[i] = hidden[i] * (1 - hidden[i])* delta[i];
   }
   }
 */
/*
   This function compute the delta value for the hidden layer
 */
void DeltaHidden(float *dst, float *delta, float *hidden, int c, int l)
{
		float sum = 0;
		for (int j = 0 ; j < l ; ++j)
		{
				for (int i = 0 ; i < c ; ++i)
				{
						sum+=delta[j*c+i];
				}
		}
		for (int j = 0 ; j < l ; ++j)
		{
				int lineoffset = j*c;
				for (int i = 0 ; i < c ; ++i)
				{
						dst[lineoffset+i] = hidden[lineoffset+i] *
								(1 - hidden[lineoffset+i] ) * sum;
				}
		}
}


/*This function update the weights after observing the error rate between
  actuel result and expected result*/
void newWeight(float *W, float* M, float* Delta, float lr, int r, int l)
{
		for (int j = 0 ; j < r ; ++j)
		{
				W[j] = W[j] - lr * Delta[j];
				for (int i = 1 ; i < l ; ++i)
				{
						W[r*i + j] =W[r*i+j]- ( lr * Delta[j] * M[j]);
				}
		}
}

/*This function convert a binary to a decimal*/
char fromBin (int *array, int l)                                                  
{                                                                                
		int result = 0;                                                              
		for (int i = 0; i < l; ++i)                                                  
		{                                                                            
				result = 2 * result + array[i];                                          
		}                                                                            
		return (char) result;                                                               
}  

void toBin (float *array, int n)                                                 
{                                                                                
		int i = 0;                                                                   
		while (n > 0)                                                                
		{                                                                            
				array[i] = (float)(n % 2);                                               
				n = n / 2;                                                               
				++i;                                                                     
		}                                                                            
}

void printVect(float *M, int l)
{
	for (int j = 0; j < l; ++j)
			printf("%f | \n", M[j]);
}	

void printMatrix(float* M, int r, int l){
		for (int j = 0 ; j < l; ++j){
				int lineoffset = j*r;
				for (int i = 0 ; i < r ; ++i){
						printf(" %f |",M[lineoffset+i]);
				}
				printf("\n");
		}
		printf("\n");
}

/*==========End Backward Propagation========*/
int main()
{
	int nbInput = 4;
	float *input = calloc(nbInput, sizeof(float));	
	float *output = calloc(nbInput, sizeof(float));
	input[0] = 0;
	input[1] = 1;
	input[2] = 0;
	input[3] = 1;

	float *WIH = calloc	(nbInput * (nbInput+1),  sizeof(float));
	initWeight(WIH, nbInput, nbInput+1);
	printMatrix(input, 1, 4);
	printf("\n");
	writeFile("weightIH.txt", WIH, nbInput * (nbInput+1));
	printMatrix(WIH, nbInput , (nbInput+1));
	printf("\n");
	readFile("weightIH.txt", WIH, nbInput * (nbInput+1));
	printMatrix(WIH, nbInput , (nbInput+1));
	product(input, WIH, output, nbInput, nbInput+1);
	printMatrix(output, 1,  nbInput);

	return 0;
}
