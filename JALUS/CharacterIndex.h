#pragma once
#include "Common.h"

class CharacterIndex
{
public:
	bool flag_0 = true;
	bool flag_0_0 = false; // TOGGLE MANUALLY!
	long long vehicle_object_id; // SET MANUALLY!
	unsigned char data_0_1 = 0;

	bool flag_1 = false; // TOGGLE MANUALLY!
	unsigned long level; // SET MANUALLY!

	bool flag_2 = true;
	bool data_2_0 = false;
	bool data_2_1 = true;

	bool flag_3 = false;
	unsigned long long data_3_0;

	bool flag_4 = false;
	unsigned long long data_4_0;

	bool flag_5 = false;
	unsigned long long data_5_0;

	bool flag_6 = false;
	unsigned long long data_6_0;

	unsigned long hair_color; // SET MANUALLY!
	unsigned long hair_style; // SET MANUALLY!
	unsigned long hd = 0;
	unsigned long shirt_color; // SET MANUALLY!
	unsigned long pants_color; // SET MANUALLY!
	unsigned long cd = 0;
	unsigned long hdc = 0;
	unsigned long eyebrows_style; // SET MANUALLY!
	unsigned long eyes_style; // SET MANUALLY!
	unsigned long mouth_style; // SET MANUALLY!
	long long account_id; // SET MANUALLY!
	unsigned long long llog = 0;
	unsigned long long data_19 = 0;
	long long lego_score; // SET MANUALLY!
	bool is_free_to_play = false;
	// character stats
	bool data_49 = false;
	wstring landing_rocket_info = L"";

	bool flag_51 = true;
	bool data_51_0 = false;
	bool data_51_1 = false;
	unsigned char data_51_2 = 0;
	bool data_51_3 = false;
	unsigned char data_51_4 = 0;

	bool flag_52 = true;
	unsigned long data_52_0 = 0;

	bool flag_53 = true;
	long long data_53_0 = 0;
	unsigned char data_53_1 = 0;
	bool data_53_2 = false;
	long data_53_3 = -1;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};