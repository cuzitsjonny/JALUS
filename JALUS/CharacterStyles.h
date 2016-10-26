#pragma once
#include "Common.h"

class CharacterStyle
{
public:
	long shirtColor = -1;
	long shirtStyle = -1;
	long pantsColor = -1;
	long hairStyle = -1;
	long hairColor = -1;
	long lh = -1;
	long rh = -1;
	long eyebrows = -1;
	long eyes = -1;
	long mouth = -1;
};

class CharacterStyles
{
public:
	static string name;

	static void init(string name, string structure);

	static long getLOTFromStyle(long shirtColor, long shirtStyle);
	static long getLOTFromStyle(long pantsColor);

	static void saveCharacterStyle(CharacterStyle style, long long charID);
	static void deleteCharacterStyle(long long charID);
	static CharacterStyle getCharacterStyle(long long charID);
};