#pragma once
#include "Direction.h"
using namespace Game2048_Game_Library;

namespace Game2048_AI_TD_Learning
{
	struct BestMoveNode
	{
	public:
		Direction bestMove;
		float reward;
		unsigned long long rawBlocks;
		unsigned long long movedRawBlocks;
	};
}