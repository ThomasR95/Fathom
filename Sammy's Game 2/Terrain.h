#pragma once
#include "typedefs.h"

#include "Obstacle.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

	void readXML(_IN_STR path, _IN_STR levelName, bool graphicsOnly = false);

	void setVisible(bool vis);
	void setActive(bool active);

	void addToRenderlist(bool force = false);

	void addCollisionObjects();
	void removeCollisionObjects();

	void setGameState(_IN_STR state) { m_gameState = state; }
	_IN_STR getGameState() { return m_gameState; }

	GfxBase* getAsset(_IN_STR id);
private:

	friend class World;

	bool m_XMLLoaded = false;

	Obstacle m_background;
	std::vector<Obstacle> m_obstacles;
	std::vector<std::string> m_dialogTriggers;
	std::string m_startDialog;
	Obstacle m_foreground;

	bool m_inRenderList = false;
	bool m_collisionAdded = false;

	std::string m_gameState = "GS_DEFAULT";
};

