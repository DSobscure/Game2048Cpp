#include "MultiLayerPerceptron.h"

namespace Game2048_AI_NeuralNetwork
{
	MultiLayerPerceptron::MultiLayerPerceptron()
	{
	}
	MultiLayerPerceptron::MultiLayerPerceptron(int inputDimension, int outputDimension, int hiddenLayerNumber, vector<int> hiddenLayerNodeNumber, float learningRate)
	{
		this->inputDimension = inputDimension;
		this->outputDimension = outputDimension;
		this->hiddenLayerNumber = hiddenLayerNumber;
		this->hiddenLayerNodeNumber = hiddenLayerNodeNumber;

		weights = vector< vector< vector<float> > >(hiddenLayerNumber + 1);
		int perviousLayerDimension = inputDimension;
		for (int i = 0; i < hiddenLayerNumber; i++)
		{
			weights[i] = vector< vector<float> >(hiddenLayerNodeNumber[i]);
			for (int j = 0; j < hiddenLayerNodeNumber[i]; j++)
			{
				weights[i][j] = vector<float>(perviousLayerDimension + 1);
				for (int k = 0; k < perviousLayerDimension + 1; k++)
				{
					weights[i][j][k] = (float)(((float)rand() / (RAND_MAX)) - 0.5f);
				}
			}
			perviousLayerDimension = hiddenLayerNodeNumber[i];
		}
		weights[hiddenLayerNumber] = vector< vector<float> >(outputDimension);
		for (int i = 0; i < outputDimension; i++)
		{
			weights[hiddenLayerNumber][i] = vector<float>(perviousLayerDimension + 1);
			for (int j = 0; j < perviousLayerDimension + 1; j++)
			{
				weights[hiddenLayerNumber][i][j] = (float)(((float)rand() / (RAND_MAX)) - 0.5f);
			}
		}
		this->learningRate = learningRate;
	}

	MultiLayerPerceptron::~MultiLayerPerceptron()
	{

	}

	int MultiLayerPerceptron::HiddenLayerNodeCount(int layer)
	{
		return hiddenLayerNodeNumber[layer];
	}

	vector<float> MultiLayerPerceptron::Compute(vector<float> input)
	{
		vector<float> output;
		int perviousLayerDimension = inputDimension;
		for (int layerIndex = 0; layerIndex < hiddenLayerNumber; layerIndex++)
		{
			int weightsLength = hiddenLayerNodeNumber[layerIndex];
			output = vector<float>(weightsLength);
			
			for (int nodeIndex = 0; nodeIndex < weightsLength; nodeIndex++)
			{
				vector<float> weightsPointer = weights[layerIndex][nodeIndex];
				{
					float sum = weightsPointer[perviousLayerDimension];
					for (int weightsIndex = 0; weightsIndex != perviousLayerDimension; weightsIndex++)
					{
						sum += weightsPointer[weightsIndex] * input[weightsIndex];
					}
					output[nodeIndex] = ActivationFunction(sum);
				}
			}
			perviousLayerDimension = weightsLength;
			input = output;
		}
		output = vector<float>(outputDimension);
		for (int nodeIndex = 0; nodeIndex < outputDimension; nodeIndex++)
		{
			vector<float> weightsPointer = weights[hiddenLayerNumber][nodeIndex];
			{
				float sum = weightsPointer[perviousLayerDimension];
				for (int weightsIndex = 0; weightsIndex != perviousLayerDimension; weightsIndex++)
				{
					sum += weightsPointer[weightsIndex] * input[weightsIndex];
				}
				output[nodeIndex] = ActivationFunction(sum);
			}
		}
		return output;
	}
	void MultiLayerPerceptron::Tranning(vector<float> input, vector<float> desiredOutput, float& error)
	{
		vector< vector<float> > nodeSums = vector< vector<float> >(hiddenLayerNumber + 1);
		vector< vector<float> > nodeDeltas = vector< vector<float> >(hiddenLayerNumber + 1);
		vector<float> output;
		vector< vector<float> > layerInput = vector< vector<float> >(hiddenLayerNumber + 1);

		int perviousLayerDimension = inputDimension;
		for (int layerIndex = 0; layerIndex < hiddenLayerNumber; layerIndex++)
		{
			int nodeCount = hiddenLayerNodeNumber[layerIndex];
			for (int i = 0; i < nodeCount; i++)
			{
				layerInput[layerIndex][i] = input[i];
			}
			layerInput[layerIndex][nodeCount] = 1;
			
			output = vector<float>(nodeCount);
			nodeSums[layerIndex] = vector<float>(nodeCount);
			nodeDeltas[layerIndex] = vector<float>(nodeCount);
			vector<float> nodeSumsPointer = nodeSums[layerIndex];
			for (int nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
			{
				vector<float> weightsPointer = weights[layerIndex][nodeIndex];
				{
					float sum = weightsPointer[perviousLayerDimension];
					for (int weightsIndex = 0; weightsIndex < perviousLayerDimension; weightsIndex++)
					{
						sum += weightsPointer[weightsIndex] * input[weightsIndex];
					}
					nodeSumsPointer[nodeIndex] = sum;
					output[nodeIndex] = ActivationFunction(sum);
				}
			}
			perviousLayerDimension = nodeCount;
			input = output;
		}

		vector<float> argumentInputVectorFinal = input;
		argumentInputVectorFinal.push_back(1);

		layerInput[hiddenLayerNumber] = argumentInputVectorFinal;
		output = vector<float>(outputDimension);
		nodeSums[hiddenLayerNumber] = vector<float>(outputDimension);
		nodeDeltas[hiddenLayerNumber] = vector<float>(outputDimension);
		for (int nodeIndex = 0; nodeIndex < outputDimension; nodeIndex++)
		{
			vector<float> weightsPointer = weights[hiddenLayerNumber][nodeIndex];
			{
				float sum = weightsPointer[perviousLayerDimension];
				vector<float> inputPointer = input;
				for (int weightsIndex = 0; weightsIndex < perviousLayerDimension; weightsIndex++)
				{
					sum += weightsPointer[weightsIndex] * inputPointer[weightsIndex];
				}
				nodeSums[hiddenLayerNumber][nodeIndex] = sum;
				output[nodeIndex] = ActivationFunction(sum);
			}
		}

		error = 0;
		for (int nodeIndex = 0; nodeIndex < outputDimension; nodeIndex++)
		{
			error += (desiredOutput[nodeIndex] - output[nodeIndex]) * (desiredOutput[nodeIndex] - output[nodeIndex]);
			nodeDeltas[hiddenLayerNumber][nodeIndex] = (desiredOutput[nodeIndex] - output[nodeIndex]) * dActivationFunction(nodeSums[hiddenLayerNumber][nodeIndex]);
		}
		error = (float)sqrt(error);
		for (int layerIndex = hiddenLayerNumber - 1; layerIndex >= 0; layerIndex--)
		{
			vector<float> nodeDeltasCache = nodeDeltas[layerIndex];
			vector<float> nodeSumsCache = nodeSums[layerIndex];
			vector< vector<float> > weightsCache = weights[layerIndex + 1];
			for (int nodeIndex = 0; nodeIndex < hiddenLayerNodeNumber[layerIndex]; nodeIndex++)
			{
				float deltaSum = 0;
				int weightsLength = sizeof(weights[layerIndex + 1])/ sizeof(weights[layerIndex + 1][0]);
				vector<float> nodeDeltasPointer = nodeDeltas[layerIndex + 1];
				{
					for (int previousLayerNodeIndex = 0; previousLayerNodeIndex != weightsLength; previousLayerNodeIndex++)
					{
						deltaSum += nodeDeltasPointer[previousLayerNodeIndex] * weightsCache[previousLayerNodeIndex][nodeIndex];
					}
				}
				nodeDeltasCache[nodeIndex] = deltaSum * dActivationFunction(nodeSumsCache[nodeIndex]);
			}
		}

		for (int nodeIndex = 0; nodeIndex < outputDimension; nodeIndex++)
		{
			int weightLength = sizeof(weights[hiddenLayerNumber][nodeIndex])/ sizeof(weights[hiddenLayerNumber][nodeIndex][0]);
			vector<float> weightsPointer = weights[hiddenLayerNumber][nodeIndex];
			{
				for (int weightsIndex = 0; weightsIndex < weightLength; weightsIndex++)
				{
					weightsPointer[weightsIndex] += learningRate * nodeDeltas[hiddenLayerNumber][nodeIndex] * layerInput[hiddenLayerNumber][weightsIndex];
				}
			}
		}
		for (int layerIndex = hiddenLayerNumber - 1; layerIndex >= 0; layerIndex--)
		{
			int nodeLength = hiddenLayerNodeNumber[layerIndex];
			for (int nodeIndex = 0; nodeIndex < nodeLength; nodeIndex++)
			{
				int weightLength = sizeof(weights[layerIndex][nodeIndex])/ sizeof(weights[layerIndex][nodeIndex][0]);
				float nodeDelta = nodeDeltas[layerIndex][nodeIndex];

				vector<float> weightsPointer = weights[layerIndex][nodeIndex];
				vector<float> layerInputPointer = layerInput[layerIndex];
				{
					for (int weightsIndex = 0; weightsIndex < weightLength; weightsIndex++)
					{
						weightsPointer[weightsIndex] += learningRate * nodeDelta * layerInputPointer[weightsIndex];
					}
				}
			}
		}
	}

	float MultiLayerPerceptron::ActivationFunction(float x)
	{
		if (x > 0)
		{
			return x;
		}
		else
		{
			return 0;
		}
	}
	float MultiLayerPerceptron::dActivationFunction(float x)
	{
		if (x > 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
