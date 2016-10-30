# include <stdio.h>
# include <stdlib.h>
# include "readWrite.h"
# include <math.h>

/* Initialize weights  */
void initWeight(float* mat, int x, int y)
{   
    for (int i = 0; i < x; ++i)
    {
        int lineoffset = i*x;
        for (int j = 0; j < y; ++j)
        {
            mat[lineoffset+j] = ((float)rand()/(double)RAND_MAX);
        }
    }
}
 
//Just using it to debug
void printmatrix(float* mat, int x)
{   
    for (int i = 0; i < x; ++i)
    {
		printf("| %f", mat[i]);
    }
 }
/*
void sumWeigt(float *sum, float *i, *w, int x, int y)
{
	float s = 0;
	for (int i = 0; i < x; ++i)
	{
		int lineoffset = i*x;
		for (int j = 0; j < y; ++j)
		{
			s += i[j] * w[j*lineoffset];
		}
		sum[i] = s;
		s = 0;
	}
}*/

int main()
{
	float inputs[] = { 1, 1, 0, 0, 0, 1, 1, 0 };
	float outputs[] = { 0, 0, 1, 1 };
	float *wIH = malloc(6 * sizeof(float));
	initWeight(wIH, 3, 2);
	printmatrix(wIH, 6);

	return 0;
}
