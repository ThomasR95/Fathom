#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Enter : public GameStateBase
{
public:
	GS_Enter();
	~GS_Enter();

	void enter();
	void run();
	void exit();

	tSprite* m_menuScreen;
};

