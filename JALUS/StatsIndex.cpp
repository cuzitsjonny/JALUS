#include "StatsIndex.h"

void StatsIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(flag_0);
		if (flag_0)
		{
			bitStream->Write(data_0_0);
			bitStream->Write(data_0_1);
			bitStream->Write(data_0_2);
			bitStream->Write(data_0_3);
			bitStream->Write(data_0_4);
			bitStream->Write(data_0_5);
			bitStream->Write(data_0_6);
			bitStream->Write(data_0_7);
			bitStream->Write(data_0_8);
		}
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(cur_health);
		bitStream->Write(max_health);
		bitStream->Write(cur_armor);
		bitStream->Write(max_armor);
		bitStream->Write(cur_imagination);
		bitStream->Write(max_imagination);
		bitStream->Write(data_1_6);
		bitStream->Write(data_1_7);
		bitStream->Write(data_1_8);
		bitStream->Write(data_1_9);
		bitStream->Write(max_health);
		bitStream->Write(max_armor);
		bitStream->Write(max_imagination);

		bitStream->Write((unsigned long)(faction_id > -1));
		if (faction_id > -1)
		{
			bitStream->Write(faction_id);
		}

		bitStream->Write(is_smashable);

		if (isConstruction)
		{
			bitStream->Write(data_1_12);
			bitStream->Write(data_1_13);

			if (is_smashable)
			{
				bitStream->Write(data_is_smashable_0);

				bitStream->Write(flag_is_smashable_1);
				if (flag_is_smashable_1)
				{
					bitStream->Write(data_is_smashable_1_0);
				}
			}
		}
	}

	bitStream->Write(flag_2);
	if (flag_2)
	{
		bitStream->Write(data_2_0);
	}
}
