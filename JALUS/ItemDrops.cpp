#include "ItemDrops.h"
#include "Objects.h"

vector<ItemDrop> ItemDrops::itemDrops;

long long ItemDrops::createDroppedItem(long lot)
{
	long long objid;
	if (lot == 176)
		objid = Objects::generateSpawnerID();
	else
		objid = Objects::generateObjectID();


	ItemDrop drop;
	drop.id = objid;
	drop.lot = lot;

	itemDrops.push_back(drop);

	return objid;

}

void ItemDrops::removeDroppedItem(long long id)
{
	bool found = false;
	for (int i = 0; i < itemDrops.size() && !found; i++)
	{
		if (itemDrops.at(i).id == id)
		{
			itemDrops.erase(itemDrops.begin() + i);
			found = true;
		}
	}
}

long ItemDrops::getDroppedItem(long long id)
{
	for (int i = 0; i < itemDrops.size(); i++)
	{
		if (itemDrops.at(i).id == id) {
			return itemDrops.at(i).lot;
		}
	}
}
