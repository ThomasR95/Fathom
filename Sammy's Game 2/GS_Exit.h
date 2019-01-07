#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Exit : public GameStateBase
{
public:
	GS_Exit();
	~GS_Exit();

	void enter();
	void run();
	void exit();
};

