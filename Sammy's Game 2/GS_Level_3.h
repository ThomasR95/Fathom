#pragma once

#include "GameStateBase.h"
#include "engine.h"



class GS_Level_3 : public GameStateBase
{
public:
	GS_Level_3();
	~GS_Level_3();

	void enter();
	void run();
	void exit();

	bool dogFed = false;
};

