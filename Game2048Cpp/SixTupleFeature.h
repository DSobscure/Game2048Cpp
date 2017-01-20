#pragma once
#include "TupleFeature.h"

using namespace Game2048_AI_TD_Learning;

namespace Game2048_AI_TD_Learning_TupleFeatures
{
	class SixTupleFeature : public TupleFeature
	{
	public:
		SixTupleFeature();
		~SixTupleFeature();
		int index;

		SixTupleFeature(vector<float> tuples, int index);
		SixTupleFeature(int index);
		int GetIndex(unsigned long long blocks);
	};
}

