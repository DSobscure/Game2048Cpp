#include<iostream>
#include"Game.h"
#include "ConsoleGameEnvironment.h"
#include "LearningAgent.h"
#include "TD_LearningAI.h"

using namespace std;
using namespace Game2048_Game_Library;
using namespace Game2048_Game_ConsoleVersion;

namespace Game2048_Game_Library
{
	StaticBitBoardScoreTableSet BitBoardScoreTableSet;
	StaticBitBoardOperationSet BitBoardOperationSet;
	StaticBitBoardShiftTableSet BitBoardShiftTableSet;
}

int main()
{
	cout << "Input Mode" << endl;
	int commandCode;
	cin >> commandCode;

	int loadedCount = 0;
	LearningAgent* agent = nullptr;
	switch (commandCode)
	{
		case 0:
			agent = new LearningAgent(new TD_LearningAI(0.0025f, loadedCount));
		break;
		default:
			cout << "Not Existed Command" << endl;
			break;
	}
	cout << "Loaded " << loadedCount << " TupleFeatures" << endl;
	switch (commandCode)
	{
		case 0:
			agent->Training("Basic", TranningMode::Classical, 1, 5000000, 1000, StaticConsoleGameEnvironment::PrintBoard);
			break;
		default:
			cout << "Not Existed Command" << endl;
			break;
	}
	if (agent != nullptr)
	{
		delete agent;
	}
	return 0;
}