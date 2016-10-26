#pragma once
#include "InventoryItems.h"

class TemporaryItems
{
private:
	static vector<InventoryItem> items;
public:
	static void addItem(InventoryItem item);
	static void removeItem(long long objectID);
	static void removeItems(long long ownerID);
	static bool isObjectIDOccupied(long long objectID);
	static vector<InventoryItem> getItems(long long ownerID);
};