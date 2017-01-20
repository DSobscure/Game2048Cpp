#pragma once
#include "TupleFeature.h"

using namespace Game2048_AI_TD_Learning;

namespace Game2048_AI_TD_Learning_TupleFeatures
{
	class FourTupleFeature : public TupleFeature
	{
	public:
		FourTupleFeature();
		~FourTupleFeature();
		int index;

		FourTupleFeature(vector<float> tuples, int index);
		FourTupleFeature(int index);
		int GetIndex(unsigned long long blocks);
	};
}

