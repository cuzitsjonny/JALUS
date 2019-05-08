#pragma once
#include "Common.h"
#include "CDClient.h"

enum InventoryType : long
{
	INVENTORY_TYPE_DEFAULT = 0,
	INVENTORY_TYPE_BRICKS = 2,
	_INVENTORY_TYPE_TEMP_ITEMS = 4,
	INVENTORY_TYPE_MODELS = 5,
	INVENTORY_TYPE_TEMP_MODELS = 6,
	INVENTORY_TYPE_BEHAVIORS = 7,
	_INVENTORY_TYPE_PROPERTIES = 8,
	_INVENTORY_TYPE_VENDOR_SELL_SPACE = 11,
	INVENTORY_TYPE_COMMENDATIONS = 12,

	INVALID_INVENTORY_TYPE
};

class InventoryItem
{
public:
	long long objectID = -1;
	long long ownerID = -1;
	long lot = -1;
	ItemType itemType = ItemType::INVALID_ITEM_TYPE;
	InventoryType invType = InventoryType::INVALID_INVENTORY_TYPE;
	short slot = -1;
	long count = -1;
	bool isBound = false;
	bool isEquipped = false;
	//bool isProxy = false;
};

class InventoryItems
{
private:
	static vector<InventoryItem> phantomStorage;
public:
	static string name;

	static void init(string name, string structure);

	static InventoryType getInventoryType(ItemType itemType);
	//static long long createPhantomItem(long long charID, long lot, bool isEquipped, bool isProxy);
	//static long long createPhantomItem(long long charID, long lot, bool isEquipped);
	//static vector<InventoryItem> getPhantomItems(long long objectID);
	//static vector<InventoryItem> phantomStorage;

	//static long long createInventoryItem(long long ownerID, long lot, long count, bool isBound, bool isEquipped, bool isProxy = false);
	static long long createInventoryItem(long long ownerID, long lot, long count, bool isBound, bool isEquipped);
	//static void createInventoryItem(long long objectID, long long ownerID, long lot, long count, bool isBound, bool isEquipped, bool isProxy = false);
	static void createInventoryItem(long long objectID, long long ownerID, long lot, long count, bool isBound, bool isEquipped);

	static void deleteInventoryItem(long long objectID);
	static void deleteInventoryItems(long long ownerID);

	static InventoryItem getInventoryItem(long long objectID);
	static vector<long> getEquippedLOTs(long long ownerID);
	static vector<InventoryItem> getEquippedInventoryItems(long long ownerID);
	static vector<InventoryItem> getInventoryItems(long long ownerID);
	
	static short getNextFreeSlot(InventoryType invType, long long ownerID);
	static short getSlotFromItem(long long objectID, long long ownerID);
	static short getInventoryTypeFromItem(long long objectID);
	static long long InventoryItems::getItemFromSlot(long long ownerID, short inventory_type, short slot);
	static long long InventoryItems::getOwnerID(long long objectID);

	static void setOwnerID(long long ownerID, long long objectID);
	static void setItemType(ItemType itemType, long long objectID);
	static void setInventoryType(InventoryType invType, long long objectID);
	static void setSlot(short slot, long long objectID);
	static void setCount(long count, long long objectID);
	static void setIsBound(bool isBound, long long objectID);
	static void setIsEquipped(bool isEquipped, long long objectID);
};