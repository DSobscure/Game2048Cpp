#pragma once
#include <vector>
#include "BitBoardTableSet.h"
#include <math.h>

using namespace std;
using namespace Game2048_Game_Library;

namespace Game2048_AI_TD_Learning
{
	class TupleFeature
	{
	public:
		TupleFeature();
		~TupleFeature();
		vector<unsigned long long> rotateBoards;

		void UpdateScore(unsigned long long rawBlocks, float delta);
		float GetScore(unsigned long long blocks);
		virtual int GetIndex(unsigned long long blocks) = 0;

		
		void SetSymmetricBoards(vector<unsigned long long> rotateSymmetry);
		unsigned long long GetMirrorSymmetricRawBlocks(unsigned long long rawBlocks);
	protected:
		vector<float> tuples;

		TupleFeature(vector<float> tuples);
		TupleFeature(int tupleNumber);
	};
}

