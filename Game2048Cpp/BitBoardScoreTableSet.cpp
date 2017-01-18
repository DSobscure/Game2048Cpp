#pragma once

namespace Game2048_Game_Library_TableSets
{
	class BitBoardScoreTableSet
	{
	public:
		int tileScoreTable[16];
		float rowScoreTable[65536];

		BitBoardScoreTableSet()
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
	} BitBoardScoreTableSet;
}