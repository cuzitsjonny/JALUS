#pragma once
#include "Common.h"
#include <map>

class CommandSender
{
private:
	long long senderID;
	
	SystemAddress clientAddress;
public:
	CommandSender(long long senderID, SystemAddress clientAddress = UNASSIGNED_SYSTEM_ADDRESS);

	void sendMessage(string msg);

	long long getSenderID();
	SystemAddress getClientAddress();
};

class Command
{
public:
	virtual vector<string> getCommandNames();
	virtual void execute(CommandSender sender, string cmd, vector<string> args);
};

class Commands
{
private:
	static map<string, Command*> commands;
	static bool isLoaded;
public:
	static void addCommand(string name, Command* command);
	static void addCommand(Command* command);
	static void loadCommands();
	static void performCommand(CommandSender sender, string cmd, vector<string> args);
};

class Ignore : public Command
{
public:
	vector<string> getCommandNames() { // These are client side commands.
		// We ignore them on the server side.
		// This list includes command aliases.
		// Thanks to Simon for getting me a list
		// of all client client sided commands.
		vector<string> names = vector<string>();
		// General
		names.push_back("quit");
		names.push_back("exit");
		names.push_back("logoutcharacter");
		names.push_back("camp");
		names.push_back("logoutaccount");
		names.push_back("logout");
		names.push_back("say");
		names.push_back("s");
		names.push_back("whisper");
		names.push_back("w");
		names.push_back("tell");
		names.push_back("team");
		names.push_back("t");
		names.push_back("location");
		names.push_back("locate");
		names.push_back("loc");
		names.push_back("faq");
		names.push_back("faqs");
		names.push_back("shop");
		names.push_back("store");
		names.push_back("minigames");
		names.push_back("forums");
		// Emotes
		names.push_back("thumbsup");
		names.push_back("thumb");
		names.push_back("victory");
		names.push_back("backflip");
		names.push_back("clap");
		names.push_back("cringe");
		names.push_back("cry");
		names.push_back("dance");
		names.push_back("gasp");
		names.push_back("giggle");
		names.push_back("salute");
		names.push_back("shrug");
		names.push_back("sigh");
		names.push_back("talk");
		names.push_back("wave");
		names.push_back("why");
		names.push_back("thanks");
		names.push_back("yes");
		// Friends
		names.push_back("addfriend");
		names.push_back("removefriend");
		names.push_back("addignore");
		names.push_back("removeignore");
		// Performance Options
		names.push_back("recommendedperfoptions");
		names.push_back("perfoptionslow");
		names.push_back("perfoptionmid");
		names.push_back("perfoptionhigh");
		// Teams
		names.push_back("invite");
		names.push_back("tinvite");
		names.push_back("teaminvite");
		names.push_back("inviteteam");
		names.push_back("leaveteam");
		names.push_back("leave");
		names.push_back("tleave");
		names.push_back("teamleave");
		names.push_back("setloot");
		names.push_back("tloot");
		names.push_back("tsetloot");
		names.push_back("teamsetloot");
		names.push_back("kickplayer");
		names.push_back("tkick");
		names.push_back("kick");
		names.push_back("tkickplayer");
		names.push_back("teamkickplayer");
		names.push_back("leader");
		names.push_back("setleader");
		names.push_back("tleader");
		names.push_back("tsetleader");
		names.push_back("teamsetleader");
		// Other
		names.push_back("cancelqueue");

		return names;
	}
};

class StopCommand : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("stop");
		names.push_back("off");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class TestCommand : public Command 
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("test");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class WispCommand : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("wisp");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class SetAdmin : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("setadmin");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class Emote : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("e");
		names.push_back("emote");
		names.push_back("play");
		names.push_back("animation");
		names.push_back("playanimation");
		names.push_back("anim");
		names.push_back("playanim");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class List : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("online");
		names.push_back("players");
		names.push_back("list");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class InvSize : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("invsize");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class Equipment : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("equipment");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class unequipall : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("unequipall");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class setattr : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("setattr");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class addskill : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("addskillfromlot");
		names.push_back("addskill");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class removeskill : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("removeskillfromlot");
		names.push_back("removeskill");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class sendchat : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("sendchat");
		names.push_back("chat");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class setcoins : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("setcoins");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class fly : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("fly");
		names.push_back("flight");
		names.push_back("jetpack");
		names.push_back("hover");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class gravity : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("gravity");
		names.push_back("grav");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class gmadditem : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("gmadditem");
		names.push_back("give");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class addmission : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("addmission");
		names.push_back("gmaddmission");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class completemission : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("completemission");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class ping : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("ping");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class createaccount : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("createaccount");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class deleteaccount : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("deleteaccount");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class banaccount : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("banaccount");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class banip : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("banip");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class spawn : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("spawn");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class despawn : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("despawn");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class nearme : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("nearme");
		names.push_back("aroundme");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class teleport : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("tp");
		names.push_back("tele");
		names.push_back("teleport");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};

class testmap : public Command
{
public:
	vector<string> getCommandNames() {
		vector<string> names = vector<string>();
		names.push_back("testmap");
		names.push_back("map");
		return names;
	}
	void execute(CommandSender sender, string cmd, vector<string> args);
};