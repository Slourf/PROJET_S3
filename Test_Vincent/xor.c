# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>

/*========== Read&Write matrix into file =========*/
/*This part is not used yet but it will be used
later in the projet.*/

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
             ++b;
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
                float rnd = ((float)rand()/(double)RAND_MAX);
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
		o[j] = sigmoid(sum);
		sum = 0.0;
	}
}

/*=============End of forward propagation===============*/


/*=================Backward propagation=================*/

/*This function compute the delta value for the output layer*/
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
	for (int j = 0 ; j < l ; ++j)
	{
		int lineoffset = (j)*c;
		for (int i = 0 ; i < c ; ++i)
		{
			dst[lineoffset+i] = W[lineoffset+i+c] * delta[i%l];
		}
	}
}


/*This function compute the delta value for the hidden layer*/
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

/*==========End Backward Propagation========*/



int main(int argc, char* argv[])
{
	int nbInput = 2;
	int nbOutput = 1;
	int nbHidden = 2;
	float *input = malloc(nbInput * sizeof(float));
	float inputs[] = { 1, 1, 0, 0, 0, 1, 1, 0 };
	float outputs[] = { 0, 0, 1, 1 };
	float *outexpected = malloc(nbOutput * sizeof(float));
	float *output = malloc(nbOutput * sizeof(float));
	float *hidden = malloc(nbHidden * sizeof(float));
	float *wIH = malloc(nbHidden * (nbInput+1) * sizeof(float));
	float *wHO = malloc(nbOutput * (nbHidden+1) * sizeof(float));
	initWeight(wIH, nbHidden, nbInput+1);
	initWeight(wHO, nbOutput, nbHidden+1);
	float *outputDelta = malloc(nbOutput * sizeof(float));
	float *productDelta = malloc(2 * sizeof(float));
	float *deltaHidden = malloc(1* 2* sizeof(float));

/*=======================Training loop======================*/
	for (int t = 0 ; t < 100000 ; ++t)
	{
		input[0] = inputs[2*(t%4)];
		input[1] = inputs[2*(t%4)+1];
		outexpected[0] = outputs[t%4];
		product(input, wIH, hidden, 2, 3);
		product(hidden, wHO, output, 1, 3);
		DeltaOutput(outputDelta, output, outexpected, 1 , 1 );
		deltaproduct(productDelta, wHO, outputDelta, 1, 2);
		DeltaHidden(deltaHidden, productDelta, hidden, 1,2);
		newWeight(wHO , hidden, outputDelta, 0.3, 1, 3);
		newWeight(wIH , input, deltaHidden, 0.3, 2, 3);
	}
/*===================End of training loop======================*/
	if (argc == 1)
	{
		input[0] = 1;
		input[1] = 1;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);
		printf("Got: \n%f XOR %f = %f\n",input[0],input[1],
											 (output[0]));
		printf("Got: \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
											 (output[0]>0.5)?1:0);
		input[0] = 0;
		input[1] = 0;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);
		printf("Got: \n%f XOR %f = %f\n",input[0],input[1],
											 (output[0]));
		printf("Got: \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
											 (output[0]>0.5)?1:0);
		input[0] = 0;
		input[1] = 1;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);
		printf("Got: \n%f XOR %f = %f\n",input[0],input[1],
											 (output[0]));
		printf("Got: \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
											 (output[0]>0.5)?1:0);
		input[0] = 1;
		input[1] = 0;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);
		printf("Got: \n%f XOR %f = %f\n",input[0],input[1],
											 (output[0]));
		printf("Got: \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
											 ((output[0]>0.5)?1:0));
	}
	else
	{
		int x1, x2;
		x1 = atoi(argv[1]);
		x2 = atoi(argv[2]);

		input[0] = x1;
		input[1] = x2;
		product(input, wIH, hidden, 2,3);
		product(hidden, wHO, output, 1, 3);
		printf("Got: \n%f XOR %f = %f\n",input[0],input[1],
											 (output[0]));
		printf("Got: \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
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
}
