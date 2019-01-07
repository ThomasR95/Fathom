#pragma once
#include "typedefs.h"
#include <memory>
#include "Terrain.h"
#include <thread>

class World
{
public:
	World();
	~World();

	struct Gateway
	{
		sf::FloatRect m_area;
		std::string m_destinationName;
		sf::Vector2f m_destinationPosition;
		bool m_active;
		std::string m_requires;
		std::string m_rejectDialog;
	};

	struct Level
	{
		Terrain m_terrain;
		std::vector<Gateway> m_links;
		std::string m_xmlPath;
		std::string m_bgm;
		std::vector<std::string> preloadLevels;
		std::vector<std::thread> loadingThreads;
	};

	void setCurrentLevel(_IN_STR level);
	std::string getCurrentLevel() { return m_activeLevel; }

	Level& getCurrentLevelObject() { return m_levels[m_activeLevel]; }

	void unloadLevel(_IN_STR level);

	void readXml(_IN_STR xmlPath);

	sf::Vector2f checkGateways(_IN_VEC2 charPos);

	Gateway* getGatewayToLevel(_IN_STR level);

	GfxBase* getAsset(_IN_STR id);

	void loadLevel(_IN_STR id);

	//std::thread loadLevelThread(_IN_STR id);

private:
	std::map<std::string, Level> m_levels;
	std::string m_activeLevel;
};

