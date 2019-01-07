#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "Anim.h"


class GS_Level_5 : public GameStateBase
{
public:
	GS_Level_5();
	~GS_Level_5();

	void enter();
	void run();
	void exit();

	enum state
	{
		IDLE,
		BLOCKING,
		TRANSFORM,
		CHASING,
		FADEOUT
	};

	state m_state;

	sf::Vector2f m_gardenerPosition;
	bool first = true;
	Anim* pGardener;
	Anim m_gardenerBlock;
	Anim m_gardenerTransform;

	sf::Clock m_blockClock;

	sf::Vector2f m_playerRespawn;
};

