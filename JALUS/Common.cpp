#include "Common.h"

bool fexists(const char* filePath)
{
	bool r;

	ifstream f(filePath);
	r = f.good();
	f.close();

	return r;
}

bool iequals(char a, char b)
{
	return (toupper(a) == toupper(b));
}

bool iequals(string a, string b)
{
	if (a.length() == b.length())
	{
		for (int i = 0; i < a.length(); i++)
		{
			if (!iequals(a[i], b[i]))
				return false;
		}
	}
	else
		return false;

	return true;
}

vector<string> split(string s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;

	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}

	return tokens;
}

string to_string(bool b)
{
	return (b ? "true" : "false");
}

string to_string(wstring wstr)
{
	return string(wstr.begin(), wstr.end());
}

wstring to_wstring(bool b)
{
	return (b ? L"true" : L"false");
}

wstring to_wstring(string str)
{
	return wstring(str.begin(), str.end());
}

vector<unsigned char> readAllBytes(const char* filePath)
{
	ifstream file(filePath, ios::binary | ios::ate);

	if (file.is_open()) {
		auto pos = file.tellg();

		vector<unsigned char> r((int)pos);

		file.seekg(0, ios::beg);
		file.read((char*)r.data(), pos);

		file.close();

		return r;
	}

	return vector<unsigned char>(0);
}

bool isValidIPAddress(const char* str)
{
		struct sockaddr_in sa;
		int result = inet_pton(AF_INET, str, &(sa.sin_addr));
		return result > 0;
}

long long randomInRange(long long min, long long max)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<long long> distr(min, max);

	return distr(eng);
}

void saveToFile(unsigned char* data, unsigned int size, const char* filePath)
{
	if (fexists(filePath))
		remove(filePath);

	ofstream file(filePath, ios::app);
	if (!file.is_open()) return;

	file.write((char*)data, size);
	file.close();
}

void saveToFile(BitStream* bitStream, const char* filePath)
{
	saveToFile(bitStream->GetData(), bitStream->GetNumberOfBytesUsed(), filePath);
}

string toUpperCase(string str)
{
	string r = "";

	for (int i = 0; i < str.length(); i++)
	{
		r += toupper(str[i]);
	}

	return r;
}

string toLowerCase(string str)
{
	string r = "";

	for (int i = 0; i < str.length(); i++)
	{
		r += tolower(str[i]);
	}

	return r;
}

bool startsWith(string str, string start)
{
	if (start.length() <= str.length())
	{
		if (start == str.substr(0, start.length()))
			return true;
	}

	return false;
}

vector<string> getAllFilesInDirectory(string path, string filter)
{
	vector<string> r = vector<string>();

	string searchPath = path + "\\" + filter;
	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile(searchPath.c_str(), &fd);

	if (h != INVALID_HANDLE_VALUE)
	{
		do {
			if (!((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)))
			{
				r.push_back(fd.cFileName);
			}
		} while (FindNextFile(h, &fd));

		FindClose(h);
	}

	return r;
}

vector<string> getAllDirectoriesInDirectory(string path, string filter)
{
	vector<string> r = vector<string>();

	string searchPath = path + "\\" + filter;
	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile(searchPath.c_str(), &fd);

	if (h != INVALID_HANDLE_VALUE)
	{
		do {
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				r.push_back(fd.cFileName);
			}
		} while (FindNextFile(h, &fd));

		FindClose(h);
	}

	return r;
}
