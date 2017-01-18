#pragma once
#include "BitBoard.h"
#include <iostream>

using namespace Game2048_Game_Library;
using namespace std;

namespace Game2048_Game_ConsoleVersion
{
	class StaticConsoleGameEnvironment
	{
	public:
		static void PrintBoard(BitBoard board)
		{
			for (int columnIndex = 0; columnIndex < 4; columnIndex++)
			{
				for (int rowIndex = 0; rowIndex < 4; rowIndex++)
				{
					unsigned long long rawBlockValue = (unsigned long long)((board.RawBlocks() >> (16 * columnIndex + 4 * rowIndex)) & 0xf);
					if (rawBlockValue > 0)
						cout << "\t" << (1 << (int)(rawBlockValue));
					else
						cout<< "\t0";
				}
				cout << endl;
			}
			cout << endl;
		}
		static Direction GetDirection()
		{
			char key;
			cin >> key;
			switch (key)
			{
			case 'w':
				return Direction::Up;
			case 's':
				return Direction::Down;
			case 'a':
				return Direction::Left;
			case 'd':
				return Direction::Right;
			default:
				return Direction::No;
			}
		}
	};
}
