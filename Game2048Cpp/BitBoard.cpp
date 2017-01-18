#include "BitBoard.h"
#include "Direction.h"
#include "BitBoardScoreTableSet.cpp"
#include "BitBoardShiftTableSet.cpp";

using namespace Game2048_Game_Library_TableSets;
using namespace Game2048_Game_Library_TableSets;

namespace Game2048_Game_Library
{
	BitBoard::BitBoard()
	{
		this->rawBlocks = 0;
	}
	BitBoard::BitBoard(BitBoard& board)
	{
		this->rawBlocks = board.rawBlocks;
	}
	BitBoard::BitBoard(unsigned long long rawBlocks)
	{
		this->rawBlocks = rawBlocks;
	}
	BitBoard::~BitBoard()
	{

	}
	BitBoard& BitBoard::operator=(BitBoard other)
	{
		this->rawBlocks = other.rawBlocks;
	}

	int BitBoard::RawEmptyCountTest(unsigned long long rawBlocks)
	{
		int result = 0;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			if (((rawBlocks >> shiftBitCount) & 0xf) == 0)
			{
				result++;
			}
		}
		return result;
	}
	int BitBoard::MaxTileTest(unsigned long long rawBlocks)
	{
		int maxTile = 1;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			int tileNumber = (int)((rawBlocks >> shiftBitCount) & 0xf);
			if (BitBoardScoreTableSet.tileScoreTable[tileNumber] > maxTile)
			{
				maxTile = BitBoardScoreTableSet.tileScoreTable[tileNumber];
			}
		}
		return maxTile;
	}
	int BitBoard::RawMaxTileTest(unsigned long long rawBlocks)
	{
		int maxTile = 1;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			int tileNumber = (int)((rawBlocks >> shiftBitCount) & 0xf);
			if (tileNumber > maxTile)
			{
				maxTile = tileNumber;
			}
		}
		return maxTile;
	}
	unsigned long long BitBoard::InversBlocks(unsigned long long rawBlocks)
	{
		int maxTile = RawMaxTileTest(rawBlocks);
		unsigned long long result = 0;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			int tileNumber = (int)((rawBlocks >> shiftBitCount) & 0xf);
			int inverseNumber = (tileNumber != 0) ? maxTile - tileNumber : 15;
			result |= ((unsigned long long)inverseNumber) << shiftBitCount;
		}
		return result;
	}
	unsigned long long BitBoard::RawBlocks()
	{
		return rawBlocks;
	}
	int BitBoard::EmptyCount()
	{
		int result = 0;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			if (((rawBlocks >> shiftBitCount) & 0xf) == 0)
			{
				result++;
			}
		}
		return result;
	}
	bool BitBoard::CanMove()
	{
		for (Direction direction = Direction::Up; direction <= Direction::Right; direction = (Direction)(direction + 1))
		{
			if (MoveCheck(direction))
				return true;
		}
		return false;
	}
	bool BitBoard::IsFull()
	{
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			if (((rawBlocks >> shiftBitCount) & 0xf) == 0)
			{
				return false;
			}
		}
		return true;
	}
	int BitBoard::MaxTile()
	{
		int maxTile = 0;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			int tileNumber = (int)((rawBlocks >> shiftBitCount) & 0xf);
			if (BitBoardScoreTableSet.tileScoreTable[tileNumber] > maxTile)
			{
				maxTile = BitBoardScoreTableSet.tileScoreTable[tileNumber];
			}
		}
		return maxTile;
	}
	int BitBoard::MaxTileRaw()
	{
		int maxTile = 0;
		for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
		{
			int tileNumber = (int)((rawBlocks >> shiftBitCount) & 0xf);
			if (tileNumber > maxTile)
			{
				maxTile = tileNumber;
			}
		}
		return maxTile;
	}
	void BitBoard::InsertNewTile()
	{
		int emptyCountCache = EmptyCount();
		if (emptyCountCache > 0)
		{
			unsigned long long tile = (rand()%10 == 0) ? 2UL : 1UL;

			int targetIndex = rand() % emptyCountCache;
			for (int shiftBitCount = 0; shiftBitCount < 64; shiftBitCount += 4)
			{
				if (((rawBlocks >> shiftBitCount) & 0xf) == 0)
				{
					if (targetIndex == 0)
					{
						rawBlocks |= tile << shiftBitCount;
						break;
					}
					else
					{
						targetIndex--;
					}
				}
			}
		}
	}
	void BitBoard::Initial()
	{
		InsertNewTile();
		InsertNewTile();
	}
	BitBoard BitBoard::Move(Direction direction, int& reward)
	{
		unsigned long long rawBlocksAfterMove, transposedRawBlocks;
		reward = 0;
		rawBlocksAfterMove = rawBlocks;
		switch (direction)
		{
		case Direction::Up:
			transposedRawBlocks = BitBoardOperationSet.Transpose(rawBlocks);
			ColumnShiftInfo shiftUpInfos[4]
			{
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= shiftUpInfos[i].column << (4 * i);
				reward += shiftUpInfos[i].reward;
			}
			return BitBoard(rawBlocksAfterMove);
		case Direction::Down:
			transposedRawBlocks = BitBoardOperationSet.Transpose(rawBlocks);
			ColumnShiftInfo shiftDownInfos[4]
			{
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= shiftDownInfos[i].column << (4 * i);
				reward += shiftDownInfos[i].reward;
			}
			return BitBoard(rawBlocksAfterMove);
		case Direction::Left:
			RowShiftInfo shiftLeftInfos[4] =
			{
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= ((unsigned long long)shiftLeftInfos[i].row) << (16 * i);
				reward += shiftLeftInfos[i].reward;
			}
			return BitBoard(rawBlocksAfterMove);
		case Direction::Right:
			RowShiftInfo shiftRightInfos[4] =
			{
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= ((unsigned long long)shiftRightInfos[i].row) << (16 * i);
				reward += shiftRightInfos[i].reward;
			}
			return BitBoard(rawBlocksAfterMove);
		default:
			return BitBoard(rawBlocks);
		}
	}
	unsigned long long BitBoard::MoveRaw(Direction direction, int& reward)
	{
		unsigned long long rawBlocksAfterMove, transposedRawBlocks;
		reward = 0;
		rawBlocksAfterMove = rawBlocks;
		switch (direction)
		{
		case Direction::Up:
			transposedRawBlocks = BitBoardOperationSet.Transpose(rawBlocks);
			ColumnShiftInfo shiftUpInfos[4] = 
			{
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftUpTable[(transposedRawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= shiftUpInfos[i].column << (4 * i);
				reward += shiftUpInfos[i].reward;
			}
			return rawBlocksAfterMove;
		case Direction::Down:
			transposedRawBlocks = BitBoardOperationSet.Transpose(rawBlocks);
			ColumnShiftInfo shiftDownInfos[4] =
			{
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.columnShiftDownTable[(transposedRawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= shiftDownInfos[i].column << (4 * i);
				reward += shiftDownInfos[i].reward;
			}
			return rawBlocksAfterMove;
		case Direction::Left:
			RowShiftInfo shiftLeftInfos[4]
			{
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftLeftTable[(rawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= ((unsigned long long)shiftLeftInfos[i].row) << (16 * i);
				reward += shiftLeftInfos[i].reward;
			}
			return rawBlocksAfterMove;
		case Direction::Right:
			RowShiftInfo shiftRightInfos[4] =
			{
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 0) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 16) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 32) & 0xFFFF],
				BitBoardShiftTableSet.rowShiftRightTable[(rawBlocks >> 48) & 0xFFFF]
			};
			for (int i = 0; i < 4; i++)
			{
				rawBlocksAfterMove ^= ((unsigned long long)shiftRightInfos[i].row) << (16 * i);
				reward += shiftRightInfos[i].reward;
			}
			return rawBlocksAfterMove;
		default:
			return rawBlocks;
		}
	}
	bool BitBoard::MoveCheck(Direction direction)
	{
		int reward;
		return rawBlocks != MoveRaw(direction, reward);
	}
	int BitBoard::TileCount(int tile)
	{
		int result = 0;
		for (int i = 0; i < 64; i += 4)
		{
			if (BitBoardScoreTableSet.tileScoreTable[((rawBlocks >> i) & 0xf)] == tile)
			{
				result++;
			}
		}
		return result;
	}
}
