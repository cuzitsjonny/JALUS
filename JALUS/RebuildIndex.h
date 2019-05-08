#pragma once
#include "Common.h"
#include "ScriptedActivityIndex.h"

class RebuildIndex
{
public:
	ScriptedActivityIndex* scriptedActivityIndex = new ScriptedActivityIndex();
	
	bool flag_0;
	unsigned long rebuildState;
	bool success;
	bool enabled;
	float rebuildTime;
	float rebuildTimePaused;

	bool flag_1;
	unsigned long data_1_0;
	float activator_pos_x;
	float activator_pos_y;
	float activator_pos_z;
	bool data_1_1;


	


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
