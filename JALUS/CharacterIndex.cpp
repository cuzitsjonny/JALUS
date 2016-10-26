#include "CharacterIndex.h"

void CharacterIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(flag_0_0);
		if (flag_0_0)
		{
			bitStream->Write(driven_vehicle_object_id);
		}

		bitStream->Write(data_0_1);
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(level);
	}

	bitStream->Write(flag_2);
	if (flag_2)
	{
		bitStream->Write(data_2_0);
		bitStream->Write(data_2_1);
	}

	if (isConstruction)
	{
		bitStream->Write(flag_3);
		if (flag_3)
		{
			bitStream->Write(co);
		}

		bitStream->Write(flag_4);
		if (flag_4)
		{
			bitStream->Write(data_4_0);
		}

		bitStream->Write(flag_5);
		if (flag_5)
		{
			bitStream->Write(data_5_0);
		}

		bitStream->Write(flag_6);
		if (flag_6)
		{
			bitStream->Write(data_6_0);
		}

		bitStream->Write(hair_color);
		bitStream->Write(hair_style);
		bitStream->Write(hd);
		bitStream->Write(shirt_color);
		bitStream->Write(pants_color);
		bitStream->Write(cd);
		bitStream->Write(hdc);
		bitStream->Write(eyebrows_style);
		bitStream->Write(eyes_style);
		bitStream->Write(mouth_style);
		bitStream->Write(account_id);
		bitStream->Write(llog);
		bitStream->Write(unknown_0);
		bitStream->Write(universe_score);
		bitStream->Write(is_player_free_to_play);

		for (int i = 0; i < 27; i++)
		{
			bitStream->Write((unsigned long long)0);
		}

		bitStream->Write(unknown_1);

		bitStream->Write((rocket_ldf.length() > 0));
		if (rocket_ldf.length() > 0)
		{
			bitStream->Write((unsigned short)rocket_ldf.length());

			for (int i = 0; i < rocket_ldf.length(); i++)
			{
				bitStream->Write(rocket_ldf[i]);
			}
		}
	}

	bitStream->Write(flag_7);
	if (flag_7)
	{
		bitStream->Write(data_7_0);
		bitStream->Write(data_7_1);

		bitStream->Write(data_7_2_0);
		bitStream->Write(data_7_2_1);
		bitStream->Write(data_7_2_2);
		bitStream->Write(data_7_2_3);
		bitStream->Write(data_7_2_4);
		bitStream->Write(data_7_2_5);
		bitStream->Write(data_7_2_6);
		bitStream->Write(data_7_2_7);

		bitStream->Write(data_7_3);
		bitStream->Write(data_7_4);
	}

	bitStream->Write(flag_8);
	if (flag_8)
	{
		bitStream->Write(data_8_0);
	}

	bitStream->Write(flag_9);
	if (flag_9)
	{
		bitStream->Write(data_9_0);
		bitStream->Write(data_9_1);
		bitStream->Write(data_9_2);
		bitStream->Write(data_9_3);
	}
}
