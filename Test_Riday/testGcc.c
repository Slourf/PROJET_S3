#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *arcv[]){
   int a, b;
   if (argc != 3)
   {
	printf ("\nError : nombre d'arguements incorrect !");
	return (EXIT_FAILURE);
   } 
   a = atoi(arcv[1]);
   b = atoi(arcv[2]);
   printf("%d + %d = %d\n", a, b, a + b);
   return (EXIT_SUCCESS);
}

