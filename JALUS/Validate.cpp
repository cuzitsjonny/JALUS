#include "Validate.h"

bool Validate::isValidS32(string str)
{
	try
	{
		stol(str);
		return true;
	}
	catch (exception& e)
	{
		return false;
	}
}

bool Validate::isValidS64(string str)
{
	try
	{
		stoll(str);
		return true;
	}
	catch (exception& e)
	{
		return false;
	}
}

bool Validate::isValidFloat(string str)
{
	try
	{
		stof(str);
		return true;
	}
	catch (exception& e)
	{
		return false;
	}
}

bool Validate::isValidDouble(string str)
{
	try
	{
		stod(str);
		return true;
	}
	catch (exception& e)
	{
		return false;
	}
}
