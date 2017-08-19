#include "ScriptedActivityIndex.h"

void ScriptedActivityIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag);
	if (flag)
	{
		bitStream->Write(count);
		if (count)
		{
			bitStream->Write(playerObjectID);

			bitStream->Write(float_0);
			bitStream->Write(float_1);
			bitStream->Write(float_2);
			bitStream->Write(float_3);
			bitStream->Write(float_4);
			bitStream->Write(float_5);
			bitStream->Write(float_6);
			bitStream->Write(float_7);
			bitStream->Write(float_8);
			bitStream->Write(float_9);

		}
	}


}
