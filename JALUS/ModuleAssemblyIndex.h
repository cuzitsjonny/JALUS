#pragma once
#include "Common.h"

class ModuleAssemblyIndex
{
public:

	//bool flagTemp;
	//unsigned long un32Temp;

	bool flag_0;
	bool flag_0_1;
	long long data_0;
	bool flag_0_2;
	long wstringLength;
	wstring data_1;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
