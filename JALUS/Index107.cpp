#include "Index107.h"

void Index107::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(data_0_0);
	}
}
