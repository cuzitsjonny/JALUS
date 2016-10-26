#include "Sessions.h"

vector<Session> Sessions::sessions;

void Sessions::addSession(SystemAddress clientAddress, long long accountID, long long charID, string transitionKey)
{
	Session s;
	s.clientAddress = clientAddress;
	s.accountID = accountID;
	s.charID = charID;
	s.transitionKey = transitionKey;

	sessions.push_back(s);
}

void Sessions::removeSession(SystemAddress clientAddress)
{
	bool found = false;
	for (int i = 0; i < sessions.size() && !found; i++)
	{
		if (sessions.at(i).clientAddress == clientAddress)
		{
			sessions.erase(sessions.begin() + i);
			found = true;
		}
	}
}

Session* Sessions::getSession(SystemAddress clientAddress)
{
	for (int i = 0; i < sessions.size(); i++)
	{
		if (sessions.at(i).clientAddress == clientAddress)
			return &sessions.at(i);
	}

	return nullptr;
}
