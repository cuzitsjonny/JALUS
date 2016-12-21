#include "LUZCache.h"
#include "Server.h"

vector<LUZFile> LUZCache::files;

LUZFile::LUZFile(string path)
{
	if (fexists(path.c_str()))
	{
		BitStream reader;

		vector<unsigned char> v = readAllBytes(path.c_str());
		for (int i = 0; i < v.size(); i++)
		{
			reader.Write(v.at(i));
		}

		v.clear();

		unsigned long version;
		reader.Read(version);

		if (version >= 0x24)
		{
			reader.IgnoreBytes(4);
		}

		reader.Read(zoneID);

		if (version >= 0x26)
		{
			reader.Read(spawnPointPos.x);
			reader.Read(spawnPointPos.y);
			reader.Read(spawnPointPos.z);
			reader.Read(spawnPointRot.x);
			reader.Read(spawnPointRot.y);
			reader.Read(spawnPointRot.z);
			reader.Read(spawnPointRot.w);
		}

		unsigned long countOfScenes;

		if (version < 0x25)
		{
			unsigned char c;
			reader.Read(c);

			countOfScenes = c;
		}
		else
		{
			reader.Read(countOfScenes);
		}

		for (int i = 0; i < countOfScenes; i++)
		{
			unsigned char len;
			string fileName = "";
			string sceneName = "";

			reader.Read(len);
			for (int k = 0; k < len; k++)
			{
				char c;
				reader.Read(c);
				fileName += c;
			}

			reader.IgnoreBytes(8);

			reader.Read(len);
			for (int k = 0; k < len; k++)
			{
				char c;
				reader.Read(c);
				sceneName += c;
			}

			reader.IgnoreBytes(3);

			childFiles.push_back(fileName);
			childScenes.push_back(sceneName);
		}
	}
}

void LUZCache::init(vector<string> names)
{
	for (int i = 0; i < names.size(); i++)
	{
		string name = names.at(i);
		LUZFile file = LUZFile(".\\maps\\" + ServerRoles::toString(Server::getServerRole()) + "\\" + name);
		files.push_back(file);
	}
}

LUZFile* LUZCache::getByZoneID(ZoneID zoneID)
{
	for (int i = 0; i < files.size(); i++)
	{
		if (files.at(i).zoneID == zoneID)
			return &files.at(i);
	}

	return nullptr;
}

vector<LUZFile> LUZCache::getAll()
{
	return files;
}
