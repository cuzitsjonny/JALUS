#pragma once

#include <iostream>
#include <tchar.h>
#include <vector>
#include <fstream>
#include <string>
#include <Ws2tcpip.h>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <chrono>
#include <regex>
#include <random>
#include <thread>

#include <RakPeerInterface.h>
#include <RakNetworkFactory.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <RakSleep.h>
#include <GetTime.h>
#include <Replica.h>
#include <ReplicaManager.h>
#include <NetworkIDManager.h>

#include <SQLAPI.h>

using namespace RakNet;
using namespace std;
using namespace chrono;

bool fexists(const char* filePath);
bool iequals(char a, char b);
bool iequals(string a, string b);
vector<string> split(string s, char delim);
string to_string(bool b);
string to_string(wstring wstr);
wstring to_wstring(string str);
vector<unsigned char> readAllBytes(const char* filePath);
bool isValidIPAddress(const char* str);
long long randomInRange(long long min, long long max);
void saveToFile(unsigned char* data, unsigned int size, const char* filePath);
void saveToFile(BitStream* bitStream, const char* filePath);
string toUpperCase(string str);
string toLowerCase(string str);
bool startsWith(string str, string start);
vector<string> getAllFilesInDirectory(string path, string filter = "*.*");
vector<string> getAllDirectoriesInDirectory(string path, string filter = "*");