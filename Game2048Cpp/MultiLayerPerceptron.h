#pragma once
#include <random>
#include <math.h>
#include <vector>

using namespace std;

namespace Game2048_AI_NeuralNetwork
{
	class MultiLayerPerceptron
	{
	public:
		MultiLayerPerceptron();
		MultiLayerPerceptron(int inputDimension, int outputDimension, int hiddenLayerNumber, vector<int> hiddenLayerNodeNumber, float learningRate);
		~MultiLayerPerceptron();
		int inputDimension;
		int outputDimension;
		int hiddenLayerNumber;
		vector<int> hiddenLayerNodeNumber;
	
		float learningRate;

		
		int HiddenLayerNodeCount(int layer);

		vector<float> Compute(vector<float> input);
		void Tranning(vector<float> input, vector<float> desiredOutput, float& error);
	private:
		vector< vector< vector<float> > > weights;
		float ActivationFunction(float x);
		float dActivationFunction(float x);
	};
}

