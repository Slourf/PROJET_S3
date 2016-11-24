# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>

/*========== Read&Write matrix into file =========*/

void writeFile(char* path, float *a, size_t len)
{
    FILE* file = fopen(path, "w");
    if (file != NULL)
    {
        for (int i = 0; i < len; i++)
        {
            fprintf(file, "%f\n", *a);
            a++;
        }
        fclose(file);
    }
    else
        errx(1, "Invalid path");
}

void readFile(char *path, float *b, size_t len)
{
    FILE* file = fopen(path, "r");
    if (file != NULL)
    {
        for (int i = 0; i < len; i++)
        {
             fscanf(file, "%f", (b+i));
             ++b;
        }
        fclose(file);
    }
    else
        errx(1, "Invalid path");
}




/*======Forward Propagation========*/

void print(float *M, int r, int l)
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

void printVec(float *v, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("%f|", v[i]);
	}
}


/*-----------Initialize weights*-----------*/ 
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

/*------layer matrix * weight matrix-----*/
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
//		printf("%f\n", sum);
		o[j] = sigmoid(sum);
		sum = 0.0;
	}
}

/*==========End Forward Propagation=========*/


int main()
{
	int nbInput = 2;
	int nbOutput = 1;
	int nbHidden = 2;
//	float *input = malloc(nbInput * sizeof(float));
	float testInput[2] = {1, 1};
	float *t = testInput;
	float *output = malloc(nbOutput * sizeof(float));
	float *hidden = malloc(nbHidden * sizeof(float));
	float *wIH = malloc(nbHidden * (nbInput+1) * sizeof(float));
	float *wHO = malloc(nbOutput * (nbHidden+1) * sizeof(float));
	initWeight(wIH, nbHidden, nbInput+1);
	initWeight(wHO, nbOutput, nbHidden+1);
//	printf("Coef IH\n");
//	print(wIH, nbHidden, nbInput+1);
//	printf("coef HO\n");
//	print(wHO, nbOutput, nbHidden+1);
	product(t, wIH, hidden, nbHidden, nbInput+1);
//	printf("hidden layer\n");
//	printVec(hidden, nbHidden);
	product(hidden, wHO, output, nbOutput, nbHidden+1);
//	printf("output layer\n");
//	printVec(output, nbOutput);
}	