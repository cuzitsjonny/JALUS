#include "Logger.h"
#include "Server.h"

bool Logger::consoleOutputMuted;
string Logger::filePath;
BlockingQueue<string> Logger::queue;
thread Logger::flusher;

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

	Logger::flusher = thread([]()
	{
		bool run = true;
		bool muted = false;
		while (run)
		{
			string msg = Logger::queue.poll();

			if (msg == LOGGER_STOP_STRING)
			{
				run = false;
			}

			else if (msg == LOGGER_MUTE_STRING)
			{
				muted = true;
			}

			else if (msg == LOGGER_UNMUTE_STRING)
			{
				muted = false;
			}

			else
			{
				if (!muted)
					cout << msg << endl;

				ofstream f(Logger::filePath, fstream::binary | fstream::app);
				f << msg << endl;
				f.close();
			}
		}
	});
}

void Logger::shutdown()
{
	Logger::queue.push(LOGGER_STOP_STRING);
	Logger::flusher.join();
}

void Logger::info(string msg)
{
	Logger::queue.push("[" + Logger::getTime() + "] [INFO] " + msg);
}

void Logger::error(string msg)
{
	Logger::queue.push("[" + Logger::getTime() + "] [ERROR] " + msg);
}

void Logger::setConsoleOutputMuted(bool muted)
{
	Logger::consoleOutputMuted = muted;

	if (muted)
	{
		Logger::queue.push(LOGGER_MUTE_STRING);
	}
	else
	{
		Logger::queue.push(LOGGER_UNMUTE_STRING);
	}
}

bool Logger::isConsoleOutputMuted()
{
	return Logger::consoleOutputMuted;
}
