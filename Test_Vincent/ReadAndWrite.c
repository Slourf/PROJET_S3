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



int main(int argc,char *argv[])
{
		
	writeFile("test.txt", argv[1]);
	readFile("test.txt");
	return 0;

}
