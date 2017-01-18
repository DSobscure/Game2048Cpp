#pragma once
#include"Direction.h"
#include"BitBoard.h"

namespace Game2048_Game_Library
{
	class Game
	{
	public:
		Game();
		Game(unsigned long long rawBoard);
		~Game();
		BitBoard Board();
		int Score();
		int Step();
		bool IsEnd();
		unsigned long long Move(Direction direction);
	private:
		BitBoard board;
		int score;
		int step;
	};
}
