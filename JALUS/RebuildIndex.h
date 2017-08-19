#pragma once
#include "Common.h"
#include "ScriptedActivityIndex.h"

class RebuildIndex
{
public:

	//bool flagTemp;
	//unsigned long un32Temp;

	ScriptedActivityIndex* scriptedActivityIndex = new ScriptedActivityIndex();
	

	bool flag_1;
	unsigned long rebuildState;
	bool success;
	bool enabled;
	float rebuildTimePassed;
	float rebuildTimePaused;

	bool flag_0;
	unsigned long data_0_0;
	float pos_x;
	float pos_y;
	float pos_z;
	bool flag_0_1;


	


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
