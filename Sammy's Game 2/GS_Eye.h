#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Eye : public GameStateBase
{
public:
	GS_Eye();
	~GS_Eye();

	void enter();
	void run();
	void exit();

	tSprite* m_pupil;

	bool m_first = true;

	sf::Clock m_waitClock;
	std::vector<std::string> m_dialog;
	int m_dialogPos = 0;
	std::string m_lastDialog;
	bool m_dialogEnded = false;
	bool m_fallen = false;
};

