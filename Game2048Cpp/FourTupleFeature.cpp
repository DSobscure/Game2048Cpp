#include "FourTupleFeature.h"

namespace Game2048_AI_TD_Learning_TupleFeatures
{
	FourTupleFeature::FourTupleFeature()
	{
	}

	FourTupleFeature::~FourTupleFeature()
	{
	}

	FourTupleFeature::FourTupleFeature(vector<float> tuples, int index) : TupleFeature(tuples)
	{
		this->index = index;
	}
	FourTupleFeature::FourTupleFeature(int index) : TupleFeature(4)
	{
		this->index = index;
	}
	int FourTupleFeature::GetIndex(unsigned long long blocks)
	{
		switch (index)
		{
		case 1:
			//oooo
			//xxxx
			//xxxx
			//xxxx
			return (int)((blocks >> 48) & 0xFFFF);
		case 2:
			//ooox
			//oxxx
			//xxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xFFF0) | ((blocks >> 44) & 0xF));
		case 3:
			//ooox
			//xoxx
			//xxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xFFF0) | ((blocks >> 40) & 0xF));
		case 4:
			//ooxx
			//ooxx
			//oxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xFF00) | ((blocks >> 40) & 0xFF));
		case 5:
			//ooxx
			//xxoo
			//xxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xFF00) | ((blocks >> 32) & 0xFF));
		case 6:
			//oxox
			//xoxx
			//oxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xF000) | ((blocks >> 44) & 0xF00) | ((blocks >> 36) & 0xF0) | ((blocks >> 28) & 0xF));
		case 7:
			//ooxx
			//xoox
			//xxxx
			//xxxx
			return (int)(((blocks >> 48) & 0xFF00) | ((blocks >> 36) & 0xFF));
		default:
			return 0;
		}
	}
}
