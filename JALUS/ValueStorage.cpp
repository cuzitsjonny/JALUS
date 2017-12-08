#include "ValueStorage.h"

vector<ValueStoring> ValueStorage::valueStorage;

void ValueStorage::createValueInMemory(long long charid, string type, long value)
{
	ValueStoring values;
	values.charid = charid;
	values.type = type;
	values.value = value;

	valueStorage.push_back(values);
}

void ValueStorage::updateValueInMemory(long long charid, string type, long value)
{
	bool found = false;
	for (int i = 0; i < valueStorage.size() && !found; i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				valueStorage.at(i).value = value;
				found = true;
			}
		}
	}
}

void ValueStorage::removeValueFromMemory(long long charid, string type)
{
	bool found = false;
	for (int i = 0; i < valueStorage.size() && !found; i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				valueStorage.erase(valueStorage.begin() + i);
				found = true;
			}
		}
	}
}

long ValueStorage::getValueInMemory(long long charid, string type)
{
	for (int i = 0; i < valueStorage.size(); i++)
	{
		if (valueStorage.at(i).charid == charid)
		{
			if (valueStorage.at(i).type == type)
			{
				return valueStorage.at(i).value;
			}
		}
	}
}