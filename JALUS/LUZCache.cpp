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

		/*if (version >= 0x20)
		{
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
		}*/

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

		/*if (version >= 0x23)
		{
			unsigned long lengthOfRestOfFile;
			reader.Read(lengthOfRestOfFile);
			reader.IgnoreBytes(4);

			unsigned long count;
			reader.Read(count);

			for (int i = 0; i < count; i++)
			{
				unsigned long pathVersion;
				reader.Read(pathVersion);
				unsigned char count;
				reader.Read(count);
				for (int i = 0; i < count; i++)
				{
					wstring pathName = L"";
					reader.Read(pathName);
				}//0	Movement
				// 1	Moving Platform
				// 2	Property
				// 3	Camera
				// 4	Spawner
				// 5	Showcase
				// 6	Race
				// 7	Rail
				unsigned long pathType;
				reader.Read(pathType);
				reader.IgnoreBytes(4);
				unsigned long pathBehavior; // 0 = Loop, 1 = Bounce, 2 = Once
				reader.Read(pathBehavior);

				if (pathType == 1)
				{
					if (pathVersion >= 18)
					{
						reader.IgnoreBytes(4);
					}
					else if (pathVersion >= 13)
					{
						unsigned char count;
						reader.Read(count);
						for (int i = 0; i < count; i++)
						{
							wstring movingPlatFormTravelSound = L"";
							reader.Read(movingPlatFormTravelSound);
						}
					}
				}
				/*else if (pathType == 2)
				{
					reader.IgnoreBytes(4);
					signed long price;
					reader.Read(price);
					signed long rentalTime;
					reader.Read(rentalTime);
					unsigned long long associatedZone;
					reader.Read(associatedZone);
					unsigned char count_4;
					reader.Read(count_4);
					for (int i = 0; i < count_4; i++)
					{
						wstring movingPlatFormTravelSound = L"";
						reader.Read(movingPlatFormTravelSound);
					}
				}*/
				/*else if (pathType == 4) // this is very unfinished, so please ignore
				{
					{
						unsigned long spawnedLOT;
						reader.Read(spawnedLOT);
						unsigned long respawnTime;
						reader.Read(respawnTime);
						signed long maxToSpawn;
						reader.Read(maxToSpawn);
						unsigned long numToMaintain;
						reader.Read(numToMaintain);
						bool activateSpawnerNetworkOnLoad;
						reader.Read(activateSpawnerNetworkOnLoad);
					}



				}*/


			}


		}

	}
}

void LUZCache::init()
{
	vector<string> d = getAllDirectoriesInDirectory(".\\maps");

	for (int i = 0; i < d.size(); i++)
	{
		vector<string> f = getAllFilesInDirectory(".\\maps\\" + d.at(i), "*.luz");

		for (int k = 0; k < f.size(); k++)
		{
			LUZFile file = LUZFile(".\\maps\\" + d.at(i) + "\\" + f.at(k));
			files.push_back(file);
		}
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
