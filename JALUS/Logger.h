#pragma once
#include "Common.h"

class Logger
{
private:
	static string filePath;
	static bool consoleOutputMuted;

	static string getTime();
public:
	static void init();

	static void info(string msg);
	static void error(string msg);

	static void setConsoleOutputMuted(bool muted);
	static bool isConsoleOutputMuted();
};