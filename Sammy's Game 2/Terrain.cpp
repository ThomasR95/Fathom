#include "Terrain.h"
#include "tinyxml2\tinyxml2.h"
#include "resourceManager.h"
#include "engine.h"

#include "Dialog.h"

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::readXML(_IN_STR path, _IN_STR levelName, bool graphicsOnly)
{
	if(m_XMLLoaded)
		return;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());
	if (doc.Error())
		return;

	theEngine->showLoadingText("LOADING...", false);

	int pos = 0;
	pos = path.rfind("/");
	auto dir = path.substr(0, pos+1);
	auto lName = path.substr(pos + 1);

	int backZ = 500;
	int Zincrement = 10;
	int progZ = backZ - Zincrement;

	auto terrain = doc.FirstChildElement("Terrain");
	if (terrain)
	{
		auto bgNode = terrain->FirstChildElement("Background");
		if (bgNode)
		{
			std::string texName = bgNode->Attribute("imageFile");

			std::string navName;
			const char* nav = bgNode->Attribute("navFile");
			if (nav) navName = nav;

			float scale = bgNode->IntAttribute("scale", 1);
			if(navName.size())
				m_background = Obstacle(resMan->getTexture(dir + texName), resMan->getTexture(dir + navName), { 0,0 });
			else
				m_background = Obstacle(resMan->getTexture(dir + texName), { 0,0 });

			m_background.setPlayerAlwaysInFront(true);
			m_background.m_UID = levelName + "_background";
			m_background.sprite.setScale({ scale, scale });
			m_background.Zlevel = backZ;
		}

		auto obst = terrain->FirstChildElement("Obstacle");
		while (obst)
		{
			std::string id = obst->Attribute("id");
			std::string texName = obst->Attribute("imageFile");
			std::string navName;
			const char* nav = obst->Attribute("navFile");
			if (nav) navName = nav;

			bool anim = obst->BoolAttribute("anim");
			float xpos = obst->FloatAttribute("posx");
			float ypos = obst->FloatAttribute("posy");
			bool collide = obst->BoolAttribute("collide", true);
			bool behind = obst->FloatAttribute("alwaysBehind", false);

			Obstacle newObst;
			if(!anim)
				newObst = Obstacle(resMan->getTexture(dir + texName), (navName.size() ? resMan->getTexture(dir + navName) : nullptr), { xpos,ypos });
			else
			{
				int frames = obst->IntAttribute("animFrames");
				int rows = obst->IntAttribute("rows");
				int cols = obst->IntAttribute("cols");
				int fps = obst->IntAttribute("fps", 12);
				newObst = Obstacle(resMan->getTexture(dir + texName), frames, rows, cols, (navName.size() ? resMan->getTexture(dir + navName) : nullptr), { xpos,ypos });
				newObst.setFPS(fps);
			}
			if (!collide || navName == "")
				newObst.setCollisionEnabled(false);
			if (behind)
				newObst.setPlayerAlwaysInFront(true);
			newObst.Zlevel = progZ;
			newObst.m_UID = id;

			const char* trig = obst->Attribute("trigger");
			if (trig)
			{
				newObst.setInteraction(trig, true);
				newObst.setCursorType("EYE");
			}

			m_obstacles.push_back(newObst);

			obst = obst->NextSiblingElement("Obstacle");

			progZ -= Zincrement;
		}

		m_foreground.m_UID = levelName + "_foreground";
		auto fgNode = terrain->FirstChildElement("Foreground");
		if (fgNode)
		{
			std::string texName = fgNode->Attribute("imageFile");
			float xpos = fgNode->FloatAttribute("posx");
			float ypos = fgNode->FloatAttribute("posy");
			
			m_foreground = Obstacle(resMan->getTexture(dir + texName), { xpos,ypos });
			m_foreground.Zlevel = progZ - Zincrement;

		}
	}


	if (!graphicsOnly)
	{
		auto dialog = doc.FirstChildElement("Dialog");
		if (dialog)
		{
			auto entry = dialog->FirstChildElement("Entry");
			while (entry)
			{
				std::string trigger = entry->Attribute("trigger");
				std::string speaker = entry->Attribute("speaker");
				float r = entry->FloatAttribute("r");
				float g = entry->FloatAttribute("g");
				float b = entry->FloatAttribute("b");
				std::string content = entry->Attribute("content");
				bool onLevelStart = entry->BoolAttribute("onStart", false);
				std::string endTrigger = "";
				if (onLevelStart)
					m_startDialog = trigger;
				Dialog::EndAction endAct = Dialog::REWIND;
				std::string endAction = entry->Attribute("endAction");
				if (endAction == "REWIND")
					endAct = Dialog::REWIND;
				else if (endAction == "DEACTIVATE")
					endAct = Dialog::DEACTIVATE;
				else
					endTrigger = endAction;

				std::string inventory;
				const char* inv = entry->Attribute("inventory");
				if (inv) inventory = inv;

				std::string requires;
				const char* req = entry->Attribute("requires");
				if (req) requires = req;

				std::string sound;
				const char* sfx = entry->Attribute("sfx");
				if (sfx) sound = sfx;

				float l = entry->FloatAttribute("btnL", 0);
				float t = entry->FloatAttribute("btnT", 0);
				float w = entry->FloatAttribute("btnW", -1);
				float h = entry->FloatAttribute("btnH", -1);

				tSprite* pGfx = nullptr;
				const char* gfx = entry->Attribute("gfx");
				if (gfx)
				{
					std::string graphic = gfx;
					float gfxX = entry->FloatAttribute("gfxX", 0);
					float gfxY = entry->FloatAttribute("gfxY", 0);

					pGfx = resMan->addSprite(trigger+"_DialogGfx", graphic, {gfxX, gfxY});
				}

				int time = entry->IntAttribute("time");

				theEngine->dialog->addEntry(trigger, content, speaker, { sf::Uint8(255 * r) , sf::Uint8(255 * g) , sf::Uint8(255 * b) , 255 }, endAct, endTrigger, {l,t,w,h}, levelName, inventory, pGfx, requires, time, sound);
				m_dialogTriggers.push_back(trigger);

				entry = entry->NextSiblingElement("Entry");
			}
		}
	}


	m_XMLLoaded = true;
}

void Terrain::setVisible(bool vis)
{
	m_background.visible = vis;
	for (auto& o : m_obstacles)
		o.visible = vis;
	m_foreground.visible = vis;
}

void Terrain::setActive(bool active)
{
	m_background.active = active;
	for (auto& o : m_obstacles)
		o.active = active;
	m_foreground.active = active;
}

void Terrain::addToRenderlist(bool force)
{
	if (m_inRenderList && !force)
		return;

	theEngine->addtoRenderList(&m_background);
	for (auto& o : m_obstacles)
		theEngine->addtoRenderList(&o);
	theEngine->addtoRenderList(&m_foreground);

	m_inRenderList = true;
}

void Terrain::addCollisionObjects()
{
	if (m_collisionAdded)
		return;

	theEngine->addCollisionObject(&m_background);
	for (auto& o : m_obstacles)
		theEngine->addCollisionObject(&o);

	theEngine->addCollisionObject(&m_foreground);

	m_collisionAdded = true;
}

void Terrain::removeCollisionObjects()
{
	if (!m_collisionAdded)
		return;

	theEngine->removeCollisionObject(&m_background);
	for (auto& o : m_obstacles)
		theEngine->removeCollisionObject(&o);

	theEngine->removeCollisionObject(&m_foreground);

	m_collisionAdded = false;
}

GfxBase * Terrain::getAsset(_IN_STR id)
{
	if (id == "FOREGROUND")
		return &m_foreground;
	else if (id == "BACKGROUND")
		return &m_background;

	for (auto& ob : m_obstacles)
		if (ob.m_UID == id)
			return &ob;

	return nullptr;
}
