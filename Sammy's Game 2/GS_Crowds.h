#pragma once

#include "GameStateBase.h"
#include "engine.h"


class GS_Crowds : public GameStateBase
{
public:
	GS_Crowds();
	~GS_Crowds();

	void enter();
	void run();
	void exit();

	std::vector<std::pair<std::string, sf::Vector2f>> m_crowdMoves;
};

