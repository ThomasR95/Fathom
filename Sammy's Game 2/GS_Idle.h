#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Idle : public GameStateBase
{
public:
	GS_Idle();
	~GS_Idle();

	void enter();
	void run();
	void exit();

};

