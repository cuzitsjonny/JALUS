#include "ModuleAssemblyIndex.h"

void ModuleAssemblyIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(flag_0);
		if (flag_0)
		{
			bitStream->Write(flag_0_1);
			if (flag_0_1)
			{
				bitStream->Write(data_0);
			}

			bitStream->Write(flag_0_2);

			bitStream->Write(wstringLength);
			if (wstringLength > 0)
			{
				bitStream->Write(data_1);
			}
		}
	}
}