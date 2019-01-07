#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Default : public GameStateBase
{
public:
	GS_Default();
	~GS_Default();

	void enter();
	void run();
	void exit();
};

