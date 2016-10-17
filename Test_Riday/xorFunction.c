# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <math.h> //for exp

#define NUM_TRAINING 4
#define NUM_INPUTS 2
#define NUM_HIDDEN 2
#define NUM_OUTPUTS 1

 void displayVectorOrMatrix (const char *label, float *m, int rows, int cols)     {
      printf ("   %s:\n", label);
      for (int i=0; i<rows*cols; i++) printf ("%10.5f%c", m[i], (cols>1 && i%c    ols==cols-1) ? '\n' : ' ');
      if (cols==1) printf ("\n");
  }
  


int main (int argc, char **argv)
{

	float inputs[NUM_TRAINING][NUM_INPUTS] = 
	{
	{0, 0},
	{0, 1},
	{1, 0},
	{1, 1}
		 };
	float outputs[NUM_TRAINING][NUM_OUTPUTS] = 
	{
	{0},
	{1},
	{1},
	{0}
		};
	
	float learningrate = 0.01;
	int iterations = 10000;
	
	//initialize the weights 
	float Wxh[NUM_INPUTS+1][NUM_HIDDEN];
	float Why[NUM_HIDDEN+1][NUM_OUTPUTS];
	for (int i = 0; i < NUM_INPUTS+1; i++)
	{
		for (int h = 0; h < NUM_HIDDEN; h++)
			Wxh[i][h] = ((float)rand() / (double)RAND_MAX) * 0.2 - 0.1;
	}
	
	for (int h = 0; h < NUM_HIDDEN + 1; h++)
	{
		for (int o = 0; o < NUM_OUTPUTS; o++)
			Why[h][o] = ((float)rand() / (double)RAND_MAX) * 0.2 - 0.1;
	}
	
	float x[NUM_INPUTS+1]; //input vector including one extra for bias
	float y[NUM_OUTPUTS]; //output vector
	float zhWeightedSums[NUM_HIDDEN]; 
	//weighted sums for the hidden nodes
	float hActivationValues[NUM_HIDDEN+1]; 
	//activation values of the hidden nodes, 
	//including one extra for the bias
	float zyWeightedSums[NUM_OUTPUTS]; 
	//weighted sums for the output nodes
	float probabilities[NUM_OUTPUTS]; 
	//activation values of the output nodes
	float outputErrors[NUM_OUTPUTS];
	 //error in the output
	float deltaWxh[NUM_INPUTS+1][NUM_HIDDEN];
	 //adjustments to weights between inputs x and hidden nodes
	float deltaWhy[NUM_HIDDEN+1][NUM_OUTPUTS]; 
	//adjustments to weights between hidden nodes and output y
	float loss, sum; 
	//for storing the loss

	//Training Loop
	for (int t=0; t<iterations; t++) 
	{ //this is how many times we will train the network
		x[0]=1;
		 memcpy(&x[1], inputs[t % NUM_TRAINING], sizeof (inputs[0])); 
		//copy the input into x with the bias=1
		memcpy (y, outputs[t % NUM_TRAINING], sizeof (outputs[0])); 
		//copy the output into y
	}

	//weighted sums for hidden nodes 
	memset (zhWeightedSums, 0, sizeof (zhWeightedSums));
	for (int h = 0; h < NUM_HIDDEN; h++)
	{
		for (int i = 0; i < NUM_INPUTS + 1; i++)
			zhWeightedSums[h] += x[i] * Wxh[i][h];
			 //multiply and sum inputs * weights
	}
	
	//Activation value 
	hActivationValues[0]=1;
	 //set the bias for the first hidden node to 1
	for (int h=0; h<NUM_HIDDEN; h++) 
	{	
		hActivationValues[h+1] = tanh (zhWeightedSums[h]); 
		//apply activation function on other hidden nodes
	}
	
	//weighted sums for input nodes 
	memset (zyWeightedSums, 0, sizeof (zyWeightedSums));
	 //set all the weighted sums to zero
	//multiply and sum inputs * weights
	for (int o=0; o<NUM_OUTPUTS; o++) 
	{
		for (int h=0; h<NUM_HIDDEN+1; h++) 
		{
			zyWeightedSums[o] += hActivationValues[h] * Why[h][o];
		}
	}
	
	//Probable answer 
	for (int sum=0, o=0; o<NUM_OUTPUTS; o++)
	 {
		probabilities[o] = exp (zyWeightedSums[o]); 
		sum += probabilities[o];
	 //compute exp(z) for softmax
	}
	for (int o=0; o<NUM_OUTPUTS; o++) 
	{	
		probabilities[o] /= sum;
		 //apply softmax by dividing by the the sum all the exps
	}

	//OutPut error
	for (int o=0; o<NUM_OUTPUTS; o++) 
	{
		outputErrors[o] = probabilities[o] - y[o];
		 //the error for each output	
	}

	//Loss
	for (int loss=0, o=0; o<NUM_OUTPUTS; o++) 
	{
		loss -= y[o] * log (probabilities[o]);
	}
	
	//Finding the gradient 
	for (int h=0; h<NUM_HIDDEN+1; h++) 
	{
		for (int o=0; o<NUM_OUTPUTS; o++) 
		{
			deltaWhy[h][o] = hActivationValues[h] * outputErrors[o];
		}
	}

	//Backward propagation
	memset (hActivationValues, 0, sizeof (hActivationValues)); 
	//set all the weighted sums to zero
	for (int h=1; h<NUM_HIDDEN+1; h++) 
	{
		for (int o=0; o<NUM_OUTPUTS; o++)
		{
			 hActivationValues[h] += Why[h][o] * outputErrors[o];
		}
	}

	//apply activation function gradient
	for (int h=0; h<NUM_HIDDEN; h++)
	{
		 zhWeightedSums[h] = hActivationValues[h+1] * 
							(1 - pow (tanh (zhWeightedSums[h]), 2));
	}

	//Multiply x*eh*zh to get the adjustments to deltaWxh, 
	//this does not include the bias node
	for (int i=0; i<NUM_INPUTS+1; i++)
	{
		 for (int h=0; h<NUM_HIDDEN; h++) 
		{
			deltaWxh[i][h] = x[i] * zhWeightedSums[h];
		}
	}
	
	//Changing the weights 
	for (int h=0; h<NUM_HIDDEN+1; h++) 
	{
		for (int o=0; o<NUM_OUTPUTS; o++) 
		{
			Why[h][o] -= learningrate * deltaWhy[h][o];
		}
	}
	for (int i=0; i<NUM_INPUTS+1; i++) 
	{
		for (int h=0; h<NUM_HIDDEN; h++)
		{
			 Wxh[i][h] -= learningrate * deltaWxh[i][h];
		}
	}
//Output weights and last input/output and predicted output
     displayVectorOrMatrix ("input/hidden weights", (float*)Wxh, NUM_INPUTS+1    , NUM_HIDDEN);
     displayVectorOrMatrix ("hidden/output weights", (float*)Why, NUM_HIDDEN+    1, NUM_OUTPUTS);
     displayVectorOrMatrix ("last input", &x[1], NUM_INPUTS, 1);
     displayVectorOrMatrix ("last output", y, NUM_OUTPUTS, 1);
     displayVectorOrMatrix ("predicted output", probabilities, NUM_OUTPUTS, 1    );

	return 0;
}
