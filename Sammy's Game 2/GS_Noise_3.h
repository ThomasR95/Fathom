#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "Anim.h"
#include "SFML\System\Clock.hpp"


class GS_Noise_3 : public GameStateBase
{
public:
	GS_Noise_3();
	~GS_Noise_3();

	void enter();
	void run();
	void exit();

	Anim* m_eyes;
	Anim* m_teeth;

	bool first = true;

	sf::Clock m_eyeClock;
	int m_blinkTime;

	std::vector<Obstacle*> m_buttons;
	std::vector<Anim*> m_hints;

	std::string m_solution = "12431421";
	std::string m_phoneCode;
	bool m_phoneUnlocked = false;
};

