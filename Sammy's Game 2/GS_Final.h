#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "tMovie.h"



class GS_Final : public GameStateBase
{
public:
	GS_Final();
	~GS_Final();

	void enter();
	void run();
	void exit();

	bool m_moviePlayed = false;
	tMovie m_video;

	tButton m_enidButton;

	sf::Vector2f m_enidPosition;
};

