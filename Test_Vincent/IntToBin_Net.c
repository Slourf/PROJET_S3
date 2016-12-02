# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>
# include "exploreFiles.h"
# include "main.h"

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
						float rnd = ((float)rand()/(double)RAND_MAX)*0.2-0.1;
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
				o[j] =
						sigmoid(sum);
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
						dst[lineoffset + i] = (Actual[lineoffset+i] - 
						Expect[lineoffset+i])*Actual[lineoffset+i] * 
						(1 - Actual[lineoffset+i]);
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

void training ()
{
		int nbInput = 15 *  15;
		int nbOutput = 7;
		int nbHidden = 15 * 15;
		//int nbTests=20000000;
		float *input = malloc(nbInput * sizeof(float));
		float *outputs = calloc(nbOutput * sizeof(float));

		float *outexpected = malloc(nbOutput * sizeof(float));
		float *output = malloc(nbOutput * sizeof(float));
		float *hidden = malloc(nbHidden * sizeof(float));
		float *wIH = malloc(nbHidden * (nbInput+1) * sizeof(float));
		float *wHO = malloc(nbOutput * (nbHidden+1) * sizeof(float));
		float *matrix_image = calloc (nbInput, sizeof(float));
		initWeight(wIH, nbHidden, nbInput+1);
		initWeight(wHO, nbOutput, nbHidden+1);
		float *outputDelta = malloc(nbOutput * sizeof(float));
		float *productDelta = malloc(nbHidden * sizeof(float));
		float *deltaHidden = malloc(nbOutput* nbHidden* sizeof(float));
		char ** characters = calloc(52, sizeof(float*));
		int iter_char = 0;
		fileName(52, characters);
		char chr = characters[iter_char];
		struct dirent *reading;
		DIR * dir = opendir("CharFile/" + chr);
		while(reading = readdir(dir)){
				//appel fonction image->matrice
				int index_input = 0;
				for (int i = 0; i < 15; ++i)
				{
						int lineofset = i * 15;
						for (int j = 0; j < 15; ++j)
						{
								input[index_input] = matrix_image[lineofset + j];
								++index_input;
						}
				}
				//a verifier cette ligne 
				toBin(outexpected, chr);
		}
	
		product(input, wIH, hidden, nbhidden, nbinput + 1);                                       
        product(hidden, wHO, output, nboutput, nbhidden + 1);                                      
        DeltaOutput(outputDelta, output, outexpected, 1 , nbOutput);                   
        deltaproduct(productDelta, wHO, outputDelta, 1, 2);                      
        DeltaHidden(deltaHidden, productDelta, hidden, 1,2);                     
        newWeight(wHO , hidden, outputDelta, 0.3, 1, 3);                         
        newWeight(wIH , input, deltaHidden, 0.3, 2, 3);

		writeFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));                    
		writeFile("weightHO.txt"., wHO, nbOutput * (nbHidden + 1));
}

char *single_forward ()
{
		
		struct text *txt = cut(img);
		//boucle repetant autant de fois que les carac a reconnaitre 
		//txt->line[i]->->mat[j];


		int index_result = 0;
		for (int i = 0; i < txt->size; ++i)
		{
			for (int j = 0; j < txt->line[i]->size; ++j)
		{
		int nbInput = 15 *  15;
		int nbOutput = 7;
		int nbHidden = 15 * 15;
		float *input = malloc(nbInput * sizeof(float));
		float *outputs  = calloc(nbOutput * sizeof(float))
		
		float *matrix_image = calloc(nbInput, sizeof(float));
		int *result = malloc(nbOutput * sizeof(int));
		float *outexpected = malloc(nbOutput * sizeof(float));
		float *output = malloc(nbOutput * sizeof(float));
		float *hidden = malloc(nbHidden * sizeof(float));
		float *wIH = malloc(nbHidden * (nbInput+1) * sizeof(float));
		float *wHO = malloc(nbOutput * (nbHidden+1) * sizeof(float));

		readFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));                    
		readFile("weightHO.txt"., wHO, nbOutput * (nbHidden + 1));

		float *outputDelta = malloc(nbOutput * sizeof(float));
		float *productDelta = malloc(nbHidden * sizeof(float));
		float *deltaHidden = malloc(nbOutput* nbHidden* sizeof(float));
		char *carac = malloc(nbchar * sizeof(char));
		char ** characters = calloc(52 , sizeof(char*));

		int index_input = 0;
		index_result += 1;
		//appel image -> matrix
		for (int x = 0; x < 15; ++x)
		{ 
				for (int y = 0; y < 15; ++y)
				{
						input[index_input] = txt->line[i]->->mat[j]->data[x][y];
						++index_input;
				}
		}
		index_input = 0;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);

		for(int i = 0; i < nbOutput; ++i)
		{
				if(output[i] > 0.5)
						result[i] = 1;
				else 
						result[i] = 0;
		}

		carac[index_result] = fromBin (result, nbOutput);
		++index_result;
		}
		}
		return carac;

}

int main(int argc, char* argv[])
{
		int nbInput = 1;
		int nbOutput = 4;
		int nbHidden = 4;
		int nbTests=20000000;
		float *input = malloc(nbInput * sizeof(float));
		float inputs[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		float outputs[] = { 0, 0, 0, 0,
				0, 0, 0, 1,
				0, 0, 1, 0,
				0, 0, 1, 1,
				0, 1, 0, 0,
				0, 1, 0, 1,
				0, 1, 1, 0,
				0, 1, 1, 1,
				1, 0, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 1,
				1, 1, 0, 0,
				1, 1, 0, 1,
				1, 1, 1, 0,
				1, 1, 1, 1};
		float *outexpected = malloc(nbOutput * sizeof(float));
		float *output = malloc(nbOutput * sizeof(float));
		float *hidden = malloc(nbHidden * sizeof(float));
		float *wIH = malloc(nbHidden * (nbInput+1) * sizeof(float));
		float *wHO = malloc(nbOutput * (nbHidden+1) * sizeof(float));
		initWeight(wIH, nbHidden, nbInput+1);
		initWeight(wHO, nbOutput, nbHidden+1);
		float *outputDelta = malloc(nbOutput * sizeof(float));
		float *productDelta = malloc(nbHidden * sizeof(float));
		float *deltaHidden = malloc(nbOutput* nbHidden* sizeof(float));

		/*=======================Training loop======================*/
		for (int t = 6 ; t < 7 ; ++t)
		{
				input[0] = inputs[(t%8)];
				outexpected[0] = outputs[(4*t)%32];
				outexpected[1] = outputs[(4*t+1)%32];
				outexpected[2] = outputs[(4*t+2)%32];
				outexpected[3] = outputs[(4*t+3)%32];

				printf("Init values\n WIH\n");
				printMatrix(wIH, nbHidden, nbInput+1);
				printf("WHO\n");
				printMatrix(wHO, nbOutput, nbHidden+1);

				product(input, wIH, hidden, 1,  nbHidden+1);
				product(hidden, wHO, output, 1,  nbHidden+1);
				DeltaOutput(outputDelta, output, outexpected, 1 , nbOutput);			//#FIXME
				deltaproduct(productDelta, wHO, outputDelta, nbOutput, nbHidden);		//#FIXME (maybe mat mult?)
				DeltaHidden(deltaHidden, productDelta, hidden, nbOutput,nbHidden);	//#FIXME (same?)
				newWeight(wHO , hidden, outputDelta, 0.3, nbOutput, nbHidden+1);
				newWeight(wIH , input, deltaHidden, 0.3, nbInput, nbHidden+1);

				printf("Input = %f\n", input[0]);
				printf("\nOutexpected:\n");
				printMatrix(outexpected, 1, 4);
				printf("\nWeight Input to Hidden\n");
				printMatrix(wIH, nbHidden, nbInput+1);
				printf("\nHidden:\n");
				printMatrix(hidden, 1, nbHidden);
				printf("\nWeight Hidden to Output\n");
				printMatrix(wHO, nbOutput, nbHidden+1);
				printf("\nOutput:\n");
				printMatrix(output, 1,nbOutput);
				printf("\nOutputDelta:\n");
				printMatrix(outputDelta, 1, nbOutput);
				printf("\nDeltaHidden\n");
				printMatrix(hidden, nbInput, nbHidden+1);
				printf("\nNew wOH:\n");
				printMatrix(wHO, nbHidden, nbInput+1);
				printf("\nNew wIH:\n");
				printMatrix(wIH, nbInput, nbHidden+1);

		}
		/*
		   for (int i = 0 ; i < 8 ; ++i)
		   {
		   input[0] = inputs[i%8];
		   product(input, wIH, hidden, nbInput, nbHidden+1);
		   product(hidden, wHO, output, nbOutput, nbHidden+1);
		   printf("%i = ",(int)input[0]);
		   for (int j = 0 ; j < 4; ++j)
		   {
		//	printf("%f ", (output[j]));
		printf("%i", (output[j]>0.5)?1:0);
		}
		printf("\n");

		}
		 */


		/*		printf("%i = ",(int)input[0]);
				for (int j = 0 ; j < 4; ++j)
				{
				printf("%f ", (outexpected[j]));
		//	printf("%i", (output[j]>0.5)?1:0);
		}
		printf("\n");
		 */
}

/*===================End of training loop======================*/
/*	if (argc == 1)
	{
	for (int i = 0 ; i < 8 ; ++i)
	{
	input[0] = inputs[i%8];
	product(input, wIH, hidden, nbInput, nbHidden+1);
	product(hidden, wHO, output, nbOutput, nbHidden+1);
	printf("%i = ",(int)input[0]);
	for (int j = 0 ; j < 4; ++j)
	{
	printf("%f ", (output[j]));
//	printf("%i", (output[j]>0.5)?1:0);
}
printf("\n");

}*/

/*	input[0] = 1;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 1;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	((output[0]>0.5)?1:0));
 */
/*	}
	else
	{
	int x1, x2;
	x1 = atoi(argv[1]);
	x2 = atoi(argv[2]);

	input[0] = x1;
	input[1] = x2;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf(" \n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	}
	free(input);
	free(outexpected);
	free(output);
	free(hidden);
	free(wIH);
	free(wHO);
	free(outputDelta);
	free(productDelta);
	free(deltaHidden);
	return 0;
	}*/
