#include "LearningAI.h"

namespace Game2048_AI_TD_Learning
{
	LearningAI::LearningAI(float learningRate)
	{
		this->learningRate = learningRate;
	}
	LearningAI::~LearningAI()
	{

	}

	Game LearningAI::PlayGame()
	{
		Game game;
		while (!game.IsEnd())
		{
			unsigned long long movedRawBlocks = game.Move(GetBestMove(game.Board(), 1));
			unsigned long long blocksAfterAdded = game.Board().RawBlocks();
			TD_State state;
			state.movedRawBlocks = movedRawBlocks;
			state.insertedRawBlocks = blocksAfterAdded;
			td_StateChain.push_back(state);
			rawBlocksRecord.push_back(blocksAfterAdded);
		}
		return game;
	}
	float LearningAI::MutiStepEvaluate(BitBoard board, Direction direction, int maxStep)
	{
		float boardValue = Evaluate(board, direction);
		if (maxStep == 1 || boardValue < 0)
		{
			return boardValue;
		}
		else
		{
			int reward;
			BitBoard searchingBoard = board.Move(direction, reward);
			searchingBoard.InsertNewTile();
			float maxScore = 0;

			bool isFirst = true;
			for (Direction searchingDirection = Direction::Up; searchingDirection <= Direction::Right; searchingDirection = (Direction)(searchingDirection + 1))
			{
				float result = MutiStepEvaluate(searchingBoard, searchingDirection, maxStep - 1);
				if (isFirst && searchingBoard.MoveCheck(searchingDirection))
				{
					maxScore = result;
					isFirst = false;
				}
				else if (result > maxScore && searchingBoard.MoveCheck(searchingDirection))
				{
					maxScore = result;
				}
			}
			return boardValue + maxScore;
		}
	}
	Direction LearningAI::GetBestMove(BitBoard board, int initialStep)
	{
		Direction nextDirection = Direction::No;
		float maxScore =  -FLT_MAX;

		for (Direction direction = Direction::Up; direction <= Direction::Right; direction = (Direction)(direction + 1))
		{
			float result = MutiStepEvaluate(board, direction, initialStep);
			if (result > maxScore && board.MoveCheck(direction))
			{
				nextDirection = direction;
				maxScore = result;
			}
		}
		return nextDirection;
	}
}
