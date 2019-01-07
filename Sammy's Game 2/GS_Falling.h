#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "tMovie.h"


class GS_Falling : public GameStateBase
{
public:
	GS_Falling();
	~GS_Falling();

	void enter();
	void run();
	void exit();


	sf::Clock m_fallingTimer;
	
	tMovie m_video;

	bool first = true;
};

