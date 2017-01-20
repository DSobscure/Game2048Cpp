#pragma once
#include"BitBoard.h"

namespace Game2048_Game_Library
{
	class Game
	{
	public:
		Game();
		Game(unsigned long long rawBoard);
		Game(Game& game);
		~Game();
		Game& operator=(Game other);
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
