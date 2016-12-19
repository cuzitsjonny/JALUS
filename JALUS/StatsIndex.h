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

	bool flag_1 = false; // TOGGLE MANUALLY!
	unsigned long cur_health; // SET MANUALLY!
	float max_health; // SET MANUALLY!
	unsigned long cur_armor; // SET MANUALLY!
	float max_armor; // SET MANUALLY!
	unsigned long cur_imagination; // SET MANUALLY!
	float max_imagination; // SET MANUALLY!
	unsigned long data_1_6 = 0;
	bool data_1_7 = false;
	bool data_1_8 = false;
	bool data_1_9 = false;
	// max_health a second time
	// max_armor a second time
	// max_imagination a second time
	long faction_id; // SET MANUALLY!
	bool is_smashable; // SET MANUALLY!
	bool data_1_12 = false;
	bool data_1_13 = false;
	bool data_is_smashable_0 = false;

	bool flag_is_smashable_1 = false;
	unsigned long data_is_smashable_1_0;

	bool flag_2 = true;
	bool data_2_0 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};