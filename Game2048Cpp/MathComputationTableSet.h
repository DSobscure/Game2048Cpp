#pragma once

namespace Game2048_Game_Library_TableSets
{
	class MathComputationTableSet
	{
	public:
		int log2Table[65536];

		MathComputationTableSet()
		{
			for (int i = 0; i < 16; i++)
			{
				log2Table[1 << i] = i;
			}
		}
	} MathComputationTableSet;
}
