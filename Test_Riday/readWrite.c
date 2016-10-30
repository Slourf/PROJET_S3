# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <assert.h>

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

void printVector(float *p, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		printf("%f\n", *(p+i));
		p++;
	} 
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
int main ()
{
	float weight[] = {1.234567, 2.134567, 3.124567, 4.123567};
	float *a = weight;
	size_t len = 4;
	float result[len];
	float *b = result;
	writeFile("test.txt", a, len);
	readFile("test.txt", b, len);
	b = result;
	printVector(b, len);
}
