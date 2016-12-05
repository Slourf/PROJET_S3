# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include <sys/types.h>
# include <dirent.h>
# include "exploreFiles.h"
# include "main.h"

/*========== Read&Write matrix into file =========*/

/*----------Save a matrix into a file------------*/
void writeFile(char* path, float *a, size_t len)
{
	FILE* file = fopen(path, "w");
	float* tmp = a;
	if (file != NULL)
	{
		for (size_t i = 0; i < len; i++)
		{
			fprintf(file, "%f\n", *a);
			a++;
		}
		a = tmp;
		fclose(file);
	}
	else
		errx(1, "Invalid path");
}

void writeFileChar(char* path, char *a, size_t len)
{
	FILE* file = fopen(path, "w");
	char* tmp = a;
	if (file != NULL)
	{
		for (size_t i = 0; i < len; i++)
		{
			fprintf(file, "%c", *a);
			a++;
		}
		a = tmp;
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
/*
 *dst est vecteur et non matrice
 *Actual est vecteur et non matrice
 *Output est vecteur et non matrice
 *This function is done
 */
/*This function compute the delta value for the output layer*/

void DeltaOutput(float *dst,float *Actual, float *Expect, int l)
{
	for (int j = 0 ; j < l ; ++j)
	{
		dst[j] = (Actual[j] - Expect[j])*
			Actual[j] * (1 - Actual[j]);
	}
}

/*
	void DeltaOutput(float *dst,float *Actual, float *Expect, int r, int l)
	{
	for (int j = 0 ; j < l ; ++j)
	{
	int lineoffset = j * r;
	for (int i = 0 ; i < r ; ++i)
	{
	dst[lineoffset + i] = (Actual[lineoffset+i] -
	Expect[lineoffset+i])*Actual[lineoffset+i] *
	(1 - Actual[lineoffset+i]);
	}
	}
	}
 */

/*This fuction compute the product of the delta of output layer and weight
  between hidden layer and output layer*/

void deltaproduct(float *dst, float *W, float* delta, int c, int l)
{

	float sum = 0.0;
	for (int j = 1 ; j < l ; ++j)
	{
		int lineoffset = j*c;
		for (int i = 0 ; i < c ; ++i)
		{
			sum += W[lineoffset+i] * delta[i];
		}
		dst[j-1] = sum;
		sum = 0.0;
	}
}


/*
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
 */

/*This function compute the delta value for the hidden layer*/

void DeltaHidden(float *dst, float *delta, float *hidden, int l)
{
	for (int i = 0; i < l; ++i)
	{
		dst[i] = hidden[i] * (1 - hidden[i])* delta[i];
	}
}

/*
	This function compute the delta value for the hidden layer
 */
/*
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
 */

/*This function update the weights after observing the error rate between
  actuel result and expected result*/
void newWeight(float *W, float* M, float* Delta, float lr, int r, int l)
{
	for (int j = 0 ; j < r ; ++j)
	{
		W[j] = W[j] - lr * Delta[j];
		for (int i = 1 ; i < l ; ++i)
		{
			//	printf("%f = %f - %f * %f * %f\n",W[r*i+j],W[r*i+j], lr, Delta[j],M[i-1]);
			W[r*i + j] =W[r*i+j]- ( lr * Delta[j] * M[i-1]);

		}
	}
	//		printf("\n");
}

/*This function convert a binary to a decimal*/
char fromBin (int *array, int l)
{
	int result = 0;
	for (int i = l; i > 0; --i)
	{
		result = 2 * result + array[i-1];
	}
	return (char) result;
}

void toBin (float *array, int n)
{
	int i = 0;
	while (n > 0)
	{
		array[i] = (float)(n % 2);
		n = n / 2;
		++i;
	}
}

void printMatrix(float* M, int r, int l)
{
	for (int j = 0 ; j < l; ++j){
		int lineoffset = j*r;
		for (int i = 0 ; i < r ; ++i){
			printf(" %f |",M[lineoffset+i]);
		}
		printf("\n");
	}
	printf("\n");
}

void printimg(float* M, int c, int l){
	for (int j = 0 ; j < l ; ++j){
		int lineoffset = j*l;
		for (int i = 0 ; i < c ; ++i){
			printf("%c",M[lineoffset+i]>0?'*':'_');
		}
		printf("\n");
	}
	printf("\n");
}

/*==========End Backward Propagation========*/

void training ()
{
	int nbInput = 15 * 15;
	int nbOutput = 7;
	int nbHidden = 15 * 15;
	float *input = calloc(nbInput, sizeof(float));
	int nbTests = 20000;
	float *outexpected = calloc(nbOutput, sizeof(float));
	float *output = calloc(nbOutput, sizeof(float));
	float *hidden = calloc(nbHidden, sizeof(float));
	float *wIH = calloc(nbHidden * (nbInput+1), sizeof(float));
	float *wHO = calloc(nbOutput * (nbHidden+1), sizeof(float));
	initWeight(wIH, nbHidden, nbInput+1);
	initWeight(wHO, nbOutput, nbHidden+1);
	float *outputDelta = calloc(nbOutput, sizeof(float));
	float *productDelta = calloc(nbHidden, sizeof(float));
	float *deltaHidden = calloc(nbOutput* nbHidden, sizeof(float));
	char * characters = calloc(52, sizeof(char));
	FILE *f1,*f2;
	f1=fopen("weightIH.txt","r");
	f2=fopen("weightHO.txt","r");
	if(f1 && f2)
	{
		fclose(f1);
		fclose(f2);
		readFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
		readFile("weightHO.txt", wHO, nbOutput * (nbHidden + 1));
	}

	fileName(characters);
	char *path = calloc(15, sizeof(char));
	strcat(path, "CharFile/");
	struct dirent *charFileRead;
	DIR* mainFile;
	for (int t = 0 ; t < nbTests ; ++t){
		printf("Tour n°%i\n",t);
		for (int i = 0 ; i < 52 ; ++i){
			path[9] = characters[i];
			mainFile = opendir(path);
			while(( charFileRead = readdir(mainFile))){
				if (charFileRead->d_name[0] != '.'){
					char* buffer = calloc(30, sizeof(char));
					strcat(buffer, path);
					strcat(buffer, "/");
					strcat(buffer, charFileRead->d_name);
					SDL_Surface * img = load_image(buffer);
					//					printf("%s\n",buffer);
					struct text *txt = cut(img);
					SDL_FreeSurface(img);
					int index_input = 0;
					for (int y = 0; y < 15; ++y)
					{
						for (int x = 0; x < 15; ++x)
						{
							input[index_input] = txt->line[0]->mat[0]->data[x][y];
							++index_input;
						}
					}
					//	printimg(input, 15, 15);
					index_input = 0;
					toBin(outexpected, characters[i]);
					product(input, wIH, hidden, nbHidden, nbInput + 1);
					product(hidden, wHO, output, nbOutput, nbHidden + 1);
					DeltaOutput(outputDelta, output, outexpected, nbOutput);
					deltaproduct(productDelta, wHO, outputDelta,
							nbOutput, nbInput+1);
					DeltaHidden(deltaHidden, productDelta, hidden, nbInput);
					newWeight(wHO , hidden, outputDelta, 0.3, nbOutput, nbHidden+1);
					newWeight(wIH , input, deltaHidden, 0.3, nbHidden, nbHidden+1);
					free(buffer);
					free_text(txt);
				}
			}
			closedir(mainFile);
		}
		writeFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
		writeFile("weightHO.txt", wHO, nbOutput * (nbHidden + 1));
	}
	writeFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
	writeFile("weightHO.txt", wHO, nbOutput * (nbHidden + 1));
	free(input);
	free(outexpected);
	free(output);
	free(hidden);
	free(wIH);
	free(wHO);
	free(outputDelta);
	free(productDelta);
	free(deltaHidden);
	free(characters);
	free(path);
}

char *single_forward (struct text *img)
{
	int index_result = 0;
	int nbInput = 15 *  15;
	int nbOutput = 7;
	int nbHidden = 15 * 15;
	float *input = calloc(nbInput, sizeof(float));

	int *result = calloc(nbOutput, sizeof(int));
	float *output = calloc(nbOutput, sizeof(float));
	float *hidden = calloc(nbHidden, sizeof(float));
	float *wIH = calloc(nbHidden * (nbInput+1), sizeof(float));
	float *wHO = calloc(nbOutput * (nbHidden+1), sizeof(float));

	readFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
	readFile("weightHO.txt", wHO, nbOutput * (nbHidden + 1));
	int nbChar = 0;
	for (size_t i = 0; i < img->size; ++i)
	{
		for (size_t j = 0; j < img->line[i]->size; ++j)
			++nbChar;
		++nbChar;
	}

	printf("Number of characters %i\n",nbChar);
	char *carac = calloc(nbChar+2, sizeof(char));
	for (size_t i = 0; i < img->size; ++i)
	{
		for (size_t j = 0; j < img->line[i]->size; ++j)
		{

			int index_input = 0;
			for (int y = 0; y < 15; ++y)
			{
				for (int x = 0; x < 15; ++x)
				{
					input[index_input] = img->line[i]->mat[j]->data[x][y];
					++index_input;
				}
			}
			int count = 0 ;
			while (count<15*15 && input[count]==0.0)
				++count;
			if (count == 15*15){
				carac[index_result] = ' ';
				++index_result;
			}
			else{
				product(input, wIH, hidden, nbInput,nbInput+1);
				product(hidden, wHO, output, nbOutput, nbHidden+1);

				for(int i = 0; i < nbOutput; ++i)
					if (output[i] > 0.5)
						result[i]=1;
					else
						result[i]=0;
				carac[index_result] = fromBin (result, nbOutput);
				++index_result;
			}
		}

		carac[index_result] = '\n';
		++index_result;
	}
	writeFileChar("Out.txt",carac, nbChar);
	free(input);
	free(output);
	free(hidden);
	free(wIH);
	free(wHO);
	free(result);
	return carac;

}


int main()
{
	//training();
	SDL_Surface * img = load_image("../../working_features/Merged/just.bmp");
	struct text *txt = cut(img);
	char * string = single_forward(txt);
	printf("%s\n", string);
	SDL_FreeSurface(img);
	free(string);

	/*
		int nbInput = 3;
		int nbOutput = 3;
		int nbHidden = 3;
		int nbTests=20000000;
		float *input = calloc(nbInput , sizeof(float));
		float inputs[] = {0,0,0,
		0,0,1,
		0,1,0,
		0,1,1};
		float outputs[] = {0,0,1,
		0,1,0,
		0,1,1,
		0,0,0};
		float *outexpected = calloc(nbOutput , sizeof(float));
		float *output = calloc(nbOutput , sizeof(float));
		float *hidden = calloc(nbHidden , sizeof(float));
		float *wIH = calloc(nbHidden * (nbInput+1) , sizeof(float));
		float *wHO = calloc(nbOutput * (nbHidden+1) , sizeof(float));
		initWeight(wIH, nbHidden, nbInput+1);
		initWeight(wHO, nbOutput, nbHidden+1);
		float *outputDelta = calloc(nbOutput , sizeof(float));
		float *productDelta = calloc(nbHidden , sizeof(float));
		float *deltaHidden = calloc(nbOutput* nbHidden, sizeof(float));


		for (int i = 0 ; i < 4 ; ++i)
		{
		input[0] = inputs[i%3];
		input[1] = inputs[i*3+1];
		input[2] = inputs[i*3+2];
		product(input, wIH, hidden, nbInput, nbHidden+1);
		product(hidden, wHO, output, nbOutput, nbHidden+1);
		printf("%f - %f - %f  = ",input[0], input[1], input[2]);
	//	printf("%f ", (output[j]));
	printf("%f - %f - %f", output[0] >0.5?1.0:0.0, output[1]>0.5?1.0:0.0,
	output[2]>0.5?1.0:0.0);
	printf("\n");

	}
	printf("\n\n\n");

	//	=======================Training loop======================
	for (int t = 0 ; t < nbTests ; ++t)
	{
	input[0] = inputs[(3*t%4)%12];
	input[1] = inputs[(3*t+1)%12];
	input[2] = inputs[(3*t+2)%12];
	outexpected[0] = outputs[(3*t)%12];
	outexpected[1] = outputs[(3*t+1)%12];
	outexpected[2] = outputs[(3*t+2)%12];
	product(input, wIH, hidden, nbInput, nbInput+1);
	product(hidden, wHO, output, nbOutput, nbHidden+1);
	DeltaOutput(outputDelta, output, outexpected, nbOutput );
	deltaproduct(productDelta, wHO, outputDelta, nbOutput, nbHidden+1);
	DeltaHidden(deltaHidden, productDelta, hidden, nbInput);
	newWeight(wHO , hidden, outputDelta, 0.3, nbOutput, nbHidden+1);
	newWeight(wIH , input, deltaHidden, 0.3, nbHidden, nbInput+1);
	}

	for (int i = 0 ; i < 4 ; ++i)
	{
	input[0] = inputs[i%3];
	input[1] = inputs[i*3+1];
	input[2] = inputs[i*3+2];
	product(input, wIH, hidden, nbInput, nbHidden+1);
	product(hidden, wHO, output, nbOutput, nbHidden+1);
	printf("%f - %f - %f  = ",input[0], input[1], input[2]);
	printf("%i - %i - %i", output[0]>0.5?1:0 , output[1]>0.5?1:0,
	output[2]>0.5?1:0);
	printf("\n");

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
*/
}

/*		printf("%i = ",(int)input[0]);
		for (int j = 0 ; j < 4; ++j)
		{
		printf("%f ", (outexpected[j]));
//	printf("%i", (output[j]>0.5)?1:0);
}
printf("\n");
 */
//}

/*===================End of training loop======================*/
/*	if (argc == 1)
	{
	for (int i = 0 ; i < 8 ; ++i)
	{
	input[0] = inputs[i%8];
	product(input, wIH, hidden, nbInput, nbHidden+1);
	product(hidden, wHO, output, nbOutput, nbHidden+1);
	printf("%i = ",(int)input[0]);
	for (int j = 0 ; j < 4; ++j)
	{
	printf("%f ", (output[j]));
//	printf("%i", (output[j]>0.5)?1:0);
}
printf("\n");

}*/

/*	input[0] = 1;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 1;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	((output[0]>0.5)?1:0));
 */
/*	}
	else
	{
	int x1, x2;
	x1 = atoi(argv[1]);
	x2 = atoi(argv[2]);

	input[0] = x1;
	input[1] = x2;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf(" \n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
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
	}*/

/*
	int main(int argc, char* argv[])
	{
	int nbInput = 2;
	int nbOutput = 1;
	int nbHidden = 2;
	float *input = calloc(nbInput , sizeof(float));
	float inputs[] = { 1, 1, 0, 0, 0, 1, 1, 0 };
	float outputs[] = { 0, 0, 1, 1 };
	float *outexpected = calloc(nbOutput , sizeof(float));
	float *output = calloc(nbOutput , sizeof(float));
	float *hidden = calloc(nbHidden , sizeof(float));
	float *wIH = calloc(nbHidden * (nbInput+1) , sizeof(float));

	float *wHO = calloc(nbOutput * (nbHidden+1) , sizeof(float));

	initWeight(wIH, nbHidden, nbInput+1);
	initWeight(wHO, nbOutput, nbHidden+1);
//		printf("wIH\n");
//		printMatrix(wIH, nbHidden, nbInput+1);
//		printf("wHO\n");
//		printMatrix(wHO, nbOutput, nbHidden+1);
float *outputDelta = calloc(nbOutput , sizeof(float));
float *productDelta = calloc(2 , sizeof(float));
float *deltaHidden = calloc( 2, sizeof(float));
 */
/*=======================Training loop======================*/
/*
	for (int t = 0 ; t < 100000 ; ++t)
	{

	input[0] = inputs[2*(t%4)];
	input[1] = inputs[2*(t%4)+1];
//				printf("imput\n");
//				printMatrix(input, 2,1);
outexpected[0] = outputs[t%4];
//				printf("outexpected\n");
//				printMatrix(outexpected, 1,1);
product(input, wIH, hidden, nbInput, nbInput+1);
//				printf("Hidden\n");
product(hidden, wHO, output, nbOutput, nbHidden+1);
//				printMatrix(hidden,2,1);
//				printf("output\n");
//				printMatrix(output, 1,1);
DeltaOutput(outputDelta, output, outexpected, nbOutput );
//				printf("Outputdelta\n");
//				printMatrix(outputDelta, 1,1);
deltaproduct(productDelta, wHO, outputDelta, nbOutput, nbHidden+1);
//				printf("ProductDelta\n");
//				printMatrix(productDelta, 1,2);
DeltaHidden(deltaHidden, productDelta, hidden, nbInput);
//				printf("deltaHidden");
//				printMatrix(deltaHidden, nbHidden, nbInput);
newWeight(wHO , hidden, outputDelta, 0.3, nbOutput, nbHidden+1);

newWeight(wIH , input, deltaHidden, 0.3, nbHidden, nbInput+1);

//				printf("wIH\n");
//				printMatrix(wIH, nbHidden, nbInput+1);
//				printf("wHO\n");
//				printMatrix(wHO, nbOutput, nbHidden+1);
}
 */	/*===================End of training loop======================*/
/*	if (argc == 1)
	{
	input[0] = 1;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 0;
	input[1] = 1;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
	(output[0]>0.5)?1:0);
	input[0] = 1;
	input[1] = 0;
	product(input, wIH, hidden, 2,3);
	product(hidden, wHO, output, 1, 3);
	printf("\n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
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
	printf(" \n%f XOR %f = %f\n",input[0],input[1],
	(output[0]));
	printf("Gives \n%i XOR %i = %i\n",(int)input[0],(int)input[1],
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
	}*/

