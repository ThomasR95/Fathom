#pragma once

#include "GameStateBase.h"
#include "engine.h"
#include "tMovie.h"


class GS_Level_1 : public GameStateBase
{
public:
	GS_Level_1();
	~GS_Level_1();

	void enter();
	void run();
	void exit();

private:

	tButton m_noteButton;

	bool m_moviePlayed = false;
	tMovie m_video;
	bool firstRun = true;
	bool started = false;
	bool m_first = true;
};

