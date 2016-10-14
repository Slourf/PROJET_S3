# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <assert.h>
# include <math.h>
/*

ONLY WORKS FOR XOR FUNCTION

*/
void readFile(char* path)
{
	FILE* file = fopen(path, "r");
	char buff[1000];
	if (file != NULL)
	{
		//Read coefficients for the neural network
		//Read the last line of the log 
		while (fgets(buff, 1000, file) != NULL)
		{
			printf("%s", buff);	
			 
		}
	}
	else
	{
		errx(1, "Invalid path");	
	}
}

void writeFile(char* path, char* str) 
{
	FILE* file = fopen(path, "w");
	if (file != NULL)
	{
		//Write coefficients after tests
		printf("File opened\n");
		fputs(str, file);
		fclose(file); 
	}
	else 
	{
		errx(1, "Invalid path");
	}
}

int xorFunction(int x1, int x2)
{

	return 0 ;
}

float sigmoid(double f)
{
	return (float)(1/(1+exp(f)));
}

float sum(int nbinput, int *inputs, int nbweight, int *weight)
{
	float res = 0;
	int *endinput = inputs + nbinput;
	int *endweight = weight + nbweight;
	for (;inputs < endinput - 1 ; ++inputs )
	{
		res+= (*inputs) * (*weight);
		++weight;
	}
	res = sigmoid(res - *weight);
	return res;
}



int activate(float sig)
{
	return 1 ? sig>=1/2 : 0;
}

void init(char *path, float *coeff)
{
	char str[1000];
	for (int i = 0 ; i < 3 ; ++i)
	{
		*coeff = ((float)rand() / (double)RAND_MAX) * 0.2 - 0.1;
		coeff++;
	}
	coeff= coeff-3; 
	for (int j = 0; j < 3; ++j)
		{
			
		}

	writeFile( path, coeff );
}

int main(int argc,char *argv[])
{
		
	writeFile("test.txt", argv[1]);
	readFile("test.txt");
	return 0;

}
