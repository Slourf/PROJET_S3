# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <assert.h>
# include <err.h>


/*========== Read&Write matrix into file =========*/

/*----------Save a matrix into a file------------*/
void writeFile(char* path, float *a, size_t len)
{
    FILE* file = fopen(path, "w");
    if (file != NULL)
    {
        for (size_t i = 0; i < len; i++)
        {
            fprintf(file, "%f\n", *a);
            a++;
        }
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
             ++b;
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
                float rnd = ((float)rand()/(double)RAND_MAX);
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



int main(int argc, char* argv[])
{
	//int nbInput = nb_pixel;
	//int nbOutput = 1;
	//int nbHidden = nb_pixel;
	float *input = malloc(nbInput * sizeof(float));
	float *output = malloc(nbOutput * sizeof(float));
	float *hidden = malloc(nbOutput * sizeof(float));
	float *wIH = malloc(nbHidden * (nbInput + 1) * sizeof(float));
	float *wHO = nalloc(nbOutput * (nbHidden + 1) * sizeof(float));

	/*initWeight is called once*/
	initWeight(wIH, nbHidden, nbInput + 1);
	initWeight(wHO, nbOutput, nbHidden + 1);
	/*Save matrix into file*/
	/*writeFile is called everytime the net learn*/
	writeFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
	weiteFile("weightHO.txt"., wHO, nbOutput * (nbHidden + 1));

	readFile("weightIH.txt", wIH, nbHidden * (nbInput + 1));
	readFile("weightHO.txt"., wHO, nbOutput * (nbHidden + 1));

	/*Then forword propagation*/







}
