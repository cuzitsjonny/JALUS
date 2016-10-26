#pragma once
#include "Common.h"

class StatsIndex
{
public:
	bool flag_0 = true;
	unsigned long data_0_0 = 0;
	unsigned long data_0_1 = 0;
	unsigned long data_0_2 = 0;
	unsigned long data_0_3 = 0;
	unsigned long data_0_4 = 0;
	unsigned long data_0_5 = 0;
	unsigned long data_0_6 = 0;
	unsigned long data_0_7 = 0;
	unsigned long data_0_8 = 0;

	bool flag_1 = false;
	unsigned long current_health;
	unsigned long current_armor;
	unsigned long current_imagination;
	unsigned long unknown_0 = 0;
	bool unknown_1 = false;
	bool unknown_2 = false;
	bool unknown_3 = false;
	float max_health;
	float max_armor;
	float max_imagination;
	long faction_id;
	bool is_smashable;
	bool after_is_smashable_0 = false;
	bool after_is_smashable_1 = false;
	bool trigger_data_0 = false;
	bool trigger_data_1 = false;
	unsigned long trigger_data_1_0;

	bool flag_2 = true;
	bool data_2_0 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};