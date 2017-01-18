#pragma once

namespace Game2048_Game_Library
{
	class StaticBitBoardScoreTableSet
	{
	public:
		int tileScoreTable[16];
		float rowScoreTable[65536];

		StaticBitBoardScoreTableSet()
		{
			tileScoreTable[0] = 0;
			for (int i = 1; i < 16; i++)
			{
				tileScoreTable[i] = 1 << i;
			}

			for (int i = 0; i < 65536; i++)
			{
				for (int shiftBitCount = 0; shiftBitCount < 16; shiftBitCount += 4)
				{
					int tileNumber = (i >> shiftBitCount) & 0xf;
					rowScoreTable[i] += tileScoreTable[tileNumber];
				}
			}
		}
	};
	extern StaticBitBoardScoreTableSet BitBoardScoreTableSet;

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

	class StaticBitBoardOperationSet
	{
	public:
		unsigned short ReverseRow(int rowContent)
		{
			return (unsigned short)((rowContent >> 12) | ((rowContent >> 4) & 0x00F0) | ((rowContent << 4) & 0x0F00) | (rowContent << 12));
		}
		unsigned long long UnpackColumn(int columnContent)
		{
			unsigned long long ulongColumnContent = (unsigned long long)columnContent;
			return (ulongColumnContent | (ulongColumnContent << 12) | (ulongColumnContent << 24) | (ulongColumnContent << 36)) & 0x000F000F000F000F;
		}
		unsigned long long Transpose(unsigned long long board)
		{
			unsigned long long result;
			unsigned long long diagonal4x4block = board & 0xFF00FF0000FF00FF;
			unsigned long long topRight4x4block = board & 0x00FF00FF00000000; ;
			unsigned long long downLeft4x4block = board & 0x00000000FF00FF00;

			unsigned long long swaped = diagonal4x4block | (topRight4x4block >> 24) | (downLeft4x4block << 24);

			unsigned long long diagonalNet = swaped & 0xF0F00F0FF0F00F0F;
			unsigned long long upperSparse4corner = swaped & 0x0F0F00000F0F0000; ;
			unsigned long long lowerSparse4corner = swaped & 0x0000F0F00000F0F0;

			result = diagonalNet | (upperSparse4corner >> 12) | (lowerSparse4corner << 12);
			return result;
		}
		float GetScore(unsigned long long board)
		{
			float score = 0;
			for (int i = 0; i < 4; i++)
			{
				score += BitBoardScoreTableSet.rowScoreTable[GetRow(board, i)];
			}
			return score;
		}
		unsigned short GetRow(unsigned long long board, int rowIndex)
		{
			switch (rowIndex)
			{
			case 0:
				return (unsigned short)((board >> 48) & 0xFFFF);
			case 1:
				return (unsigned short)((board >> 32) & 0xFFFF);
			case 2:
				return (unsigned short)((board >> 16) & 0xFFFF);
			case 3:
				return (unsigned short)((board) & 0xFFFF);
			}
			return (unsigned short)((board) & 0xFFFF);
		}
		unsigned short GetColumn(unsigned long long board, int columnIndex)
		{
			unsigned short columnMask = (unsigned short)(0xf000f000f000f000 >> (columnIndex * 4));

			board = (board & columnMask) << 4 * columnIndex;
			return (unsigned short)(GetRow(board, 0) | (GetRow(board, 1) >> 4) | (GetRow(board, 2) >> 8) | (GetRow(board, 3) >> 12));
		}
		unsigned long long SetRows(unsigned short* rows)
		{
			unsigned long long result = 0;
			result |= (unsigned long long)rows[0] << 48;
			result |= (unsigned long long)rows[1] << 32;
			result |= (unsigned long long)rows[2] << 16;
			result |= rows[3];
			return result;
		}
		unsigned long long SetColumns(unsigned short* columns)
		{
			return Transpose(SetRows(columns));
		}
	};
	extern StaticBitBoardOperationSet BitBoardOperationSet;

	class StaticBitBoardShiftTableSet
	{
	public:
		RowShiftInfo rowShiftLeftTable[65536];
		RowShiftInfo rowShiftRightTable[65536];
		ColumnShiftInfo columnShiftUpTable[65536];
		ColumnShiftInfo columnShiftDownTable[65536];

		StaticBitBoardShiftTableSet()
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
	};
	extern StaticBitBoardShiftTableSet BitBoardShiftTableSet;
}
