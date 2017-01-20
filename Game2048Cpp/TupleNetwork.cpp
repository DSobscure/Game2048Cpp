#include "TupleNetwork.h"

namespace Game2048_AI_TD_Learning
{
	TupleNetwork::TupleNetwork(string networkName, int index)
	{
		switch (index)
		{
			case 0:
				featureSet.push_back(new SixTupleFeature(1));
				featureSet.push_back(new SixTupleFeature(101));
				featureSet.push_back(new SixTupleFeature(502));
				featureSet.push_back(new SixTupleFeature(602));
				break;
		}
	}


	TupleNetwork::~TupleNetwork()
	{
		for (unsigned int i = 0; i < featureSet.size(); i++)
		{
			delete featureSet[i];
		}
	}

	float TupleNetwork::GetValue(unsigned long long blocks)
	{
		SetLocalRoatatedBoards(blocks);
		for (unsigned int i = 0; i < featureSet.size(); i++)
		{
			featureSet[i]->SetSymmetricBoards(rotatedBoards);
		}
		float sum = 0;
		for (unsigned int i = 0; i < featureSet.size(); i++)
		{
			sum += featureSet[i]->GetScore(blocks);
		}
		return sum;
	}
	void TupleNetwork::UpdateValue(unsigned long long blocks, float delta)
	{
		SetLocalRoatatedBoards(blocks);
		for (unsigned int i = 0; i < featureSet.size(); i++)
		{
			featureSet[i]->SetSymmetricBoards(rotatedBoards);
		}
		for (unsigned int i = 0; i < featureSet.size(); i++)
		{
			featureSet[i]->UpdateScore(blocks, delta);
		}
	}
	void TupleNetwork::SetLocalRoatatedBoards(unsigned long long rawBlocks)
	{
		unsigned short rowContents[4];
		unsigned short reversedRowContents[4];
		unsigned short verticalFillpedRowContents[4];
		unsigned short reversedVerticalFlippedRowContents[4];

		for (int i = 0; i < 4; i++)
		{
			rowContents[i] = BitBoardOperationSet.GetRow(rawBlocks, i);
			reversedRowContents[i] = BitBoardOperationSet.ReverseRow(rowContents[i]);
			verticalFillpedRowContents[3 - i] = rowContents[i];
			reversedVerticalFlippedRowContents[3 - i] = reversedRowContents[i];
		}

		rotatedBoards[0] = rawBlocks;//origin board
		rotatedBoards[1] = BitBoardOperationSet.SetColumns(reversedRowContents);//clockwise rotate 270
		rotatedBoards[2] = BitBoardOperationSet.SetColumns(verticalFillpedRowContents);//clockwise rotate 90
		rotatedBoards[3] = BitBoardOperationSet.SetRows(reversedVerticalFlippedRowContents);//clockwise rotate 180
	}
	void TupleNetwork::Save()
	{

	}
	void TupleNetwork::Load(int& loadedCount)
	{
		loadedCount = 0;
	}
}
