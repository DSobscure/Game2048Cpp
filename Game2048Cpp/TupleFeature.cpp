#include "TupleFeature.h"

namespace Game2048_AI_TD_Learning
{
	TupleFeature::TupleFeature()
	{
	}


	TupleFeature::~TupleFeature()
	{
	}

	void TupleFeature::UpdateScore(unsigned long long rawBlocks, float delta)
	{
		for (int i = 0; i < 4; i++)
		{
			int index = GetIndex(rotateBoards[i]);
			int symmetricIndex = GetIndex(GetMirrorSymmetricRawBlocks(rotateBoards[i]));

			tuples[index] += delta;
			if (symmetricIndex != index)
				tuples[symmetricIndex] += delta;
		}
	}

	float TupleFeature::GetScore(unsigned long long blocks)
	{
		float sum = 0;
		for (int i = 0; i < 4; i++)
		{
			int index = GetIndex(rotateBoards[i]);
			int symmetricIndex = GetIndex(GetMirrorSymmetricRawBlocks(rotateBoards[i]));

			sum += tuples[index];
			if (symmetricIndex != index)
				sum += tuples[symmetricIndex];
		}
		return sum;
	}

	void TupleFeature::SetSymmetricBoards(vector<unsigned long long> rotateSymmetry)
	{
		rotateBoards = rotateSymmetry;
	}
	unsigned long long TupleFeature::GetMirrorSymmetricRawBlocks(unsigned long long rawBlocks)
	{
		unsigned short reversedRowContents[4];

		for (int i = 0; i < 4; i++)
		{
			reversedRowContents[i] = BitBoardOperationSet.ReverseRow(BitBoardOperationSet.GetRow(rawBlocks, i));
		}

		return BitBoardOperationSet.SetRows(reversedRowContents); ;
	}

	TupleFeature::TupleFeature(vector<float> tuples)
	{
		this->tuples = tuples;
	}
	TupleFeature::TupleFeature(int tupleNumber)
	{
		tuples = vector<float>((int)pow(16, tupleNumber));
	}
}

