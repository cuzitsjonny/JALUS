#pragma once
#include "Common.h"

enum LoginReturnCode : unsigned char
{
	LRC_SUCCESS = 0x01,
	LRC_BANNED = 0x02,
	LRC_CUSTOM = 0x05,
	LRC_INVALID_USERNAME_PASSWORD = 0x06,
	LRC_LOCKED = 0x07,
	LRC_ACTIVATION_PENDING = 0x09,
	LRC_GAME_TIME_EXPIRED = 0x0b,
	LRC_FREE_TRIAL_ENDED = 0x0c,
	LRC_PLAY_SCHEDULE_DOES_NOT_ALLOW = 0x0d,
	LRC_NOT_ACTIVATED = 0x0e
};

string to_string(LoginReturnCode lrc);

class AuthStamp
{
public:
	unsigned long stampID;
	long nib;
	unsigned long unknown;
};

class AuthStampContainer
{
private:
	vector<AuthStamp> stamps;
public:
	void addAuthStamp(unsigned long stampID, long nib, unsigned long unknown);
	void writeToBitStream(BitStream* bitStream);
};

class AuthenticationInstance
{
public:
	static void performAuthentication(BitStream* data, SystemAddress clientAddress);
};