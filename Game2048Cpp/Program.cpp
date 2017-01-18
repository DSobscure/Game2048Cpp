#include<iostream>
#include"Game.h"
#include "ConsoleGameEnvironment.h"

using namespace std;
using namespace Game2048_Game_Library;
using namespace Game2048_Game_ConsoleVersion;

namespace Game2048_Game_Library
{
	StaticBitBoardScoreTableSet BitBoardScoreTableSet;
	StaticBitBoardOperationSet BitBoardOperationSet;
	StaticBitBoardShiftTableSet BitBoardShiftTableSet;
}

int main()
{
	Game game;

	while (!game.IsEnd())
	{
		StaticConsoleGameEnvironment::PrintBoard(game.Board());
		game.Move(StaticConsoleGameEnvironment::GetDirection());
	}

	system("PAUSE");
	return 0;
}