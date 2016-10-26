#include "TimeUtils.h"

long long TimeUtils::millisecondsToSeconds(long long time)
{
	return (long long)((double)time / 1000.0);
}

long long TimeUtils::millisecondsToMinutes(long long time)
{
	return (long long)((double)time / 1000.0 / 60.0);
}

long long TimeUtils::millisecondsToHours(long long time)
{
	return (long long)((double)time / 1000.0 / 60.0 / 60.0);
}

long long TimeUtils::millisecondsToDays(long long time)
{
	return (long long)((double)time / 1000.0 / 60.0 / 60.0 / 24.0);
}

long long TimeUtils::secondsToMilliseconds(long long time)
{
	return (long long)((double)time * 1000.0);
}

long long TimeUtils::secondsToMinutes(long long time)
{
	return (long long)((double)time / 60.0);
}

long long TimeUtils::secondsToHours(long long time)
{
	return (long long)((double)time / 60.0 / 60.0);
}

long long TimeUtils::secondsToDays(long long time)
{
	return (long long)((double)time / 60.0 / 60.0 / 24.0);
}

long long TimeUtils::minutesToMilliseconds(long long time)
{
	return (long long)((double)time * 1000.0 * 60.0);
}

long long TimeUtils::minutesToSeconds(long long time)
{
	return (long long)((double)time * 60.0);
}

long long TimeUtils::minutesToHours(long long time)
{
	return (long long)((double)time / 60.0);
}

long long TimeUtils::minutesToDays(long long time)
{
	return (long long)((double)time / 60.0 / 24.0);
}

long long TimeUtils::hoursToMilliseconds(long long time)
{
	return (long long)((double)time * 1000.0 * 60.0 * 60.0);
}

long long TimeUtils::hoursToSeconds(long long time)
{
	return (long long)((double)time * 60.0 * 60.0);
}

long long TimeUtils::hoursToMinutes(long long time)
{
	return (long long)((double)time * 60.0);
}

long long TimeUtils::hoursToDays(long long time)
{
	return (long long)((double)time / 24.0);
}

long long TimeUtils::daysToMilliseconds(long long time)
{
	return (long long)((double)time * 1000.0 * 60.0 * 60.0 * 24.0);
}

long long TimeUtils::daysToSeconds(long long time)
{
	return (long long)((double)time * 60.0 * 60.0 * 24.0);
}

long long TimeUtils::daysToMinutes(long long time)
{
	return (long long)((double)time * 60.0 * 24.0);
}

long long TimeUtils::daysToHours(long long time)
{
	return (long long)((double)time * 24.0);
}

long long TimeUtils::parse(string time)
{
	long long l;

	try {
		if (time == "-1")
			return -1;
		else if (regex_match(time, regex("[0-9]+[m]{1}[s]{1}")))
		{
			l = stoll(time.substr(0, time.length() - 2));
			return l;
		}
		else if (regex_match(time, regex("[0-9]+[s]{1}[e]{1}[c]{1}")))
		{
			l = stoll(time.substr(0, time.length() - 3));
			return secondsToMilliseconds(l);
		}
		else if (regex_match(time, regex("[0-9]+[m]{1}[i]{1}[n]{1}")))
		{
			l = stoll(time.substr(0, time.length() - 3));
			return minutesToMilliseconds(l);
		}
		else if (regex_match(time, regex("[0-9]+[h]{1}")))
		{
			l = stoll(time.substr(0, time.length() - 1));
			return hoursToMilliseconds(l);
		}
		else if (regex_match(time, regex("[0-9]+[d]{1}")))
		{
			l = stoll(time.substr(0, time.length() - 1));
			return daysToMilliseconds(l);
		}
		else
			return -2;
	}
	catch (exception &e)
	{
		return -2;
	}
}
