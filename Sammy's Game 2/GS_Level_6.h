#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Level_6 : public GameStateBase
{
public:
	GS_Level_6();
	~GS_Level_6();

	void enter();
	void run();
	void exit();

	sf::Clock m_blockClock;
	bool m_firstRun = true;
};

