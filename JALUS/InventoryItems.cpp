#include "InventoryItems.h"
#include "Config.h"
#include "Logger.h"
#include "Objects.h"
#include "Server.h"

string InventoryItems::name;

void InventoryItems::init(string name, string structure)
{
	InventoryItems::name = name;

	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "CREATE TABLE IF NOT EXISTS";
		ss << " " << name << " ";
		ss << "(";
		ss << structure;
		ss << ");";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		Logger::error("Failed to init SQLTable! (Name: " + name + ") Error: '" + string(x.ErrMessage()) + "'");
		Server::shutdown(-1);
	}
}

InventoryType InventoryItems::getInventoryType(ItemType itemType)
{
	switch (itemType)
	{

	case ITEM_TYPE_BRICK:
		return InventoryType::INVENTORY_TYPE_BRICKS;

	case ITEM_TYPE_HAT:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_HAIR:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_NECK:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_LEFT_HAND:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_RIGHT_HAND:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_LEGS:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_LEFT_TRINKET:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_RIGHT_TRINKET:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_BEHAVIOR:
		return InventoryType::INVENTORY_TYPE_BEHAVIORS;

	case ITEM_TYPE_PROPERTY:
		return InventoryType::_INVENTORY_TYPE_PROPERTIES; // ?

	case ITEM_TYPE_MODEL:
		return InventoryType::INVENTORY_TYPE_MODELS; // ?

	case ITEM_TYPE_COLLECTABLE:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_CONSUMABLE:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_CHEST:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_EGG:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_PET_FOOD:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_QUEST_OBJECT:
		return InventoryType::INVENTORY_TYPE_COMMENDATIONS;

	case ITEM_TYPE_PET_INVENTORY_ITEM:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	case ITEM_TYPE_PACKAGE:
		return InventoryType::INVENTORY_TYPE_DEFAULT;

	case ITEM_TYPE_LOOT_MODEL:
		return InventoryType::INVENTORY_TYPE_MODELS;

	case ITEM_TYPE_VEHICLE:
		return InventoryType::INVENTORY_TYPE_MODELS;

	case ITEM_TYPE_CURRENCY:
		return InventoryType::INVENTORY_TYPE_DEFAULT; // ?

	}
}

long long InventoryItems::createInventoryItem(long long ownerID, long lot, long count, bool isBound, bool isEquipped)
{
	long long id = Objects::createObject(lot);
	
	InventoryItems::createInventoryItem(id, ownerID, lot, count, isBound, isEquipped);

	return id;
}

void InventoryItems::createInventoryItem(long long objectID, long long ownerID, long lot, long count, bool isBound, bool isEquipped)
{
	SAConnection con;
	SACommand cmd;

	ItemType itemType = CDClient::getItemType(lot);
	InventoryType invType = InventoryItems::getInventoryType(itemType);
	short slot = InventoryItems::getNextFreeSlot(invType, ownerID);

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "INSERT INTO";
		ss << " " << InventoryItems::name << " ";
		ss << "(id, owner_id, item_type, inventory_type, slot, count, is_bound, is_equipped)";
		ss << " VALUES ";
		ss << "('" << objectID << "', '" << ownerID << "', '" << itemType << "', '" << invType << "', '" << slot << "', '" << count << "', '" << isBound << "', '" << isEquipped << "');";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::deleteInventoryItem(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "DELETE FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	Objects::deleteObject(objectID);
}

void InventoryItems::deleteInventoryItems(long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		vector<long long> ids;

		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT id FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE owner_id = '" << ownerID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			ids.push_back(cmd.Field("id").asNumeric());
		}

		if (ids.size() > 0)
		{
			ss.str("");

			ss << "DELETE FROM";
			ss << " " << InventoryItems::name << " ";
			ss << "WHERE id IN (";

			for (int i = 0; i < ids.size(); i++)
			{
				if (i == 0)
					ss << ids.at(i);
				else
					ss << ", " << ids.at(i);
			}

			ss << ");";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			ss.str("");

			ss << "DELETE FROM";
			ss << " " << Objects::name << " ";
			ss << "WHERE id IN (";

			for (int i = 0; i < ids.size(); i++)
			{
				if (i == 0)
					ss << ids.at(i);
				else
					ss << ", " << ids.at(i);
			}

			ss << ");";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

InventoryItem InventoryItems::getInventoryItem(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	InventoryItem r = InventoryItem();

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT owner_id, item_type, inventory_type, slot, count, is_bound, is_equipped FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		if (cmd.FetchFirst())
		{
			r.objectID = objectID;
			r.ownerID = cmd.Field("owner_id").asNumeric();
			r.itemType = (ItemType)cmd.Field("item_type").asLong();
			r.invType = (InventoryType)cmd.Field("inventory_type").asLong();
			r.slot = cmd.Field("slot").asLong();
			r.count = cmd.Field("count").asLong();
			r.isBound = cmd.Field("is_bound").asBool();
			r.isEquipped = cmd.Field("is_equipped").asBool();

			ss.str("");

			ss << "SELECT lot FROM";
			ss << " " << Objects::name << " ";
			ss << "WHERE id = '" << objectID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchFirst())
				r.lot = cmd.Field("lot").asLong();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

vector<long> InventoryItems::getEquippedLOTs(long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	vector<long> r;

	try
	{
		vector<long long> ids;

		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT id FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE owner_id = '" << ownerID << "' AND is_equipped = '1';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			ids.push_back(cmd.Field("id").asNumeric());
		}

		for (int i = 0; i < ids.size(); i++)
		{
			ss.str("");

			ss << "SELECT lot FROM";
			ss << " " << Objects::name << " ";
			ss << "WHERE id = '" << ids.at(i) << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchFirst())
				r.push_back(cmd.Field("lot").asLong());
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

vector<InventoryItem> InventoryItems::getEquippedInventoryItems(long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	vector<InventoryItem> r;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT id, item_type, inventory_type, slot, count, is_bound, is_equipped FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE owner_id = '" << ownerID << "' AND is_equipped = '1';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			InventoryItem item;

			item.objectID = cmd.Field("id").asNumeric();
			item.ownerID = ownerID;
			item.itemType = (ItemType)cmd.Field("item_type").asLong();
			item.invType = (InventoryType)cmd.Field("inventory_type").asLong();
			item.slot = cmd.Field("slot").asLong();
			item.count = cmd.Field("count").asLong();
			item.isBound = cmd.Field("is_bound").asBool();
			item.isEquipped = cmd.Field("is_equipped").asBool();

			r.push_back(item);
		}

		for (int i = 0; i < r.size(); i++)
		{
			ss.str("");

			ss << "SELECT lot FROM";
			ss << " " << Objects::name << " ";
			ss << "WHERE id = '" << r.at(i).objectID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchFirst())
				r.at(i).lot = cmd.Field("lot").asLong();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

vector<InventoryItem> InventoryItems::getInventoryItems(long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	vector<InventoryItem> r;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT id, item_type, inventory_type, slot, count, is_bound, is_equipped FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE owner_id = '" << ownerID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			InventoryItem item;

			item.objectID = cmd.Field("id").asNumeric();
			item.ownerID = ownerID;
			item.itemType = (ItemType)cmd.Field("item_type").asLong();
			item.invType = (InventoryType)cmd.Field("inventory_type").asLong();
			item.slot = cmd.Field("slot").asLong();
			item.count = cmd.Field("count").asLong();
			item.isBound = cmd.Field("is_bound").asBool();
			item.isEquipped = cmd.Field("is_equipped").asBool();

			r.push_back(item);
		}

		for (int i = 0; i < r.size(); i++)
		{
			ss.str("");

			ss << "SELECT lot FROM";
			ss << " " << Objects::name << " ";
			ss << "WHERE id = '" << r.at(i).objectID << "';";

			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			if (cmd.FetchFirst())
				r.at(i).lot = cmd.Field("lot").asLong();
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return r;
}

short InventoryItems::getNextFreeSlot(InventoryType invType, long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	long slot = 0;
	bool found = false;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		while (!found)
		{
			stringstream ss;
			ss << "SELECT id FROM";
			ss << " " << InventoryItems::name << " ";
			ss << "WHERE owner_id = '" << ownerID << "' AND inventory_type = '" << invType << "' AND slot = '" << slot << "';";

			cmd.setConnection(&con);
			cmd.setCommandText(ss.str().c_str());
			cmd.Execute();

			found = (!cmd.FetchFirst());

			if (!found)
				slot++;
		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return slot;
}

short InventoryItems::getSlotFromItem(long long objectID, long long ownerID)
{
	SAConnection con;
	SACommand cmd;

	short slot = 0;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		// SELECT slot, inventory_type FROM `inventoryitems` WHERE owner_id = 1338408125224744078 AND id = 1023823589281344817
		stringstream ss;
		ss << "SELECT slot FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE id = '" << objectID << "' AND owner_id = '" << ownerID << "';";
		//ss << "WHERE owner_id = '" << ownerID << "' AND inventory_type = '" << invType << "' AND slot = '" << slot << "';";
		
		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		
		while (cmd.FetchNext())
		{
			//short whichSlot = cmd.Field("slot").asShort();

			//r.push_back(cmd.Field("RowCount").asLong());
			slot = cmd.Field("slot").asShort();


		}

		

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return slot;
}

short InventoryItems::getInventoryTypeFromItem(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	short inventory_type;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		// SELECT slot, inventory_type FROM `inventoryitems` WHERE owner_id = 1338408125224744078 AND id = 1023823589281344817
		stringstream ss;
		ss << "SELECT inventory_type FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE id = '" << objectID << "';";
		//ss << "WHERE owner_id = '" << ownerID << "' AND inventory_type = '" << invType << "' AND slot = '" << slot << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();


		while (cmd.FetchNext())
		{
			//short whichSlot = cmd.Field("slot").asShort();

			//r.push_back(cmd.Field("RowCount").asLong());
			inventory_type = cmd.Field("inventory_type").asShort();


		}



		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return inventory_type;
}

long long InventoryItems::getItemFromSlot(long long ownerID, short inventory_type, short slot)
{
	SAConnection con;
	SACommand cmd;

	long long id;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		// SELECT slot, inventory_type FROM `inventoryitems` WHERE owner_id = 1338408125224744078 AND id = 1023823589281344817
		stringstream ss;
		ss << "SELECT id FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE owner_id = '" << ownerID << "' AND inventory_type = '" << inventory_type << "' AND slot = '" << slot << "';";
		//ss << "WHERE owner_id = '" << ownerID << "' AND slot = '" << slot << "' AND inventory_type'" << inventory_type << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();


		while (cmd.FetchNext())
		{
			//short whichSlot = cmd.Field("slot").asShort();

			//r.push_back(cmd.Field("RowCount").asLong());
			id = cmd.Field("id").asNumeric();


		}



		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return id;
}

long long InventoryItems::getOwnerID(long long objectID)
{
	SAConnection con;
	SACommand cmd;

	long long owner_id = 0;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "SELECT owner_id FROM";
		ss << " " << InventoryItems::name << " ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		while (cmd.FetchNext())
		{
			//short whichSlot = cmd.Field("slot").asShort();

			//r.push_back(cmd.Field("RowCount").asLong());
			owner_id = cmd.Field("owner_id").asNumeric();


		}

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}

	return owner_id;
}

void InventoryItems::setOwnerID(long long ownerID, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET owner_id = '" << ownerID << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setItemType(ItemType itemType, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET item_type = '" << itemType << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setInventoryType(InventoryType invType, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET inventory_type = '" << invType << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setSlot(short slot, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET slot = '" << slot << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setCount(long count, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET count = '" << count << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setIsBound(bool isBound, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET is_bound = '" << isBound << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

void InventoryItems::setIsEquipped(bool isEquipped, long long objectID)
{
	SAConnection con;
	SACommand cmd;

	try
	{
		con.Connect((Config::getMySQLHost() + "@" + Config::getMySQLDatabase()).c_str(),
			Config::getMySQLUsername().c_str(),
			Config::getMySQLPassword().c_str(),
			SA_MySQL_Client);

		stringstream ss;
		ss << "UPDATE";
		ss << " " << InventoryItems::name << " ";
		ss << "SET is_equipped = '" << isEquipped << "' ";
		ss << "WHERE id = '" << objectID << "';";

		cmd.setConnection(&con);
		cmd.setCommandText(ss.str().c_str());
		cmd.Execute();

		con.Commit();
		con.Disconnect();
	}
	catch (SAException &x)
	{
		try
		{
			con.Rollback();
		}
		catch (SAException &) {}
	}
}

