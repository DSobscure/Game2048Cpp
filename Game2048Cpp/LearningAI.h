#pragma once
#include <vector>
#include "TD_State.h"
#include "Game.h"

using namespace std;
using namespace Game2048_Game_Library;

namespace Game2048_AI_TD_Learning
{
	class LearningAI
	{
	public:
		LearningAI(float learningRate);
		~LearningAI();
		virtual Game Train() = 0;
		virtual Game RewardTrain(bool isTrainedByScore, float previousAverage, float previousDeviation) = 0;
		Game PlayGame();
		virtual void Save() = 0;
		float MutiStepEvaluate(BitBoard board, Direction direction, int maxStep);
	protected:
		float learningRate;
		vector<TD_State> td_StateChain;
		vector<unsigned long long> rawBlocksRecord;

		Direction GetBestMove(BitBoard board, int initialStep = 1);
		virtual float Evaluate(BitBoard board, Direction direction) = 0;
	};
}

