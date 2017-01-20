#pragma once
#include "LearningAI.h"
#include "TupleNetwork.h"
#include "BestMoveNode.h"

namespace Game2048_AI_TD_Learning
{
	class TD_LearningAI : public LearningAI
	{
	public:
		TD_LearningAI(float learningRate, int& loadedCount);
		~TD_LearningAI();
		void UpdateEvaluation();
		void Save();
		Game Train();
		Game RewardTrain(bool isTrainedByScore, float previousAverage = 0, float previousDeviation = 0);
	protected:
		float Evaluate(BitBoard board, Direction direction);
		
	private:
		TupleNetwork* tupleNetwork;
	};
}

