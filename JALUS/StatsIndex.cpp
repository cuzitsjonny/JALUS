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
		bitStream->Write(current_health);
		bitStream->Write(max_health);
		bitStream->Write(current_armor);
		bitStream->Write(max_armor);
		bitStream->Write(current_imagination);
		bitStream->Write(max_imagination);
		bitStream->Write(unknown_0);
		bitStream->Write(unknown_1);
		bitStream->Write(unknown_2);
		bitStream->Write(unknown_3);
		bitStream->Write(max_health);
		bitStream->Write(max_armor);
		bitStream->Write(max_imagination);

		if (faction_id > -1)
		{
			bitStream->Write((unsigned long)1);
			bitStream->Write(faction_id);
		}
		else
		{
			bitStream->Write((unsigned long)0);
		}

		bitStream->Write(is_smashable);
		
		if (isConstruction)
		{
			bitStream->Write(after_is_smashable_0);
			bitStream->Write(after_is_smashable_1);
			if (is_smashable)
			{
				bitStream->Write(trigger_data_0);
				bitStream->Write(trigger_data_1);
				if (trigger_data_1)
				{
					bitStream->Write(trigger_data_1_0);
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
