#include "RebuildIndex.h"

void RebuildIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{

	//bitStream->Write(flagTemp);
	//bitStream->Write(un32Temp);

	//bitStream->Write(flag_1);
	/*if (flag_1)
	{
		bitStream->Write(rebuildState);

		bitStream->Write(success);
		bitStream->Write(enabled);
		
		bitStream->Write(rebuildTimePassed);
		bitStream->Write(rebuildTimePaused);
	}*/
	bitStream->Write(true);
	bitStream->Write((unsigned long)0);

	bitStream->Write(true);
	bitStream->Write((unsigned long)2);

	//bitStream->Write(success);
	bitStream->Write(true);
	//bitStream->Write(enabled);
	bitStream->Write(true);

	bitStream->Write(rebuildTimePassed);
	bitStream->Write(rebuildTimePaused);



	if (isConstruction)
	{
		bitStream->Write(false);
		

		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);

		bitStream->Write(true);

		/*bitStream->Write(flag_0);
		if (flag_0)
		{
			bitStream->Write(data_0_0);
		}

		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);

		bitStream->Write(flag_0_1);
		*/
	}




}
