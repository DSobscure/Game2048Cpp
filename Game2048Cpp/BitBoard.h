#pragma once
#include"Direction.h"
#include<random>

using namespace std;

namespace Game2048_Game_Library
{
	class BitBoard
	{
	public:
		BitBoard();
		BitBoard(BitBoard& board);
		BitBoard(unsigned long long rawBlocks);
		~BitBoard();
		BitBoard& operator=(BitBoard other);

		static int RawEmptyCountTest(unsigned long long rawBlocks);
		static int MaxTileTest(unsigned long long rawBlocks);
		static int RawMaxTileTest(unsigned long long rawBlocks);
		static unsigned long long InversBlocks(unsigned long long rawBlocks);
		unsigned long long RawBlocks();
		int EmptyCount();
		bool CanMove();
		bool IsFull();
		int MaxTile();
		int MaxTileRaw();
		void InsertNewTile();
		void Initial();
		BitBoard Move(Direction direction, int& reward);
		unsigned long long MoveRaw(Direction direction, int& reward);
		bool MoveCheck(Direction direction);
		int TileCount(int tile);

	private: 
		unsigned long long rawBlocks;
	};
}

