#pragma once

namespace Game2048_AI_TD_Learning
{
	class GameRecord
	{
	public:
		int startRound;
		int roundSize;
		int round;
		float averageScore;
		float averageStep;
		int maxScore;
		int minScore;
		int maxStep;
		int minStep;
		float winRate128;
		float winRate256;
		float winRate512;
		float winRate1024;
		float winRate2048;
		float winRate4096;
		float winRate8192;
		float winRate16384;
		float scoreDeviation;
		float stepDeviation;
		float deltaTime;
		float averageSpeed;
		unsigned long long rawMaxScoreBoard;
		unsigned long long rawMinScoreBoard;
		unsigned long long rawMaxStepBoard;
		unsigned long long rawMinStepBoard;
	};
}
