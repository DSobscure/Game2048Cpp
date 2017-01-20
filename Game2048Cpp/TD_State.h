#pragma once
namespace Game2048_AI_TD_Learning
{
	struct TD_State
	{
	public:
		unsigned long long movedRawBlocks;
		unsigned long long insertedRawBlocks;
	};
}
