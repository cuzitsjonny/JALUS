#include "Commands.h"
#include "ServerLoop.h"
#include "Accounts.h"
#include "SHA512.h"
#include "TimeUtils.h"
#include "Bans.h"
#include "CDClient.h"

void Commands::performCommand(CommandSender sender, string cmd, vector<string> args)
{
	if (iequals(cmd, "stop")) // /stop
	{
		ServerLoop::stop();
	}

	else if (iequals(cmd, "createAccount")) // /createAccount <string:username> <string:password>
	{
		if (args.size() == 2)
		{
			string username = args.at(0);
			string password = args.at(1);

			long long accountID = Accounts::getAccountID(username);
			if (accountID == -1)
			{
				Accounts::createAccount(username, sha512(password));
				sender.sendMessage("Successfully created account! (Username: " + username + ")");
			}
			else
				sender.sendMessage("The username '" + username + "' is already in use!");
		}
		else
			sender.sendMessage("Invalid parameters! Syntax: /" + cmd + " <username> <password>");
	}

	else if (iequals(cmd, "deleteAccount")) // /deleteAccount <string:username>
	{
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

	else if (iequals(cmd, "banAccount")) // /banAccount <string:username> <string:time> <string:reason>
	{
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
					Bans::ban(to_string(accountID), sender.getSenderID(), false, reason, howLong);
					sender.sendMessage(username + " got banned successfully! (Duration: " + Bans::toTimeString(howLong) + ") (Reason: " + reason + ")");
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

	else if (iequals(cmd, "test"))
	{
		cout << (int)CDClient::getItemType(12809) << endl;
	}

	else if (iequals(cmd, "banIP")) // /banAccount <string:ip> <string:time> <string:reason>
	{
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
					Bans::ban(ip, sender.getSenderID(), true, reason, howLong);
					sender.sendMessage(ip + " got banned successfully! (Duration: " + Bans::toTimeString(howLong) + ") (Reason: " + reason + ")");
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

	else
		sender.sendMessage("The input '" + cmd + "' is not a valid command!");
}

CommandSender::CommandSender(long long senderID)
{
	this->senderID = senderID;
}

void CommandSender::sendMessage(string msg)
{
	if (senderID == -1)
		cout << msg << endl;
	else
		;// TODO: Implement Sessions
}

long long CommandSender::getSenderID()
{
	return senderID;
}
