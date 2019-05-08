#pragma once
#include "Common.h"

class MovingPlatformIndex
{
public:
	bool flag_0 = false;
	bool flag_0_0;
	wstring pathName;
	unsigned long data_0_1;
	bool data_0_2;

	bool flag_1_0;
	unsigned long subcomponentType; // 4 is mover, 5 is simple mover

	// TYPE 4 - MOVER
	bool flag_1_0_0;
	unsigned long state;
	signed long data_1_0_0;
	bool data_1_0_1;
	bool data_1_0_2;
	float data_1_0_3;

	float target_pos_x;
	float target_pos_y;
	float target_pos_z;

	unsigned long currWaypointIndex;
	unsigned long nextWaypointIndex;

	float idleTimePassed;
	unsigned long data_1_0_4;

	// TYPE 5 - SIMPLE MOVER
	bool flag_1_1;
	bool flag_1_2;
	float pos_x;
	float pos_y;
	float pos_z;
	float rot_x;
	float rot_y;
	float rot_z;
	float rot_w;
	
	bool flag_1_3;
	unsigned long data_1_3_0;
	unsigned long data_1_3_1;
	bool data_1_3_2;


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};