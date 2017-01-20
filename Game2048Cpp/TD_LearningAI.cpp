#include "TD_LearningAI.h"

namespace Game2048_AI_TD_Learning
{
	TD_LearningAI::TD_LearningAI(float learningRate, int& loadedCount) : LearningAI(learningRate)
	{
		tupleNetwork = new TupleNetwork("SuperNormalTD", 0);
		tupleNetwork->Load(loadedCount);
	}


	TD_LearningAI::~TD_LearningAI()
	{
		delete tupleNetwork;
	}
	void TD_LearningAI::UpdateEvaluation()
	{
		vector<BestMoveNode> bestMoveNodes = vector<BestMoveNode>(td_StateChain.size());
		for (unsigned int i = 0; i < td_StateChain.size(); i++)
		{
			BitBoard board = BitBoard(td_StateChain[i].insertedRawBlocks);
			Direction nextDirection = GetBestMove(board);
			int nextReward = 0;

			bestMoveNodes[i].bestMove = nextDirection;
			bestMoveNodes[i].rawBlocks = board.RawBlocks();
			bestMoveNodes[i].movedRawBlocks = board.MoveRaw(nextDirection, nextReward);
			bestMoveNodes[i].reward = (float)nextReward;
		}
		for (int i = td_StateChain.size() - 1; i >= 0; i--)
		{
			float score = bestMoveNodes[i].reward + tupleNetwork->GetValue(bestMoveNodes[i].movedRawBlocks);
			if (i == td_StateChain.size() - 1 && bestMoveNodes[i].rawBlocks == bestMoveNodes[i].movedRawBlocks)
			{
				score = 0;
			}
			tupleNetwork->UpdateValue(td_StateChain[i].movedRawBlocks, learningRate * (score - tupleNetwork->GetValue(td_StateChain[i].movedRawBlocks)));
		}
	}
	void TD_LearningAI::Save()
	{

	}
	Game TD_LearningAI::Train()
	{
		Game game = PlayGame();
		UpdateEvaluation();
		td_StateChain.clear();
		rawBlocksRecord.clear();

		return game;
	}
	Game TD_LearningAI::RewardTrain(bool isTrainedByScore, float previousAverage, float previousDeviation)
	{
		return Game();
	}
	float TD_LearningAI::Evaluate(BitBoard board, Direction direction)
	{
		if (board.MoveCheck(direction))
		{
			int result;
			BitBoard boardAfter = board.Move(direction, result);
			unsigned long long rawBoard = boardAfter.RawBlocks();
			boardAfter.InsertNewTile();
			if (boardAfter.CanMove())
				return (result + tupleNetwork->GetValue(rawBoard));
			else
				return -1;
		}
		else
		{
			return 0;
		}
	}
}
