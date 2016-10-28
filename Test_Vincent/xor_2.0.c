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

void sumWeight (float* zh, float* xi, float* wxh, int x, int y)
{
	int sum = 0;
	for (int i = 0 ; i < x; ++i)
	{
		int lineoffset = i*x; 
		for (int j = 0; j < y ; ++j)
		{
			sum+= xi[j] * wxh[j*lineoffset]; //verifier cette ligne
		}
		zh[i] = sum;
		sum = 0;
	}
}

float ActFunc(float f)
{
	return tanh(f);
}

void actValue (float* hAct, float* zh, int x)
{
	hAct[0] = 1 ; 
	for (int i = 0 ; i < x-1 ; ++i)
			{
				hAct[i+1] = ActFunc(zh[i]);
			}
}

void probAnswer(float* zy, float* prob, int l)
{
	int sum = 0; 
	for (int i = 0 ; i < l ; ++i)
	{
	prob[i] = exp(zy[i]);
		sum += prob[i] ;
	}
	for (int i = 0 ; i < l ; ++i)
	{
		prob[i] /= sum;
	}
}

void outPutError(float* outputErr, float* prob, float* y, int l)
{
	for (int i = 0 ; i < l ; ++i)
	{
		outputErr[i] = prob[i] - y[i]; 
	}
}

void findGradient(float* delta, float* hAct, float* out, int x, int y)
{
	for (int i = 0 ; i < x ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j) 
		{
			delta[lineoffset+j] = hAct[i] * out[j];
		}
	}
}
/*

		BACKWARD PROPAGATION

*/
void hActValue(float* hAct, float* why, float* out, int x, int y)
{
	for (int i = 1 ; i < x ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j)
		{
			hAct[i] = why[lineoffset+j] * out[j];
		}
	}
}

void partDeri(float* zh, float* hact, int x)
{
	for (int i = 0 ; i < x ; ++i) 
	{
		zh[i] = hact[i+1] * (1-pow(tanh(zh[i]),2));
	}
}

void changeXH (float* delta, float* xi, float* zh, int x, int y)
{
	for (int i = 0 ; i < x ; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0 ; j < y ; ++j)
		{
			delta[lineoffset+j] = xi[i] * zh[j];
		}
	}
}

void changeWeight(float* wh, float* delta, int x, int y, int learningrate)
{
	for (int i = 0 ; i < x ; ++i)
	{
		int lineoffset = i * x; 
		for (int j = 0 ; j < y ; ++j)
		{
			wh[lineoffset+j] -= learningrate * delta[lineoffset+j]; 
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
	
	for(int t = 0 ; t < 1007 ; ++t)
	{
		x[0] = 1; 
		x[1] = inputs[2*(t%4)];
		x[2] = inputs[2*(t%4)+1];
		
		y[0] = outputs[t%4];
		
//		printf("%f %f %f %f\n",x[0],x[1],x[2], y[0]);		
		sumWeight(h, x, coefInput, nbHidden, nbInput);
		
		actValue(hActVal, h,nbHidden);
		sumWeight(WeightSum, hActVal, coefOutput, nbOutput, nbHidden);
		probAnswer(outActVal, WeightSum, nbOutput);
		outPutError(outError, outActVal, y, nbOutput);
		findGradient(dhAdjust, hActVal, outError, nbHidden+1, nbOutput);
		hActValue(hActVal, coefOutput, outError, nbHidden+1, nbOutput);
		partDeri(h, hActVal, nbHidden);
	changeXH(dxAdjust, x, h, nbInput+1, nbHidden);
		changeWeight( coefOutput, dhAdjust, nbHidden+1, nbOutput, learnRate);	
		changeWeight( coefInput, dxAdjust, nbInput+1, nbHidden, learnRate);
	/*	loss = 0;
		for (int i = 0; i < nbOutput; i++) 
		{
			loss -= y[0] * log(outActVal[i]);
		}
		printf("%f\n",loss); */
	}

	printf("%f XOR %f = %f\n", x[1], x[2], y[0]); 
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
