#include "InventoryIndex.h"

void InventoryIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(items.size() > 0);
	if (items.size() > 0)
	{
		bitStream->Write((unsigned long)items.size());
		for (int i = 0; i < items.size(); i++)
		{
			InventoryItem item = items.at(i);

			bitStream->Write(item.objectID);
			bitStream->Write(item.lot);
			bitStream->Write(false);

			bitStream->Write(item.count > 0);
			if (item.count > 0)
			{
				bitStream->Write(item.count);
			}

			bitStream->Write(item.slot > 0);
			if (item.slot > 0)
			{
				bitStream->Write(item.slot);
			}

			bitStream->Write(false);
			bitStream->Write(false); // has compressed LDF?
			bitStream->Write(true); // needs to query CDClient?
		}
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(data_1_0);
	}
}
