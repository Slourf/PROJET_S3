#include <stdio.h>
#include <stdlib.h>

unsigned int percep (unsigned int x1, unsigned int x2) 
{
	unsigned int output;
	unsigned int w1 = 1, w2 = 0;
	unsigned int threshold = 1;

	output = w1 * x1 + w2 + x2;
	return output > threshold ? 1 : 0;
}

unsigned int nand (unsigned int x1, unsigned int x2) 
{
	 int output;
	
	output = x1 * (-2) + x2 * (-2) + 3;
	return (output > 0) ? 1 : 0;
}

int main()
{
	printf("%i NAND %i = %i\n", 0, 0, nand(0, 0));
	printf("%i NAND %i = %i\n", 0, 1, nand(0, 1));
	printf("%i NAND %i = %i\n", 1, 0, nand(1, 0));	
	printf("%i NAND %i = %i\n", 1, 1, nand(1, 1));

	return 0;
}
