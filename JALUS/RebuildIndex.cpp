#include "RebuildIndex.h"
#include "ScriptedActivityIndex.h"

void RebuildIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{

	scriptedActivityIndex->writeToBitStream(bitStream, false);

	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(rebuildState);

		bitStream->Write(success);
		bitStream->Write(enabled);
		
		bitStream->Write(rebuildTime);
		bitStream->Write(rebuildTimePaused);
	}

	if (isConstruction)
	{
		bitStream->Write(flag_1);
		if (flag_0)
		{
			bitStream->Write(data_1_0);
		}
		
		bitStream->Write(activator_pos_x);
		bitStream->Write(activator_pos_y);
		bitStream->Write(activator_pos_z);

		bitStream->Write(data_1_1);

	}




}
