#include "RebuildIndex.h"

void RebuildIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{

	//bitStream->Write(flagTemp);
	//bitStream->Write(un32Temp);

	bitStream->Write(scriptedActivityIndex);
	

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(rebuildState);

		bitStream->Write(success);
		bitStream->Write(enabled);
		
		bitStream->Write(rebuildTimePassed);
		bitStream->Write(rebuildTimePaused);
	}



	if (isConstruction)
	{
		bitStream->Write(flag_0);
		if (flag_0)
		{
			bitStream->Write(data_0_0);
		}

		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);

		bitStream->Write(flag_0_1);

	}




}
