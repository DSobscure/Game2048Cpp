#pragma once
#include <vector>
#include "TupleFeature.h"
#include "FourTupleFeature.h"
#include "SixTupleFeature.h"

using namespace std;
using namespace Game2048_AI_TD_Learning_TupleFeatures;

namespace Game2048_AI_TD_Learning
{
	class TupleNetwork
	{
	public:
		TupleNetwork(string networkName, int index);
		~TupleNetwork();

		vector<TupleFeature*> featureSet;
		string networkName;

		float GetValue(unsigned long long blocks);
		void UpdateValue(unsigned long long blocks, float delta);
		void SetLocalRoatatedBoards(unsigned long long rawBlocks);
		void Save();
		void Load(int& loadedCount);

	private:
		vector<unsigned long long> rotatedBoards = vector<unsigned long long>(4);
	};
}
