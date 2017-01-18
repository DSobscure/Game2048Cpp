#pragma once
#include"BitBoardScoreTableSet.cpp";

using namespace Game2048_Game_Library_TableSets;

namespace Game2048_Game_Library
{
	class BitBoardOperationSet
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
			unsigned short columnMask = (0xf000f000f000f000 >> (columnIndex * 4));

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
	} BitBoardOperationSet;
}
