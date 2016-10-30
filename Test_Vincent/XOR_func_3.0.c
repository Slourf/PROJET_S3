# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <assert.h>
# include <math.h>


float sigmoid(float f)
{
	return 1/(1+exp(-f));
}

float sigmoidDerivative(float f)
{
	return f * (1-f);
}


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



int main()
{
	float inputs[] = {0,0,0,1,1,0,1,1};
	float outpus[] = { 0 , 1 , 1 , 0};
	int nbInputs = 2; 
	int nbOutputs = 1; 
	int nbHidden = 2;

	return 0;
}
