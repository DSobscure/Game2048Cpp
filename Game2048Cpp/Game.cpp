#include "Game.h"

namespace Game2048_Game_Library
{
	Game::Game()
	{
		board = BitBoard(0);
		board.Initial();
		score = 0;
		step = 0;
	}
	Game::Game(unsigned long long rawBoard)
	{
		board = BitBoard(rawBoard);
		score = 0;
		step = 0;
	}

	Game::~Game()
	{
	}
	BitBoard Game::Board()
	{
		return board;
	}
	int Game::Score()
	{
		return score;
	}
	int Game::Step()
	{
		return step;
	}
	bool Game::IsEnd()
	{
		return !board.CanMove();
	}
	
	unsigned long long Game::Move(Direction direction)
	{
		unsigned long long movedRawBlocks = board.RawBlocks();
		if (board.MoveCheck(direction))
		{
			int reward;
			board = board.Move(direction, reward);
			score += reward;
			step++;
			movedRawBlocks = board.RawBlocks();
			board.InsertNewTile();
		}
		return movedRawBlocks;
	}
}
