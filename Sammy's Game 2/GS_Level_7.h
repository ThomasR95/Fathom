#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Level_7 : public GameStateBase
{
public:
	GS_Level_7();
	~GS_Level_7();

	void enter();
	void run();
	void exit();

	sf::Clock m_blockClock;

	tSprite* m_dragonHead;

	bool m_first = true;
};

