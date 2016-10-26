#include "Logger.h"
#include "Server.h"

string Logger::filePath;
bool Logger::consoleOutputMuted;

string Logger::getTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[10];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

void Logger::init()
{
	Logger::filePath = ".\\logs\\" + ServerRoles::toString(Server::getServerRole()) + ".log";
	Logger::consoleOutputMuted = false;

	if (fexists(Logger::filePath.c_str()))
		remove(Logger::filePath.c_str());
	else
		CreateDirectory(".\\logs", NULL);
}

void Logger::info(string msg)
{
	string output = "[" + Logger::getTime() + "] [INFO] " + msg;

	if (!Logger::consoleOutputMuted)
		cout << output << endl;

	ofstream f(Logger::filePath, fstream::binary | fstream::app);
	f << output << "\n";
	f.close();
}

void Logger::error(string msg)
{
	string output = "[" + Logger::getTime() + "] [ERROR] " + msg;

	if (!Logger::consoleOutputMuted)
		cout << output << endl;

	ofstream f(Logger::filePath, fstream::binary | fstream::app);
	f << output << "\n";
	f.close();
}

void Logger::setConsoleOutputMuted(bool muted)
{
	Logger::consoleOutputMuted = muted;
}

bool Logger::isConsoleOutputMuted()
{
	return Logger::consoleOutputMuted;
}
