#pragma once
#include "Common.h"

class ItemDrop
{
public:
	long long id;
	long lot;
};

class ItemDrops
{
private:
	static vector<ItemDrop> itemDrops;
public:
	static long long createDroppedItem(long lot);
	static void removeDroppedItem(long long id);
	static long getDroppedItem(long long id);
};