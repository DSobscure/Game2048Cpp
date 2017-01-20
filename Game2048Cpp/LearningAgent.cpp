#include "LearningAgent.h"

namespace Game2048_AI_TD_Learning
{
	LearningAgent::LearningAgent(LearningAI* learningAI)
	{
		this->learningAI = learningAI;
	}


	LearningAgent::~LearningAgent()
	{
	}

	void LearningAgent::Training(string logName, TranningMode mode, int startRound, int trainingTimes, int recordSize, void(*printBoardFunction)(BitBoard))
	{
		vector<float> scores;
		vector<int> steps;
		vector<GameRecord> records;

		int maxScore = 0, minScore = INT32_MAX;
		int maxTile = 0, minTile = 65536;
		int maxCount = 0, minCount = 0;
		int maxStep = 0, minStep = INT32_MAX;
		int win128Count = 0, win256Count = 0, win512Count = 0, win1024Count = 0, win2048Count = 0, win4096Count = 0, win8192Count = 0, win16384Count = 0;
		BitBoard minScoreBoard, maxScoreBoard, minStepBoard, maxStepBoard;

		float totalSecond = 0;

		float previousAverageScore = 0;
		float previousScoreDeviation = 0;
		float previousAverageStep = 0;
		float previousStepDeviation = 0;

		clock_t startTime = clock();

		for (int i = 1; i <= trainingTimes; i++)
		{
			Game game;
			switch (mode)
			{
				case TranningMode::Classical:
					game = learningAI->Train();
					break;
				case TranningMode::ScoreTrainedReward:
					game = learningAI->RewardTrain(true, previousAverageScore, previousScoreDeviation);
					break;
				case TranningMode::StepTrainedReward:
					game = learningAI->RewardTrain(false, previousAverageStep, previousStepDeviation);
					break;
				default:
					cout << "NotSupported TranningMode" << endl;
					break;
			}
			scores.push_back((float)game.Score());
			steps.push_back(game.Step());

			win128Count += (game.Board().MaxTile() >= 128) ? 1 : 0;
			win256Count += (game.Board().MaxTile() >= 256) ? 1 : 0;
			win512Count += (game.Board().MaxTile() >= 512) ? 1 : 0;
			win1024Count += (game.Board().MaxTile() >= 1024) ? 1 : 0;
			win2048Count += (game.Board().MaxTile() >= 2048) ? 1 : 0;
			win4096Count += (game.Board().MaxTile() >= 4096) ? 1 : 0;
			win8192Count += (game.Board().MaxTile() >= 8192) ? 1 : 0;
			win16384Count += (game.Board().MaxTile() >= 16384) ? 1 : 0;

			if (game.Score() > maxScore)
			{
				maxScore = game.Score();
				maxScoreBoard = BitBoard(game.Board().RawBlocks());
			}
			if (game.Score() < minScore)
			{
				minScore = game.Score();
				minScoreBoard = BitBoard(game.Board().RawBlocks());
			}
			if (game.Step() > maxStep)
			{
				maxStep = game.Step();
				maxStepBoard = BitBoard(game.Board().RawBlocks());
			}
			if (game.Step() < minStep)
			{
				minStep = game.Step();
				minStepBoard = BitBoard(game.Board().RawBlocks());
			}

			if (game.Board().MaxTile() > maxTile)
			{
				maxTile = game.Board().MaxTile();
				maxCount = 1;
			}
			else if (game.Board().MaxTile() == maxTile)
			{
				maxCount++;
			}

			if (game.Board().MaxTile() < minTile)
			{
				minTile = game.Board().MaxTile();
				minCount = 1;
			}
			else if (game.Board().MaxTile() == minTile)
			{
				minCount++;
			}

			if (i % recordSize == 0)
			{
				totalSecond = (clock() - startTime) / CLOCKS_PER_SEC;
				startTime = clock();
				float totalScore = 0;
				for (unsigned int i = 0; i < scores.size(); i++)
				{
					totalScore += scores[i];
				}
				float totalStep = 0;
				for (unsigned int i = 0; i < steps.size(); i++)
				{
					totalStep += steps[i];
				}

				previousAverageScore = totalScore / recordSize;
				previousAverageStep = totalStep / recordSize;

				float scoreDeviationSum = 0;
				for (unsigned int i = 0; i < scores.size(); i++)
				{
					scoreDeviationSum += pow(scores[i] - previousAverageScore, 2);
				}

				float stepDeviationSum = 0;
				for (unsigned int i = 0; i < scores.size(); i++)
				{
					stepDeviationSum += pow(steps[i] - previousAverageStep, 2);
				}
				float scoreDeviation = sqrt(scoreDeviationSum / recordSize);
				float stepDeviation = sqrt(stepDeviationSum / recordSize);

				
				previousScoreDeviation = scoreDeviation;
				previousStepDeviation = stepDeviation;

				GameRecord record;
				record.startRound = startRound;
				record.roundSize = recordSize;
				record.round = i;
				record.averageScore = previousAverageScore;
				record.averageStep = previousAverageStep;
				record.minScore = minScore;
				record.maxStep = maxStep;
				record.minStep = minStep;
				record.winRate128 = win128Count / (float)recordSize;
				record.winRate256 = win256Count / (float)recordSize;
				record.winRate512 = win512Count / (float)recordSize;
				record.winRate1024 = win1024Count / (float)recordSize;
				record.winRate2048 = win2048Count / (float)recordSize;
				record.winRate4096 = win4096Count / (float)recordSize;
				record.winRate8192 = win8192Count / (float)recordSize;
				record.winRate16384 = win16384Count / (float)recordSize;
				record.scoreDeviation = scoreDeviation;
				record.stepDeviation = stepDeviation;
				record.deltaTime = totalSecond;
				record.averageSpeed = totalStep / totalSecond;
				record.rawMaxScoreBoard = maxScoreBoard.RawBlocks();
				record.rawMinScoreBoard = minScoreBoard.RawBlocks();
				record.rawMaxStepBoard = maxStepBoard.RawBlocks();
				record.rawMinStepBoard = minStepBoard.RawBlocks();

				records.push_back(record);
				cout << "Round: " << i << " AvgScore: " << previousAverageScore << " - Deviation: " << scoreDeviation << ", AverageStep: " << previousAverageStep <<" - Deviation: "<< stepDeviation << endl;
				cout << "Max Score: " << maxScore << ", Min Score: " << minScore << endl;
				cout << "Max Steps: " << maxStep << ", Min Steps: " << minStep << endl;
				cout << "128WinRate: " << (win128Count / (float)recordSize) << endl;
				cout << "256WinRate: " << (win256Count / (float)recordSize) << endl;
				cout << "512WinRate: " << (win512Count / (float)recordSize) << endl;
				cout << "1024WinRate: " << (win1024Count / (float)recordSize) << endl;
				cout << "2048WinRate: " << (win2048Count / (float)recordSize) << endl;
				cout << "4096WinRate: " << (win4096Count / (float)recordSize) << endl;
				cout << "8192WinRate: " << (win8192Count / (float)recordSize) << endl;
				cout << "16384WinRate: " << (win16384Count / (float)recordSize) << endl;
				cout << "Max Tile: " << maxTile <<" #" << maxCount << ", Min Tile: " << minTile << " #" << minCount << endl;
				cout << "Delta Time: " << totalSecond <<" seconds" << endl;
				cout << "Average Speed: "<< (totalStep / totalSecond) <<"moves/sec" << endl;
				cout << endl;;

				printBoardFunction(minScoreBoard);
				printBoardFunction(maxScoreBoard);

				win128Count = 0;
				win256Count = 0;
				win512Count = 0;
				win1024Count = 0;
				win2048Count = 0;
				win4096Count = 0;
				win8192Count = 0;
				win16384Count = 0;
				maxTile = 0;
				minTile = 65536;
				maxCount = 0;
				minCount = 0;
				maxStep = 0;
				minStep = INT32_MAX;
				totalSecond = 0;
				maxScore = 0;
				minScore = INT32_MAX;
				scores.clear();
				steps.clear();
				minScoreBoard = BitBoard();
				maxScoreBoard = BitBoard();
				minStepBoard = BitBoard();
				maxStepBoard = BitBoard();
				if (i % 20000 == 0)
				{
					learningAI->Save();
					//File.WriteAllBytes(logName, SerializationHelper.Serialize(records));
				}
			}
		}
	}
}
