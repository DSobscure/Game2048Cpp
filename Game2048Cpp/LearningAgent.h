#pragma once
#include "LearningAI.h"
#include "TranningMode.h"
#include <vector>
#include "GameRecord.h"
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

namespace Game2048_AI_TD_Learning
{
	class LearningAgent
	{
	public:
		LearningAgent(LearningAI* learningAI);
		~LearningAgent();
		void Training(string logName, TranningMode mode, int startRound, int trainingTimes, int recordSize, void (*printBoardFunction)(BitBoard));
	private:
		LearningAI* learningAI;
	};
}

