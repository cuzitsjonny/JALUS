#include "AuthenticationInstance.h"
#include "PacketUtils.h"
#include "SHA512.h"
#include "Accounts.h"
#include "Bans.h"
#include "Logger.h"
#include "Server.h"
#include "TransitionInfos.h"
#include "Config.h"
#include "PingTool.h"

string to_string(LoginReturnCode lrc)
{
	switch (lrc)
	{

	case LRC_SUCCESS:
		return "SUCCESS";

	case LRC_BANNED:
		return "BANNED";

	case LRC_CUSTOM:
		return "CUSTOM";

	case LRC_INVALID_USERNAME_PASSWORD:
		return "INVALID_USERNAME_PASSWORD";

	case LRC_LOCKED:
		return "LOCKED";

	case LRC_ACTIVATION_PENDING:
		return "ACTIVATION_PENDING";

	case LRC_GAME_TIME_EXPIRED:
		return "GAME_TIME_EXPIRED";

	case LRC_FREE_TRIAL_ENDED:
		return "FREE_TRIAL_ENDED";

	case LRC_PLAY_SCHEDULE_DOES_NOT_ALLOW:
		return "PLAY_SCHEDULE_DOES_NOT_ALLOW";

	case LRC_NOT_ACTIVATED:
		return "NOT_ACTIVATED";

	default:
		return "INVALID_LOGIN_RETURN_CODE";

	}
}

void AuthStampContainer::addAuthStamp(unsigned long stampID, long nib, unsigned long unknown)
{
	AuthStamp as;
	as.stampID = stampID;
	as.nib = nib;
	as.unknown = unknown;

	stamps.push_back(as);
}

void AuthStampContainer::writeToBitStream(BitStream* bitStream)
{
	bitStream->Write(stamps.size() * 16 + 4);

	for (int i = 0; i < stamps.size(); i++)
	{
		AuthStamp as = stamps.at(i);

		bitStream->Write(as.stampID);
		bitStream->Write(as.nib);
		bitStream->Write(GetTime());
		bitStream->Write(as.unknown);
	}
}

void AuthenticationInstance::performAuthentication(BitStream* data, SystemAddress clientAddress)
{
	if ((data->GetNumberOfUnreadBits() / 8) >= 148)
	{
		LoginReturnCode lrc = LoginReturnCode::LRC_SUCCESS;
		string customMessage = "";
		long long accountID = -1;

		string address = clientAddress.ToString(false);
		string username = to_string(PacketUtils::readWStringFromBitStream(data, 33));
		string passwordHash = sha512(to_string(PacketUtils::readWStringFromBitStream(data, 41)));

		string nextInstanceAddress = Config::getCharactersInstanceAddress();
		unsigned short nextInstancePort = Config::getCharactersInstancePort();

		size_t pos = username.find("\\");

		if (pos == string::npos || Config::getIgnoreDomains())
		{
			if (pos != string::npos)
				username = username.substr((pos + 1), username.length());

			if (Config::getIgnoreDomains())
			{
				if (Bans::isBanned(address, true))
				{
					lrc = LoginReturnCode::LRC_CUSTOM;

					BanInfo i = Bans::getInfo(address, true);
					auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
					long long diff = ms.count() - (i.bannedTimestamp + i.howLong);

					string timeString = Bans::toTimeString(diff * (-1));

					string bannedUsername = Accounts::getUsername(i.banned);
					if (bannedUsername == "")
						bannedUsername = "System";

					customMessage = "The Ban Hammer has spoken!\n\n";
					customMessage += "Banned by: " + bannedUsername + "\n";
					customMessage += "Reason: " + i.reason + "\n";
					customMessage += "Duration: " + timeString;
				}
				else
				{
					accountID = Accounts::getAccountID(username);
					if (accountID != -1)
					{
						if (Accounts::getPasswordHash(accountID) == passwordHash)
						{
							Accounts::setLoginTries(0, accountID);

							if (Accounts::isLocked(accountID))
								lrc = LoginReturnCode::LRC_LOCKED;

							if (Bans::isBanned(to_string(accountID), false))
							{
								lrc = LoginReturnCode::LRC_CUSTOM;

								BanInfo i = Bans::getInfo(to_string(accountID), false);
								auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
								long long diff = ms.count() - (i.bannedTimestamp + i.howLong);

								string timeString = Bans::toTimeString(diff * (-1));

								string bannedUsername = Accounts::getUsername(i.banned);
								if (bannedUsername == "")
									bannedUsername = "System";

								customMessage = "The Ban Hammer has spoken!\n\n";
								customMessage += "Banned by: " + bannedUsername + "\n";
								customMessage += "Reason: " + i.reason + "\n";
								customMessage += "Duration: " + timeString;
							}
						}
						else
						{
							lrc = LoginReturnCode::LRC_INVALID_USERNAME_PASSWORD;

							int loginTries = Accounts::getLoginTries(accountID);

							if (!Accounts::isLocked(accountID))
							{
								loginTries++;

								if (loginTries > 2)
								{
									loginTries = 0;
									Accounts::updateLockedTimestamp(accountID);
									Logger::info("The account '" + username + "' got locked for 3 hours!");
								}

								Accounts::setLoginTries(loginTries, accountID);
							}
						}
					}
					else
						lrc = LoginReturnCode::LRC_INVALID_USERNAME_PASSWORD;
				}
			}
		}
		else
		{
			string domain = toUpperCase(username.substr(0, pos));
			username = username.substr((pos + 1), username.length());

			string a = Config::getDomainInstanceAddress(domain);
			if (a.length() > 0)
			{
				nextInstanceAddress = a;
				nextInstancePort = Config::getDomainInstancePort(domain);
			}
		}

		bool isNextInstanceAvailable = PingTool::ping(nextInstanceAddress.c_str(), nextInstancePort, "3.25 ND1", 8);
		if (!isNextInstanceAvailable)
		{
			lrc = LoginReturnCode::LRC_CUSTOM;
			customMessage = "The instance you want to connect to is not available at the moment!\nPlease try again later.";
		}

		BitStream* response = PacketUtils::createPacketBase(RCT_WORLD_TO_CLIENT, WORLD_CLIENT_LOGIN_RESPONSE);
		response->Write(lrc);

		PacketUtils::writeStringToBitStream("Talk_Like_A_Pirate", response, 33);

		for (int i = 0; i < 7; i++)
		{
			PacketUtils::writeStringToBitStream("", response, 33);
		}

		response->Write((unsigned short)1);
		response->Write((unsigned short)10);
		response->Write((unsigned short)64);

		if (accountID != -1 && lrc == LoginReturnCode::LRC_SUCCESS)
		{
			string transitionKey = TransitionInfos::generateTransitionInfo(clientAddress, accountID);
			PacketUtils::writeWStringToBitStream(to_wstring(transitionKey), response, 33);
		}
		else
			PacketUtils::writeWStringToBitStream(L"INVALID_LOGIN", response, 33);

		PacketUtils::writeStringToBitStream(nextInstanceAddress, response, 33);
		PacketUtils::writeStringToBitStream(nextInstanceAddress, response, 33);

		response->Write(nextInstancePort);
		response->Write(nextInstancePort);

		PacketUtils::writeStringToBitStream(nextInstanceAddress, response, 33);

		PacketUtils::writeStringToBitStream("00000000-0000-0000-0000-000000000000", response, 37);

		response->Write((unsigned long)0);

		PacketUtils::writeStringToBitStream("US", response, 3);

		response->Write((unsigned char)0);
		response->Write((unsigned char)0);
		response->Write((unsigned long long)0);

		response->Write((unsigned short)customMessage.length());
		PacketUtils::writeWStringToBitStream(to_wstring(customMessage), response, customMessage.length());

		AuthStampContainer asc;
		asc.addAuthStamp(0, 0, 0);
		asc.addAuthStamp(7, 36, 0);
		asc.addAuthStamp(8, 4, 0);
		asc.addAuthStamp(9, 0, 0);
		asc.addAuthStamp(10, 0, 0);
		asc.addAuthStamp(11, 1, 0);
		asc.addAuthStamp(14, 1, 0);
		asc.addAuthStamp(15, 0, 0);
		asc.addAuthStamp(17, 1, 0);
		asc.addAuthStamp(5, 0, 0);
		asc.addAuthStamp(6, 1, 0);
		asc.addAuthStamp(20, 1, 0);
		asc.addAuthStamp(19, 10044, 0);
		asc.addAuthStamp(21, 0, 0);
		asc.addAuthStamp(22, 0, 0);
		asc.addAuthStamp(23, 10038, 0);
		asc.addAuthStamp(27, 10038, 0);
		asc.addAuthStamp(28, 1, 0);
		asc.addAuthStamp(29, 0, 0);
		asc.addAuthStamp(30, 10044, 0);
		asc.writeToBitStream(response);

		Logger::info("Client attempted to log in! (Result: " + to_string(lrc) + ") (Address: " + string(clientAddress.ToString()) + ")");

		Server::sendPacket(response, clientAddress);
	}
}
