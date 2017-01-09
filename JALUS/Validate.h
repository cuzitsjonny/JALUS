#pragma once
#include "Common.h"

class Validate
{
public:
	static bool isValidS32(string str);
	static bool isValidS64(string str);
	static bool isValidFloat(string str);
	static bool isValidDouble(string str);
};