#include "Commands.h"
#include "ServerLoop.h"
#include "Accounts.h"
#include "SHA512.h"
#include "TimeUtils.h"
#include "Bans.h"
#include "CDClient.h"
#include "ObjectsManager.h"
#include "Chat.h"
#include "PacketUtils.h"
#include "Characters.h"
#include "Validate.h"
#include "Objects.h"
#include "GameMessages.h"
#include "Config.h"
#include "TransitionInfos.h"
#include "Sessions.h"
#include "General.h"
#include "PingTool.h"
#include "LUZCache.h"
#include "Scheduler.h"
#include "Helpers.h"
#include "Missions.h"
#include "Common.h"
#include "ValueStorage.h"

map<string, Command*> Commands::commands = map<string, Command*>();
bool Commands::isLoaded = false;

void Commands::performCommand(CommandSender sender, string cmd, vector<string> args)
{
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));

	if (!isLoaded) {
		loadCommands();
	}

	if (commands.find(cmd) == commands.end()) {
		// Command not found
		sender.sendMessage("The input '" + cmd + "' is not a valid command!");
	}
	else {
		commands.at(cmd)->execute(sender, cmd, args);
	}
}

void Commands::addCommand(string name, Command* command) {
	commands.insert({ name, command });
}

void Commands::addCommand(Command* command) {
	for (int i = command->getCommandNames().size(); i--;) {
		addCommand(command->getCommandNames()[i], command);
	}
}

void Commands::loadCommands() {
	addCommand(new Ignore());
	addCommand(new StopCommand());
	addCommand(new TestCommand());
	addCommand(new WispCommand());
	addCommand(new SetAdmin());
	addCommand(new Emote());
	addCommand(new List());
	addCommand(new InvSize());
	addCommand(new Equipment());
	addCommand(new unequipall());
	addCommand(new setattr());
	addCommand(new addskill());
	addCommand(new removeskill());
	addCommand(new sendchat());
	addCommand(new setcoins());
	addCommand(new fly());
	addCommand(new gravity());
	addCommand(new gmadditem());
	addCommand(new addmission());
	addCommand(new completemission());
	addCommand(new ping());
	addCommand(new createaccount());
	addCommand(new deleteaccount());
	addCommand(new banaccount());
	addCommand(new banip());
	addCommand(new spawn());
	addCommand(new despawn());
	addCommand(new nearme());
	addCommand(new teleport());
	addCommand(new testmap());
	isLoaded = true;
}

CommandSender::CommandSender(long long senderID, SystemAddress clientAddress)
{
	this->senderID = senderID;
	this->clientAddress = clientAddress;
}

void CommandSender::sendMessage(string msg)
{
	if (senderID == -1)
	{
		cout << msg << endl;
	}
	else
	{
		Chat::sendChatMessage(to_wstring(msg), clientAddress);
	}
}

long long CommandSender::getSenderID()
{
	return senderID;
}

SystemAddress CommandSender::getClientAddress()
{
	return clientAddress;
}

vector<string> Command::getCommandNames() {
	vector<string> names;
	return names;
}

void Command::execute(CommandSender sender, string cmd, vector<string> args) { }

void TestCommand::execute(CommandSender sender, string cmd, vector<string> args) {
	sender.sendMessage("Test command stuffs");
}

void StopCommand::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 6) {
		ServerLoop::stop();
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void WispCommand::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		bool value = ValueStorage::getValueFromDatabase(sender.getSenderID(), "wisp");
		ValueStorage::updateValueInDatabase(sender.getSenderID(), "wisp", !value);
		ValueStorage::updateValueInMemory(sender.getSenderID(), "wisp", !value);
		sender.sendMessage("Wisp lee effect now set to " + toUpperCase(to_string(!value)) + ". Give it a few seconds to update.");
	}
}

void SetAdmin::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 9) {
		if (sender.getSenderID() != -1)
		{
			if (args.size() >= 2) {
				long level = -1;
				long long account = Characters::getAccountID(Characters::getCharacterID(args.at(0)));
				string username = Accounts::getUsername(account);
				try {
					level = stol(args.at(1));
					//sender.sendMessage("Set GM level: (Data: '" + args.at(2) + "')");
				}
				catch (exception& e) {}

				if (account != -1 && level != -1) {
					if (Characters::getAccountID(sender.getSenderID()) != account) {
						Accounts::setGMLevel(level, account);
						sender.sendMessage("Set " + args.at(0) + "'s GM Level to " + to_string(level));
					}
					else
						sender.sendMessage("Could not set own account level. You are GM Level " + to_string(Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()))));
				}
				else {
					if (username != "")
						sender.sendMessage("Could not set level for player account " + username);
					else
						sender.sendMessage("Could not set account level for player " + args.at(0));
				}
			}
			else if (args.size() == 1)
				sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <player> <gmlevel>");
			else
				sender.sendMessage("You are GM Level " + to_string(Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()))));
		}
	}
	else
		sender.sendMessage("No permission.");
}

void Emote::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() >= 1) {
			if (CDClient::isAnimationValid(args.at(0)))
				if (args.at(0).find("horse") == std::string::npos)
					Helpers::broadcastAnimation(sender.getSenderID(), args.at(0));
				else
					sender.sendMessage(args.at(0) + " is not a valid animation!");
			else
				sender.sendMessage(args.at(0) + " is not a valid animation!");
		}
	}
}

void List::execute(CommandSender sender, string cmd, vector<string> args) {
	int count = Server::getReplicaManager()->GetParticipantCount();
	string list = "There are currently ";
	list.append(to_string(count));
	list.append(" player(s) online: ");
	for (int k = 0; k < count; k++)
	{
		if (count == 1) {
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
			list.append(to_string(ObjectsManager::getObjectBySystemAddress(participant)->name) + ".");
		}
		else if (count == 2) {
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
			list.append(to_string(ObjectsManager::getObjectBySystemAddress(participant)->name));
			if (k == (count - 2))
				list.append(" and ");
			else if (k == (count - 1))
				list.append(".");
		}
		else {
			if (k == (count - 1))
				list.append("and ");
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
			list.append(to_string(ObjectsManager::getObjectBySystemAddress(participant)->name));
			if (k < (count - 1))
				list.append(", ");
			else
				list.append(".");
		}
	}

	sender.sendMessage(list);
}

void InvSize::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			//ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());
			long newMaxInventory = stol(args.at(0));
			Characters::setMaxInventory(newMaxInventory, sender.getSenderID());
			GameMessages::setInventorySize(sender.getSenderID(), InventoryType::INVENTORY_TYPE_DEFAULT, newMaxInventory, sender.getClientAddress());
		}
	}
}

void Equipment::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());
		sender.sendMessage(to_string(player->inventoryIndex->items.size()) + " items.");
	}
}

void unequipall::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());
		sender.sendMessage(to_string(player->inventoryIndex->items.size()) + " items.");
		for (int k = player->inventoryIndex->items.size(); k--;)
		{
			Helpers::unequip(sender.getSenderID(), player->inventoryIndex->items[k].objectID, sender.getClientAddress());
		}
		sender.sendMessage(to_string(player->inventoryIndex->items.size()) + " items.");
		ObjectsManager::serializeObject(player);
	}
}

void setattr::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());
		if (args.size() == 2)
		{
			if (args.at(0) == "health")
			{
				player->statsIndex->cur_health = stoul(args.at(1));
			}
			else if (args.at(0) == "maxHealth")
			{
				player->statsIndex->max_health = stoul(args.at(1));
			}
			else if (args.at(0) == "armor")
			{
				player->statsIndex->cur_armor = stoul(args.at(1));
			}
			else if (args.at(0) == "maxArmor")
			{
				player->statsIndex->max_armor = stoul(args.at(1));
			}
			else if (args.at(0) == "imagi" || args.at(0) == "imagination")
			{
				player->statsIndex->cur_imagination = stoul(args.at(1));
			}
			else if (args.at(0) == "maxImagi" || args.at(0) == "maxImagination")
			{
				player->statsIndex->max_imagination = stoul(args.at(1));
			}
			ObjectsManager::serializeObject(player);
		}
		else if (args.size() == 0)
		{
			sender.sendMessage("/setattr [health, armor, imagi] [value]");
			sender.sendMessage("Sets the specified stat to a temporary value.");
		}
		else if (args.size() == 1)
		{
			sender.sendMessage("You need to specify a value!");
		}
	}
}

void addskill::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			Helpers::addSkill(sender.getSenderID(), stol(args.at(0)), sender.getClientAddress());
		}
	}
}

void removeskill::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			Helpers::removeSkill(sender.getSenderID(), stol(args.at(0)), sender.getClientAddress());
		}
	}
}

void sendchat::execute(CommandSender sender, string cmd, vector<string> args) {
	if (args.size() >= 1)
	{
		std::string globalChatMsg;
		for (int k = 0; k < args.size(); k++)
		{
			globalChatMsg.append(args.at(k) + " ");
		}

		Helpers::sendGlobalChat(to_wstring(globalChatMsg));
	}

	else
		sender.sendMessage("You need to say something!");
}

void setcoins::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			//Session* session = Sessions::getSession(sender.getClientAddress());
			ReplicaObject* replica = ObjectsManager::getObjectByID(sender.getSenderID());
			long long newCurrency = stoll(args.at(0));
			Characters::setCurrency(newCurrency, sender.getSenderID());
			Position position;
			position.x = replica->controllablePhysicsIndex->pos_x;
			position.y = replica->controllablePhysicsIndex->pos_y;
			position.z = replica->controllablePhysicsIndex->pos_z;

			GameMessages::setCurrency(sender.getSenderID(), newCurrency, position, sender.getClientAddress());
		}
		else
		{
			sender.sendMessage("You need to specify an amount!");
		}
	}
}

void fly::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		bool enabled = true;
		bool bBypassChecks = true; //default: false
		bool bDoHover = false;
		bool bUse = true;
		//int effectID = 158; //default: -1
		int effectID = -1; //default: -1
		float fAirspeed = 10;
		float fMaxAirspeed = 15;
		float fVertVel = 1;
		int iWarningEffectID = -1;

		if (args.size() == 1)
		{
			if (args.at(0) == "off" || stoi(args.at(0)) == 0)
			{
				bUse = false;
			}
			/*else
			{
			fAirspeed = stof(args.at(0));
			fMaxAirspeed = fAirspeed + 5;
			}*/
		}
		else
		{
			if (iequals(cmd, "hover"))
			{
				bDoHover = true;
			}
			bUse = true;
		}



		if (enabled == true)
		{
			Session* session = Sessions::getSession(sender.getClientAddress());

			BitStream* enableJetpack = PacketUtils::createGMBase(sender.getSenderID(), GAME_MESSAGE_ID_SET_JETPACK_MODE);
			enableJetpack->Write((bool)bBypassChecks);
			enableJetpack->Write((bool)bDoHover);
			enableJetpack->Write((bool)bUse);
			enableJetpack->Write((int)effectID); // effectID
			enableJetpack->Write((float)fAirspeed); // fAirspeed
			enableJetpack->Write((float)fMaxAirspeed); // fMaxAirspeed
			enableJetpack->Write((float)fVertVel); // fVertVel
			enableJetpack->Write((int)iWarningEffectID); // iWarningEffectID

			Server::sendPacket(enableJetpack, sender.getClientAddress());
			//Server::broadcastPacket(enableJetpack, sender.getClientAddress());
			/*for (int k = 0; k < Server::getReplicaManager()->GetParticipantCount(); k++)
			{
			SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(k);
			Server::sendPacket(enableJetpack, participant);
			//Server::broadcastPacket(enableJetpack, sender.getClientAddress());
			}*/
		}
		else
		{
			sender.sendMessage("This response should never appear so something might be wrong.");
		}
	}
}

void gravity::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			Session* session = Sessions::getSession(sender.getClientAddress());
			BitStream* updateGravity = PacketUtils::createGMBase(sender.getSenderID(), GAME_MESSAGE_ID_SET_GRAVITY_SCALE);
			sender.sendMessage("Gravity set to: " + args.at(0));
			float grav = stof(args.at(0));

			updateGravity->Write((float)grav);

			Server::sendPacket(updateGravity, sender.getClientAddress());
		}
	}
}

void gmadditem::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1 || args.size() == 2)
		{
			string strLOT = args.at(0);
			long amount = 1;

			if (args.size() == 2)
				amount = stol(args.at(1));

			if (amount == 1)
			{
				long lot = stol(strLOT);

				Helpers::createSyncedItemStack(sender.getSenderID(), lot, amount, true, true, sender.getClientAddress());
			}
			//else 
		}
	}
}

void addmission::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			long long missionID = stoll(args.at(0));


			Helpers::addMissionWithTasks(missionID, sender.getSenderID());


		}
	}
}

void completemission::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			long missionID = stoll(args.at(0));

			Missions::completeMission(missionID, sender.getSenderID(), sender.getClientAddress());

		}
	}
}

void ping::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 2)
		{
			string address = args.at(0);
			string strPort = args.at(1);
			unsigned short port = 0;

			if (Validate::isValidS32(strPort))
			{
				long p = stol(strPort);

				if (p > 0 && p <= USHRT_MAX)
				{
					port = p;
				}
			}

			if (port > 0)
			{
				SystemAddress clientAddress = sender.getClientAddress();

				sender.sendMessage("Trying to reach " + address + ":" + to_string(port) + "...");

				async([address, port, clientAddress]() {
					bool b = PingTool::ping(address.c_str(), port, "3.25 ND1", 8);

					Session* session = Sessions::getSession(clientAddress);
					if (session != nullptr)
					{
						Chat::sendChatMessage(L"PingResult received!\n{\n   Address: " + to_wstring(address) + L",\n   Port: " + to_wstring(port) + L",\n   IsReachable: " + to_wstring(b) + L"\n}", clientAddress);
					}
				});
			}
			else
				sender.sendMessage("The second argument has to be a valid unsigned short int!");
		}
		else
			sender.sendMessage("Pong!");
		//sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <address> <port>");
	}
}

void createaccount::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 9) {
		if (args.size() >= 2)
		{
			string username = args.at(0);
			string password = args.at(1);

			long long accountID = Accounts::getAccountID(username);
			if (accountID == -1)
			{
				Accounts::createAccount(username, sha512(password));
				sender.sendMessage("Successfully created account! (Username: " + username + ")");
				if (args.size() == 3) {
					try {
						Accounts::setGMLevel(stol(args.at(2)), accountID);
						//sender.sendMessage("Set GM level: (Data: '" + args.at(2) + "')");
					}
					catch (exception& e) {
						string error(e.what());
						sender.sendMessage("Failed to set GM level: (Data: '" + error + "')");
					}
				}
			}
			else
				sender.sendMessage("The username '" + username + "' is already in use!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <username> <password> <gmlevel>");
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void deleteaccount::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 9) {
		if (args.size() == 1)
		{
			string username = args.at(0);

			long long accountID = Accounts::getAccountID(username);
			if (accountID != -1)
			{
				Accounts::deleteAccount(accountID);
				sender.sendMessage("Successfully deleted account! (Username: " + username + ")");
			}
			else
				sender.sendMessage("There is no account with the username '" + username + "'!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <username>");
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void banaccount::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 9) {
		if (args.size() == 3)
		{
			string username = args.at(0);
			string time = args.at(1);
			string reason = args.at(2);

			long long accountID = Accounts::getAccountID(username);
			if (accountID != -1)
			{
				long long howLong = TimeUtils::parse(time);
				if (howLong != -2)
				{
					Bans::ban(to_string(accountID), Characters::getAccountID(sender.getSenderID()), false, reason, howLong);
					sender.sendMessage(username + " got banned successfully! (Duration: " + Bans::toTimeString(howLong) + ") (Reason: " + reason + ")");

					if (Server::isWorldInstance())
					{
						for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
						{
							SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

							if (ObjectsManager::getObjectBySystemAddress(participant)->characterIndex->account_id == accountID)
							{
								BitStream* response = PacketUtils::createPacketBase(RCT_GENERAL, GENERAL_PACKET_DISCONNECT_NOTIFY);
								response->Write(DisconnectID::DISCONNECT_KICK);
								response->Write((unsigned long)0);
								Server::sendPacket(response, participant);
							}
						}
					}
				}
				else
					sender.sendMessage("This command requires a valid time specification! Example: '/banAccount ExampleUsername 120sec \"Example Reason\"'");
			}
			else
				sender.sendMessage("There is no account with the username '" + username + "'!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <username> <time> <reason>");
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void banip::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 9) {
		if (args.size() == 3)
		{
			string ip = args.at(0);
			string time = args.at(1);
			string reason = args.at(2);

			if (isValidIPAddress(ip.c_str()))
			{
				long long howLong = TimeUtils::parse(time);
				if (howLong != -2)
				{
					Bans::ban(ip, Characters::getAccountID(sender.getSenderID()), true, reason, howLong);
					sender.sendMessage(ip + " got banned successfully! (Duration: " + Bans::toTimeString(howLong) + ") (Reason: " + reason + ")");

					if (Server::isWorldInstance())
					{
						for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
						{
							SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

							if (string(participant.ToString(false)) == ip)
							{
								BitStream* response = PacketUtils::createPacketBase(RCT_GENERAL, GENERAL_PACKET_DISCONNECT_NOTIFY);
								response->Write(DisconnectID::DISCONNECT_KICK);
								response->Write((unsigned long)0);
								Server::sendPacket(response, participant);
							}
						}
					}
				}
				else
					sender.sendMessage("This command requires a valid time specification! Example: '/banIP 127.0.0.1 120sec \"Example Reason\"'");
			}
			else
				sender.sendMessage("'" + ip + "' is not a valid IP address!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <ip> <time> <reason>");
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void spawn::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (account_gm_level >= 5) {
		if (sender.getSenderID() != -1)
		{
			if (args.size() == 1 || args.size() == 2)
			{
				string strLOT = args.at(0);
				string name = "";

				if (args.size() == 2)
					name = args.at(1);

				if (Validate::isValidS32(strLOT))
				{
					long lot = stol(strLOT);

					ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());

					Position pos = Position();
					pos.x = player->controllablePhysicsIndex->pos_x;
					pos.y = player->controllablePhysicsIndex->pos_y;
					pos.z = player->controllablePhysicsIndex->pos_z;

					Rotation rot = Rotation();
					rot.x = player->controllablePhysicsIndex->rot_x;
					rot.y = player->controllablePhysicsIndex->rot_y;
					rot.z = player->controllablePhysicsIndex->rot_z;
					rot.w = player->controllablePhysicsIndex->rot_w;

					long long id = Objects::generateObjectID();
					ReplicaObject* other = new ReplicaObject(id, lot, to_wstring(name), 0, pos, rot);
					ObjectsManager::spawnObject(other);

					if (name == "")
					{
						sender.sendMessage("You successfully spawned a new object!\n{\n   LOT: " + to_string(lot) + ",\n   ObjectID: " + to_string(id) + "\n}");
					}
					else
					{
						sender.sendMessage("You successfully spawned a new object!\n{\n   LOT: " + to_string(lot) + ",\n   ObjectID: " + to_string(id) + ",\n   Name: " + name + "\n}");
					}
				}
				else
					sender.sendMessage("The first argument has to be a valid long int!");
			}
			else
				sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <LOT> [name]");
		}
		else
			sender.sendMessage("You can't use this command here!");
	}
	else {
		sender.sendMessage("No permission.");
	}
}

void despawn::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			string strObjectID = args.at(0);

			if (Validate::isValidS64(strObjectID))
			{
				long long objectID = stoll(strObjectID);

				ReplicaObject* replica = ObjectsManager::getObjectByID(objectID);

				if (replica != nullptr)
				{
					long lot = replica->lot;
					string name = to_string(replica->name);

					ObjectsManager::despawnObject(replica);

					if (name == "")
					{
						sender.sendMessage("You successfully despawned an object!\n{\n   LOT: " + to_string(lot) + ",\n   ObjectID: " + to_string(objectID) + "\n}");
					}
					else
					{
						sender.sendMessage("You successfully despawned an object!\n{\n   LOT: " + to_string(lot) + ",\n   ObjectID: " + to_string(objectID) + ",\n   Name: " + name + "\n}");
					}
				}
				else
					sender.sendMessage("There's no object with ObjectID " + to_string(objectID) + "!");
			}
			else
				sender.sendMessage("The first argument has to be a valid long long int!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <ObjectID>");
	}
	else
		sender.sendMessage("You can't use this command here!");
}

void nearme::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			string strRadius = args.at(0);

			if (Validate::isValidFloat(strRadius))
			{
				float radius = stof(strRadius);

				ReplicaObject* player = ObjectsManager::getObjectByID(sender.getSenderID());

				float min_x = player->controllablePhysicsIndex->pos_x - radius;
				float min_y = player->controllablePhysicsIndex->pos_y - radius;
				float min_z = player->controllablePhysicsIndex->pos_z - radius;

				float max_x = player->controllablePhysicsIndex->pos_x + radius;
				float max_y = player->controllablePhysicsIndex->pos_y + radius;
				float max_z = player->controllablePhysicsIndex->pos_z + radius;

				vector<ReplicaObject*> match = vector<ReplicaObject*>();

				for (int i = 0; i < Server::getReplicaManager()->GetReplicaCount(); i++)
				{
					ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);

					if (other->clientAddress == UNASSIGNED_SYSTEM_ADDRESS)
					{
						if (other->controllablePhysicsIndex != nullptr)
						{
							ControllablePhysicsIndex* index = other->controllablePhysicsIndex;

							if (index->pos_x >= min_x && index->pos_x <= max_x)
							{
								//sender.sendMessage("x Coords: index" + std::to_string(index->pos_x) + ", min " + std::to_string(min_x) + ", and max " + std::to_string(max_x));
								if (index->pos_y >= min_y && index->pos_y <= max_y)
								{
									//sender.sendMessage("y Coords: index" + std::to_string(index->pos_y) + ", min " + std::to_string(min_y) + ", and max " + std::to_string(max_y));
									if (index->pos_z >= min_z && index->pos_z <= max_z)
									{
										//sender.sendMessage("z Coords: index" + std::to_string(index->pos_z) + ", min " + std::to_string(min_z) + ", and max " + std::to_string(max_z));
										match.push_back(other);
									}
								}
							}
						}

						if (other->simplePhysicsIndex != nullptr)
						{
							SimplePhysicsIndex* index = other->simplePhysicsIndex;

							if (index->pos_x >= min_x && index->pos_x <= max_x)
							{
								//sender.sendMessage("x Coords: index" + std::to_string(index->pos_x) + ", min " + std::to_string(min_x) + ", and max " + std::to_string(max_x));
								if (index->pos_y >= min_y && index->pos_y <= max_y)
								{
									//sender.sendMessage("y Coords: index" + std::to_string(index->pos_y) + ", min " + std::to_string(min_y) + ", and max " + std::to_string(max_y));
									if (index->pos_z >= min_z && index->pos_z <= max_z)
									{
										//sender.sendMessage("z Coords: index" + std::to_string(index->pos_z) + ", min " + std::to_string(min_z) + ", and max " + std::to_string(max_z));
										match.push_back(other);
									}
								}
							}
						}

						if (other->phantomPhysicsIndex != nullptr)
						{
							PhantomPhysicsIndex* index = other->phantomPhysicsIndex;

							if (index->pos_x >= min_x && index->pos_x <= max_x)
							{
								//sender.sendMessage("x Coords: index" + std::to_string(index->pos_x) + ", min " + std::to_string(min_x) + ", and max " + std::to_string(max_x));
								if (index->pos_y >= min_y && index->pos_y <= max_y)
								{
									//sender.sendMessage("y Coords: index" + std::to_string(index->pos_y) + ", min " + std::to_string(min_y) + ", and max " + std::to_string(max_y));
									if (index->pos_z >= min_z && index->pos_z <= max_z)
									{
										//sender.sendMessage("z Coords: index" + std::to_string(index->pos_z) + ", min " + std::to_string(min_z) + ", and max " + std::to_string(max_z));
										match.push_back(other);
									}
								}
							}
						}
					}
				}

				if (match.size() > 0)
				{
					sender.sendMessage("The following objects are around you in a radius of " + strRadius + ":");

					for (int i = 0; i < match.size(); i++)
					{
						if (match.at(i)->name == L"")
						{
							sender.sendMessage("{\n   ListID: " + to_string(i + 1) + ",\n   LOT: " + to_string(match.at(i)->lot) + ",\n   ObjectID: " + to_string(match.at(i)->objectID) + "\n}");
						}
						else
						{
							sender.sendMessage("{\n   ListID: " + to_string(i + 1) + ",\n   LOT: " + to_string(match.at(i)->lot) + ",\n   ObjectID: " + to_string(match.at(i)->objectID) + ",\n   Name: " + to_string(match.at(i)->name) + "\n}");
						}
					}
				}
				else
					sender.sendMessage("There are no objects around you in a radius of " + strRadius + "!");
			}
			else
				sender.sendMessage("The first argument has to be a valid float!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <radius>");
	}
	else
		sender.sendMessage("You can't use this command here!");
}

void teleport::execute(CommandSender sender, string cmd, vector<string> args) {
	long account_gm_level = Accounts::getGMLevel(Characters::getAccountID(sender.getSenderID()));
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1 || args.size() == 3)
		{
			if (args.size() == 1)
			{
				string name = args.at(0);

				ReplicaObject* f = nullptr;

				for (int i = 0; i < Server::getReplicaManager()->GetReplicaCount(); i++)
				{
					ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);

					if (other->clientAddress != UNASSIGNED_SYSTEM_ADDRESS)
					{
						if (other->name == to_wstring(name))
							f = other;
					}
				}

				if (f != nullptr)
				{
					Position pos = Position();
					pos.x = f->controllablePhysicsIndex->pos_x;
					pos.y = f->controllablePhysicsIndex->pos_y;
					pos.z = f->controllablePhysicsIndex->pos_z;

					Rotation rot = Rotation();
					rot.x = f->controllablePhysicsIndex->rot_x;
					rot.y = f->controllablePhysicsIndex->rot_y;
					rot.z = f->controllablePhysicsIndex->rot_z;
					rot.w = f->controllablePhysicsIndex->rot_w;

					for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					{
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

						GameMessages::teleport(sender.getSenderID(), false, false, true, true, pos, participant, rot);
					}

					sender.sendMessage("You magically appeared at " + to_string(f->name) + "'s location!");
				}
				else
				{
					name = toLowerCase(name);

					vector<ReplicaObject*> match = vector<ReplicaObject*>();

					for (int i = 0; i < Server::getReplicaManager()->GetReplicaCount(); i++)
					{
						ReplicaObject* other = (ReplicaObject*)Server::getReplicaManager()->GetReplicaAtIndex(i);

						if (other->clientAddress != UNASSIGNED_SYSTEM_ADDRESS)
						{
							if (startsWith(toLowerCase(to_string(other->name)), name))
							{
								match.push_back(other);
							}
						}
					}

					if (match.size() == 0)
					{
						sender.sendMessage("Player not found!");
					}
					else if (match.size() == 1)
					{
						f = match.at(0);

						Position pos = Position();
						pos.x = f->controllablePhysicsIndex->pos_x;
						pos.y = f->controllablePhysicsIndex->pos_y;
						pos.z = f->controllablePhysicsIndex->pos_z;

						Rotation rot = Rotation();
						rot.x = f->controllablePhysicsIndex->rot_x;
						rot.y = f->controllablePhysicsIndex->rot_y;
						rot.z = f->controllablePhysicsIndex->rot_z;
						rot.w = f->controllablePhysicsIndex->rot_w;

						for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
						{
							SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

							GameMessages::teleport(sender.getSenderID(), false, false, true, true, pos, participant, rot);
						}

						sender.sendMessage("You magically appeared at " + to_string(f->name) + "'s location!");
					}
					else
					{
						sender.sendMessage(to_string(match.size()) + " players found! Please be a bit more specific!");
					}
				}
			}

			if (args.size() == 3)
			{
				string strX = args.at(0);
				string strY = args.at(1);
				string strZ = args.at(2);

				if (Validate::isValidFloat(strX) && Validate::isValidFloat(strY) && Validate::isValidFloat(strZ))
				{
					Position pos = Position();
					pos.x = stof(strX);
					pos.y = stof(strY);
					pos.z = stof(strZ);

					for (int i = 0; i < Server::getReplicaManager()->GetParticipantCount(); i++)
					{
						SystemAddress participant = Server::getReplicaManager()->GetParticipantAtIndex(i);

						GameMessages::teleport(sender.getSenderID(), false, false, false, true, pos, participant);
					}

					sender.sendMessage("You magically appeared at the specified location!");
				}
				else
					sender.sendMessage("All 3 arguments have to be valid floats!");
			}
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " (<player> | <x> <y> <z>)");
	}
	else
		sender.sendMessage("You can't use this command here!");
}

void testmap::execute(CommandSender sender, string cmd, vector<string> args) {
	if (sender.getSenderID() != -1)
	{
		if (args.size() == 1)
		{
			string strZoneID = args.at(0);

			ZoneID zoneID = ZoneIDs::fromString(strZoneID);

			if (zoneID != ZoneID::INVALID_ZONE_ID)
			{
				if (zoneID != ServerRoles::toZoneID(Server::getServerRole()))
				{
					SystemAddress clientAddress = sender.getClientAddress();
					string address = Config::getWorldInstanceAddress(zoneID);
					unsigned short port = Config::getWorldInstancePort(zoneID);

					sender.sendMessage("Trying to reach " + ServerRoles::toString(ServerRoles::fromZoneID(zoneID)) + "...");

					async([address, port, zoneID, clientAddress]()
					{
						bool b = PingTool::ping(address.c_str(), port, "3.25 ND1", 8);

						if (b)
						{
							Session* session = Sessions::getSession(clientAddress);
							if (session != nullptr)
							{
								Chat::sendChatMessage(L"Alright! Let's go to " + to_wstring(ZoneIDs::toPrintString(zoneID)) + L"!", clientAddress);

								if (zoneID != ZoneID::INVALID_ZONE_ID)
								{
									Location loc;

									loc.zoneID = zoneID;
									loc.mapClone = 0;

									Position spp = LUZCache::getByZoneID(loc.zoneID)->spawnPointPos;
									Rotation spr = LUZCache::getByZoneID(loc.zoneID)->spawnPointRot;

									loc.position.x = spp.x;
									loc.position.y = spp.y;
									loc.position.z = spp.z;
									loc.rotation.x = spr.x;
									loc.rotation.y = spr.y;
									loc.rotation.z = spr.z;
									loc.rotation.w = spr.w;

									Locations::saveLocation(loc, session->charID);
								}

								TransitionInfos::insertTransitionInfo(session->clientAddress, session->accountID, session->charID, session->transitionKey);
								General::redirectToServer(session->clientAddress, address, port, false);
								session->gotRedirected = true;
							}
						}
						else
						{
							Chat::sendChatMessage(L"The instance you want to connect to is not available at the moment!\nPlease try again later.", clientAddress);
						}
					});
				}
				else
					sender.sendMessage("You already are in/on " + ZoneIDs::toPrintString(zoneID) + "!");
			}
			else
				sender.sendMessage("The input '" + strZoneID + "' is not a valid ZoneID or ZoneName!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " (<ZoneID> | <ZoneName>)");
	}
	else
		sender.sendMessage("You can't use this command here!");
}