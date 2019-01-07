#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "Obstacle.h"


class GS_Noise_2 : public GameStateBase
{
public:
	GS_Noise_2();
	~GS_Noise_2();

	void enter();
	void run();
	void exit();

	bool first = true;

	std::vector<Obstacle*> m_runes;
	std::vector<Anim*> m_hints;
};

