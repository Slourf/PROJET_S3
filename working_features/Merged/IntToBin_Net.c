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
		int f = 0;
		for (size_t i = 0; i < len; i++)
		{
			f = fscanf(file, "%f", (b+i));
		}
		if (f)
			fclose(file);
		else
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

/*This function compute the delta value for the output layer*/

void DeltaOutput(float *dst,float *Actual, float *Expect, int l)
{
	for (int j = 0 ; j < l ; ++j)
	{
		dst[j] = (Actual[j] - Expect[j])*
			Actual[j] * (1 - Actual[j]);
	}
}


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



/*This function compute the delta value for the hidden layer*/

void DeltaHidden(float *dst, float *delta, float *hidden, int l)
{
	for (int i = 0; i < l; ++i)
	{
		dst[i] = hidden[i] * (1 - hidden[i])* delta[i];
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
			W[r*i + j] =W[r*i+j]- ( lr * Delta[j] * M[i-1]);

		}
	}
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

/*
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
*/

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
//	writeFileChar("Out.txt",carac, nbChar);
	free(input);
	free(output);
	free(hidden);
	free(wIH);
	free(wHO);
	free(result);
	return carac;

}
