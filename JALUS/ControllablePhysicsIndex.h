#pragma once
#include "Common.h"

class ControllablePhysicsIndex
{
public:
	bool flag_0 = false;
	unsigned long data_0_0;
	bool data_0_1;
	bool data_0_2;

	bool flag_1 = true;
	unsigned long data_1_0 = 0;
	unsigned long data_1_1 = 0;
	unsigned long data_1_2 = 0;
	unsigned long data_1_3 = 0;
	unsigned long data_1_4 = 0;
	unsigned long data_1_5 = 0;
	unsigned long data_1_6 = 0;

	bool flag_2 = false;
	float data_2_0;
	float data_2_1;

	bool flag_3 = true;
	unsigned long data_3_0 = 0;
	bool data_3_1 = false;

	bool flag_4 = true;
	bool flag_4_0 = false;
	unsigned long data_4_0_0;
	bool data_4_0_1;

	bool flag_5 = false; // TOGGLE MANUALLY!
	float pos_x; // SET MANUALLY!
	float pos_y; // SET MANUALLY!
	float pos_z; // SET MANUALLY!
	float rot_x; // SET MANUALLY!
	float rot_y; // SET MANUALLY!
	float rot_z; // SET MANUALLY!
	float rot_w; // SET MANUALLY!
	bool is_on_ground; // SET MANUALLY!
	bool data_5_8 = false;

	bool flag_velocity = false;
	float vel_x;
	float vel_y;
	float vel_z;

	bool flag_angular_velocity = false;
	float ang_vel_x;
	float ang_vel_y;
	float ang_vel_z;

	bool flag_5_11 = false;
	long long data_5_11_0;
	float data_5_11_1;
	float data_5_11_2;
	float data_5_11_3;

	bool flag_5_11_4;
	float data_5_11_4_0;
	float data_5_11_4_1;
	float data_5_11_4_2;

	bool data_5_12 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};