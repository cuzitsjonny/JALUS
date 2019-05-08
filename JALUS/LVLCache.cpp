#include "LVLCache.h"
#include "Server.h"
#include "Locations.h"
#include "ObjectsManager.h"
#include "Logger.h"
#include "LUZCache.h"

vector<ObjectPropertyContainer> LVLCache::containers = vector<ObjectPropertyContainer>();

void LVLCache::loadObjects()
{
	LUZFile* luzFile = LUZCache::getByZoneID(ServerRoles::toZoneID(Server::getServerRole()));

	for (int i = 0; i < luzFile->childFiles.size(); i++)
	{
		vector<string> p = split(luzFile->childFiles.at(i), '.');

		if (p.size() == 2)
		{
			if (iequals(p[1], "lvl"))
			{
				string path = ".\\maps\\" + to_string(luzFile->zoneID) + "\\" + luzFile->childFiles.at(i);

				BitStream reader;

				vector<unsigned char> v = readAllBytes(path.c_str());
				for (int i = 0; i < v.size(); i++)
				{
					reader.Write(v.at(i));
				}

				v.clear();

				unsigned long version = 0;

				while ((reader.GetNumberOfUnreadBits() / 8) > 0)
				{
					string cur = "";

					for (int i = 0; i < 4; i++)
					{
						char c;
						reader.Read(c);
						cur += c;
					}

					if (cur == "CHNK")
					{
						unsigned long type;
						reader.Read(type);

						reader.IgnoreBytes(12);

						while (((reader.GetReadOffset() / 8) % 16) != 0)
						{
							reader.IgnoreBytes(1);
						}

						if (type == 1000)
						{
							reader.Read(version);
							reader.IgnoreBytes(12);
						}

						if (type == 2001)
						{
							unsigned long countOfObjects;
							reader.Read(countOfObjects);

							for (int i = 0; i < countOfObjects; i++)
							{
								long long objectID;
								reader.Read(objectID);

								objectID = (objectID | 70368744177664);

								long lot;
								reader.Read(lot);

								if (version >= 0x26)
								{
									reader.IgnoreBytes(4);
								}

								if (version >= 0x20)
								{
									reader.IgnoreBytes(4);
								}

								Position pos = Position();
								reader.Read(pos.x);
								reader.Read(pos.y);
								reader.Read(pos.z);

								Rotation rot = Rotation();
								reader.Read(rot.w);
								reader.Read(rot.x);
								reader.Read(rot.y);
								reader.Read(rot.z);

								float scale;
								reader.Read(scale);

								wstring ldf = L"";
								unsigned long len;
								reader.Read(len);

								for (int k = 0; k < len; k++)
								{
									wchar_t c;
									reader.Read(c);
									ldf += c;
								}

								if (version >= 0x07)
								{
									reader.IgnoreBytes(4);
								}

								//Logger::info("Loading object with ID " + to_string(objectID) + "!");

								ObjectPropertyContainer container = ObjectPropertyContainer();
								istringstream stream(to_string(ldf));

								string line;
								while (getline(stream, line))
								{
									vector<string> p1 = split(line, '=');
									vector<string> p2 = split(p1[1], ':');

									ObjectProperty pro = ObjectProperty();
									pro.key = p1[0];
									pro.type = stoi(p2[0]);

									if (p2.size() == 2)
									{
										pro.value = p2[1];
									}

									container.properties.push_back(pro);
								}

								container.objectID = objectID;
								containers.push_back(container);
								// THIS SPAWNS THE OBJECTS INTO THE WORLD
								
								if (lot == 176 || lot == 6842 || lot == 8139)
								{ // I think this if statement makes it so certain objects don't crash the server
									Logger::info("Loading object with ID " + to_string(objectID) + "!");
									ReplicaObject* replica = new ReplicaObject(objectID, lot, L"", 0, pos, rot);
									replica->scale = scale;
									Server::getReplicaManager()->ReferencePointer(replica);
								}
							}

							while (((reader.GetReadOffset() / 8) % 16) != 0)
							{
								reader.IgnoreBytes(1);
							}
						}
					}
				}
			}
		}
	}
}

vector<ObjectProperty> LVLCache::getObjectProperties(long long objectID)
{
	for (int i = 0; i < containers.size(); i++)
	{
		ObjectPropertyContainer container = containers.at(i);
		
		if (container.objectID == objectID)
			return container.properties;
	}

	return vector<ObjectProperty>();
}

ObjectProperty LVLCache::getObjectProperty(string key, long long objectID)
{
	vector<ObjectProperty> v = getObjectProperties(objectID);

	for (int i = 0; i < v.size(); i++)
	{
		if (v.at(i).key == key)
			return v.at(i);
	}

	ObjectProperty pro;
	pro.key = key;
	pro.value = "";

	return pro;
}
