#include "Component107Index.h"

void Component107Index::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(data_0_0);
	}
}
