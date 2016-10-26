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

	bool flag_5 = false;

	float pos_x;
	float pos_y;
	float pos_z;

	float rot_x;
	float rot_y;
	float rot_z;
	float rot_w;

	bool is_player_on_ground;
	bool unknown = false;

	bool flag_velocity = false;

	float vel_x;
	float vel_y;
	float vel_z;

	bool flag_angular_velocity = false;

	float ang_vel_x;
	float ang_vel_y;
	float ang_vel_z;

	bool flag_on_moving_platform = false;

	long long platform_object_id;

	float platform_unknown_0_0;
	float platform_unknown_0_1;
	float platform_unknown_0_2;

	bool flag_platform_unknown;

	float platform_unknown_1_0;
	float platform_unknown_1_1;
	float platform_unknown_1_2;

	bool flag_unknown;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};