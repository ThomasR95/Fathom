#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "tButton.h"
#include "Anim.h"


class GS_Level_2 : public GameStateBase
{
public:
	GS_Level_2();
	~GS_Level_2();

	void enter();
	void run();
	void exit();

	enum state
	{
		UNBROKEN,
		BREAKING,
		BROKEN
	};

private:

	tButton m_rockButton;

	bool m_first = true;
	bool m_firstRun = true;

	Anim m_rockBreak;
	Anim m_houndSwipe;

	state m_state = UNBROKEN;
};

