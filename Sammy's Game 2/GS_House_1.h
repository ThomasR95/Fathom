#pragma once

#include "GameStateBase.h"
#include "engine.h"

#include "tMovie.h"

class GS_House_1 : public GameStateBase
{
public:
	GS_House_1();
	~GS_House_1();

	void enter();
	void run();
	void exit();

	bool m_moviePlayed = false;
	tMovie m_video;
};

