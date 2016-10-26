#pragma once
#include "Common.h"

class CharacterIndex
{
public:
	bool flag_0 = true;
	bool flag_0_0 = false;
	long long driven_vehicle_object_id;
	bool data_0_1 = 0;

	bool flag_1 = false;
	long level;

	bool flag_2 = true;
	bool data_2_0 = false;
	bool data_2_1 = true;

	bool flag_3 = false;
	unsigned long long co;

	bool flag_4 = false;
	unsigned long long data_4_0;

	bool flag_5 = false;
	unsigned long long data_5_0;

	bool flag_6 = false;
	unsigned long long data_6_0;

	long hair_color;
	long hair_style;
	long hd = 0;
	long shirt_color;
	long pants_color;
	long cd = 0;
	long hdc = 0;
	long eyebrows_style;
	long eyes_style;
	long mouth_style;
	long long account_id;
	long long llog = 0;
	long long unknown_0 = 0;
	long long universe_score;
	bool is_player_free_to_play = false;
	// structs
	bool unknown_1 = false;
	wstring rocket_ldf = L"";

	bool flag_7 = true;
	bool data_7_0 = false; // turns name yellow?
	bool data_7_1 = false; // one of the mythran bits?

	// unsigned char data_7_2 = 0;
	bool data_7_2_0 = false; // one of the mythran bits?
	bool data_7_2_1 = false;
	bool data_7_2_2 = false;
	bool data_7_2_3 = false;
	bool data_7_2_4 = false;
	bool data_7_2_5 = false;
	bool data_7_2_6 = false;
	bool data_7_2_7 = false;

	bool data_7_3 = false;
	unsigned char data_7_4 = 0;

	bool flag_8 = true;
	unsigned long data_8_0 = 0; // of 1 head glows?

	bool flag_9 = true;
	long long data_9_0 = 0;
	unsigned char data_9_1 = 0;
	bool data_9_2 = false;
	long data_9_3 = -1;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};