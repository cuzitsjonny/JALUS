#pragma once
#include "Common.h"
#include "BlockingQueue.h"

#define LOGGER_STOP_STRING "LOGGER_COMMAND_STOP"
#define LOGGER_MUTE_STRING "LOGGER_COMMAND_MUTE"
#define LOGGER_UNMUTE_STRING "LOGGER_COMMAND_UNMUTE"

class Logger
{
private:
	static bool consoleOutputMuted;
	static string filePath;
	static BlockingQueue<string> queue;
	static thread flusher;

	static string getTime();
public:
	static void init();
	static void shutdown();

	static void info(string msg);
	static void error(string msg);

	static void setConsoleOutputMuted(bool muted);
	static bool isConsoleOutputMuted();
};