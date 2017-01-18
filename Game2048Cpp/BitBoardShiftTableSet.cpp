#pragma once
#include"BitBoardScoreTableSet.cpp";
#include"BitBoardOperationSet.cpp";

using namespace Game2048_Game_Library;

namespace Game2048_Game_Library_TableSets
{
	struct RowShiftInfo
	{
	public:
		unsigned short row;
		int reward;

		RowShiftInfo() {}
		RowShiftInfo(unsigned short row, int reward)
		{
			this->row = row;
			this->reward = reward;
		}
	};
	struct ColumnShiftInfo
	{
	public:
		unsigned long long column;
		int reward;

		ColumnShiftInfo() {}
		ColumnShiftInfo(unsigned long long column, int reward)
		{
			this->column = column;
			this->reward = reward;
		}
	};
	class BitBoardShiftTableSet
	{
	public:
		RowShiftInfo rowShiftLeftTable[65536];
		RowShiftInfo rowShiftRightTable[65536];
		ColumnShiftInfo columnShiftUpTable[65536];
		ColumnShiftInfo columnShiftDownTable[65536];

		BitBoardShiftTableSet()
		{
			for (int rowContent = 0; rowContent < 65536; rowContent++)
			{
				int reward = 0;
				int lines[4] =
				{
					(rowContent >> 0) & 0xf,
					(rowContent >> 4) & 0xf,
					(rowContent >> 8) & 0xf,
					(rowContent >> 12) & 0xf
				};
				// execute a move to the left
				for (int blockIndex = 0; blockIndex < 3; blockIndex++)
				{
					int firstNonEmptyBlockIndex = blockIndex + 1;
					while (firstNonEmptyBlockIndex < 4 && lines[firstNonEmptyBlockIndex] == 0)
					{
						firstNonEmptyBlockIndex++;
					}
					if (firstNonEmptyBlockIndex > 3)
						break;

					if (lines[blockIndex] == 0)
					{
						lines[blockIndex] = lines[firstNonEmptyBlockIndex];
						lines[firstNonEmptyBlockIndex] = 0;
						blockIndex--;
					}
					else if (lines[blockIndex] == lines[firstNonEmptyBlockIndex])
					{
						if (lines[blockIndex] != 0xf)
						{
							lines[blockIndex]++;
							lines[firstNonEmptyBlockIndex] = 0;
							reward += 2 << (lines[blockIndex] - 1);
						}
					}
				}

				int reversedRowContent = BitBoardOperationSet.ReverseRow(rowContent);
				int resultRowContent = (lines[0] << 0) | (lines[1] << 4) | (lines[2] << 8) | (lines[3] << 12);
				int reversedResultRowContent = BitBoardOperationSet.ReverseRow(resultRowContent);

				rowShiftLeftTable[rowContent] = RowShiftInfo((unsigned short)(rowContent ^ resultRowContent), reward);
				rowShiftRightTable[reversedRowContent] = RowShiftInfo((unsigned short)(reversedRowContent ^ reversedResultRowContent), reward);
				columnShiftUpTable[rowContent] = ColumnShiftInfo(BitBoardOperationSet.UnpackColumn(rowContent) ^ BitBoardOperationSet.UnpackColumn(resultRowContent), reward);
				columnShiftDownTable[reversedRowContent] = ColumnShiftInfo(BitBoardOperationSet.UnpackColumn(reversedRowContent) ^ BitBoardOperationSet.UnpackColumn(reversedResultRowContent), reward);
			}
		}
	} BitBoardShiftTableSet;
}