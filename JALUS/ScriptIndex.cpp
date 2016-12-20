#include "ScriptIndex.h"

void ScriptIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(data_0);
	}
}
