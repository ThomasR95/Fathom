#include "World.h"
#include "tinyxml2\tinyxml2.h"
#include "engine.h"
#include "resourceManager.h"
#include "Player.h"

World::World()
{
}


World::~World()
{
}

void World::setCurrentLevel(_IN_STR level)
{
	for (auto& th : m_levels[m_activeLevel].loadingThreads)
		th.join();

	m_levels[m_activeLevel].loadingThreads.clear();

	m_activeLevel = level;

	for (auto& l : m_levels)
	{
		if (l.first == level)
		{
			if (l.second.m_bgm != "")
			{
				auto music = resMan->getMusic(l.second.m_bgm);
				if (music)
				{
					music->setLoop(true);
					if (music->getStatus() != music->Playing)
						music->play();
				}
			}

			l.second.m_terrain.readXML(l.second.m_xmlPath, level);
			l.second.m_terrain.setActive(true);
			l.second.m_terrain.setVisible(true);
			l.second.m_terrain.addToRenderlist();
			l.second.m_terrain.addCollisionObjects();
			for (auto& t : l.second.m_terrain.m_dialogTriggers)
				theEngine->dialog->setTrigBtnState(t, tButton::bState::ON);

			theEngine->setNextState(l.second.m_terrain.getGameState());
			theEngine->exitState();

			if (l.second.m_terrain.m_startDialog.size())
				theEngine->dialog->startDialog(l.second.m_terrain.m_startDialog);

			//if (l.second.preloadLevels.size())
			//{
			//	for (auto& lv : l.second.preloadLevels)
			//	{
			//		l.second.loadingThreads.emplace_back(std::thread(&World::loadLevel, this, lv));
			//	}
			//	l.second.preloadLevels.clear();
			//}
			
		}
		else
		{
			l.second.m_terrain.setActive(false);
			l.second.m_terrain.setVisible(false);
			l.second.m_terrain.removeCollisionObjects();
			for (auto& t : l.second.m_terrain.m_dialogTriggers)
				theEngine->dialog->setTrigBtnState(t, tButton::bState::INVISIBLE);

			if (l.second.m_bgm != "")
			{
				auto music = resMan->getMusic(l.second.m_bgm);
				if (music)
				{
					if (l.second.m_bgm != m_levels[level].m_bgm && music->getStatus() == music->Playing)
						music->stop();
				}
			}
		}
	}
}

void World::unloadLevel(_IN_STR level)
{
	for (auto& l : m_levels)
	{
		if (l.first == level)
		{
			auto btex = l.second.m_terrain.m_background.sprite.getTexture();
			theEngine->removeFromRenderList(&l.second.m_terrain.m_background);
			resMan->unloadTexture(btex);

			for (auto& obst : l.second.m_terrain.m_obstacles)
			{
				auto otex = obst.sprite.getTexture();
				theEngine->removeFromRenderList(&obst);
				resMan->unloadTexture(otex);
			}

			auto ftex = l.second.m_terrain.m_foreground.sprite.getTexture();
			theEngine->removeFromRenderList(&l.second.m_terrain.m_foreground);
			resMan->unloadTexture(ftex);

			l.second.m_terrain.m_background = Obstacle();
			l.second.m_terrain.m_foreground = Obstacle();
			l.second.m_terrain.m_obstacles.clear();
			l.second.m_terrain.m_XMLLoaded = false;
		}
	}
}

void World::readXml(_IN_STR xmlPath)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlPath.c_str());
	if (doc.Error())
		return;

	int pos = 0;
	pos = xmlPath.rfind("/");
	auto dir = xmlPath.substr(0, pos + 1);

	auto root = doc.FirstChildElement("Levels");
	if (root)
	{
		//count levels
		auto levelNode = root->FirstChildElement("Level");
		int lCount = 0;
		while (levelNode)
		{
			lCount++;
			levelNode = levelNode->NextSiblingElement("Level");
		}
		
		int loadLevel = 0;
		levelNode = root->FirstChildElement("Level");
		while (levelNode)
		{
			loadLevel++;
			theEngine->showLoadingText("SETTING UP: LEVEL "+std::to_string(loadLevel)+" OF " +std::to_string(lCount));
			std::string levelName = levelNode->Attribute("levelName");

			std::string resourceFolder = levelNode->Attribute("resourceFolder");
			const char* xml = levelNode->Attribute("xmlFile");
			std::string xmlFile;
			if (xml)
				xmlFile = xml;
			std::string xmlPath;
			if (xmlFile != "")
				xmlPath = dir + resourceFolder + "/" + xmlFile;
			else
				xmlPath = dir + resourceFolder + "/" + resourceFolder+".xml";

			m_levels[levelName].m_xmlPath = xmlPath;//m_terrain.readXML(xmlPath);

			const char* state = levelNode->Attribute("gamestate");
			if (state)
				m_levels[levelName].m_terrain.setGameState(state);

			auto gateNode = levelNode->FirstChildElement("Gateway");
			while (gateNode)
			{
				float top = gateNode->FloatAttribute("top");
				float left = gateNode->FloatAttribute("left");
				float width = gateNode->FloatAttribute("width");
				float height = gateNode->FloatAttribute("height");
				sf::FloatRect rect(left, top, width, height);
				std::string destination = gateNode->Attribute("destinationLevel");
				float dstX = gateNode->FloatAttribute("dstPointX");
				float dstY = gateNode->FloatAttribute("dstPointY");
				bool active = gateNode->BoolAttribute("active", true);
				const char* req = gateNode->Attribute("requires");
				std::string requires;
				if (req)
					requires = req;

				const char* rej = gateNode->Attribute("reject");
				std::string reject;
				if (rej)
					reject = rej;

				m_levels[levelName].m_links.push_back({ rect, destination, {dstX, dstY}, active, requires, reject });
				gateNode = gateNode->NextSiblingElement("Gateway");
			}

			const char* bgm = levelNode->Attribute("bgm");
			std::string music = "";
			if (bgm)
				music = bgm;
			m_levels[levelName].m_bgm = music;

			const char* pl = levelNode->Attribute("preload");
			std::string preload = "";
			if (pl)
				preload = pl;
			int plposn = 0;
			while ((plposn = preload.find(",")) != std::string::npos)
			{
				m_levels[levelName].preloadLevels.push_back(preload.substr(0, plposn));
				preload.erase(0, plposn+1);
			}
			if(preload.size() > 0u)
				m_levels[levelName].preloadLevels.push_back(preload);

			levelNode = levelNode->NextSiblingElement("Level");
		}
	}
}

sf::Vector2f World::checkGateways(_IN_VEC2 charPos)
{
	for (auto& g : m_levels[m_activeLevel].m_links)
	{
		if (g.m_area.contains(charPos))
		{
			if (g.m_active)
			{
				setCurrentLevel(g.m_destinationName);
				return g.m_destinationPosition;
			}
			else if (g.m_requires != "")
			{
				if (!theEngine->getPlayer()->checkInventory(g.m_requires))
					theEngine->dialog->startDialog(g.m_rejectDialog);
				else
					g.m_active = true;
			}
		}
		else if (g.m_requires != "" && g.m_rejectDialog != "" && !theEngine->getPlayer()->checkInventory(g.m_requires))
		{
			theEngine->dialog->rewindDialog(g.m_rejectDialog);
		}
	}

	return { -1, -1 };
}

World::Gateway* World::getGatewayToLevel(_IN_STR level)
{
	for (auto& g : m_levels[m_activeLevel].m_links)
	{
		if (g.m_destinationName == level)
			return &g;
	}
	return nullptr;
}

GfxBase * World::getAsset(_IN_STR id)
{
	return m_levels[m_activeLevel].m_terrain.getAsset(id);
}

void World::loadLevel(_IN_STR id)
{
	m_levels[id].m_terrain.readXML(m_levels[id].m_xmlPath, id);
}

//std::thread World::loadLevelThread(_IN_STR id)
//{
//	return std::thread();
//}
