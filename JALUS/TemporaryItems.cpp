#include "TemporaryItems.h"

vector<InventoryItem> TemporaryItems::items;

void TemporaryItems::addItem(InventoryItem item)
{
	items.push_back(item);
}

void TemporaryItems::removeItem(long long objectID)
{
	bool found = false;
	for (int i = 0; i < items.size() && !found; i++)
	{
		if (items.at(i).objectID == objectID)
		{
			items.erase(items.begin() + i);
			found = true;
		}
	}
}

void TemporaryItems::removeItems(long long ownerID)
{
	int a = 0;
	while (a < items.size())
	{
		a = 0;
		bool found = false;
		for (int i = 0; i < items.size() && !found; i++)
		{
			if (items.at(i).ownerID == ownerID)
			{
				items.erase(items.begin() + i);
				found = true;
			}

			if (!found)
				a++;
		}
	}
}

bool TemporaryItems::isObjectIDOccupied(long long objectID)
{
	bool found = false;
	for (int i = 0; i < items.size() && !found; i++)
	{
		if (items.at(i).objectID == objectID)
			found = true;
	}

	return found;
}

vector<InventoryItem> TemporaryItems::getItems(long long ownerID)
{
	vector<InventoryItem> r;

	for (int i = 0; i < items.size(); i++)
	{
		if (items.at(i).ownerID == ownerID)
		{
			r.push_back(items.at(i));
		}
	}

	return r;
}
