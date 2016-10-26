#include "InventoryIndex.h"
#include "TemporaryItems.h"

InventoryIndex::InventoryIndex(bool isPlayer, long long objectID)
{
	this->isPlayer = isPlayer;
	this->objectID = objectID;
}

void InventoryIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	vector<InventoryItem> it;

	if (isPlayer)
	{
		it = InventoryItems::getEquippedInventoryItems(objectID);
	}
	else
	{
		it = TemporaryItems::getItems(objectID);
	}

	bitStream->Write(it.size() > 0);
	if (it.size() > 0)
	{
		bitStream->Write((unsigned long)it.size());
		if (it.size() > 0)
		{
			for (int i = 0; i < it.size(); i++)
			{
				InventoryItem item = it.at(i);

				bitStream->Write(item.objectID);
				bitStream->Write(item.lot);

				bitStream->Write(false);

				bitStream->Write(true);
				bitStream->Write(item.count);

				bitStream->Write(item.slot > 0);
				if (item.slot > 0)
				{
					bitStream->Write(item.slot);
				}

				bitStream->Write(false);

				bitStream->Write(false);

				bitStream->Write(true);
			}
		}
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(data_1_0);
	}
}
