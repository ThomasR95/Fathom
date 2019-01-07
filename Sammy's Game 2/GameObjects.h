#pragma once
#include "Obstacle.h"
#include "Player.h"
#include "World.h"
#include "NPC.h"
#include "tCursor.h"

class GameObjects
{
public:
	GameObjects();
	~GameObjects();

	static void setupVisuals();

	static std::vector<Obstacle> obstacles;
	static Player thePlayer;

	static NPC hound;
	static NPC gardener;

	static tCursor theCursor;

	static World theWorld;

	static tSprite* m_blackout;

};

