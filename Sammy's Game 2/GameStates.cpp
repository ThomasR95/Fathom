#include "typedefs.h"

#include "GameStates.h"

#include "GS_Enter.h"
#include "GS_Idle.h"
#include "GS_Exit.h"
#include "GS_Default.h"
#include "GS_Level_1.h"
#include "GS_Level_2.h"
#include "GS_Level_3.h"
#include "GS_Level_4.h"
#include "GS_Level_5.h"
#include "GS_Level_6.h"
#include "GS_Level_7.h"
#include "GS_House_1.h"

#include "GS_Falling.h"
#include "GS_Crowds.h"
#include "GS_Noise_2.h"
#include "GS_Noise_3.h"
#include "GS_Eye.h"
#include "GS_Final.h"

GameStates::GameStates()
{
}


GameStates::~GameStates()
{
}

void GameStates::signalExit()
{
	lastState = currentState;
	if(currentState != "")
		m_gameStates[currentState]->exit();

	if (nextState == "")
		theEngine->close();
	else
	{
		currentState = nextState;
		m_gameStates[currentState]->enter();
	}
}

void GameStates::signalExitTo(_IN_STR state)
{
	nextState = state;
	signalExit();
}

void GameStates::initStates()
{
	addState("GS_ENTER", new GS_Enter());
	addState("GS_IDLE", new GS_Idle());
	addState("GS_EXIT", new GS_Exit());
	addState("GS_DEFAULT", new GS_Default());
	addState("GS_LEVEL_1", new GS_Level_1());
	addState("GS_LEVEL_2", new GS_Level_2());
	addState("GS_LEVEL_3", new GS_Level_3());
	addState("GS_LEVEL_4", new GS_Level_4());
	addState("GS_LEVEL_5", new GS_Level_5());
	addState("GS_LEVEL_6", new GS_Level_6());
	addState("GS_LEVEL_7", new GS_Level_7());
	addState("GS_HOUSE_1", new GS_House_1());

	addState("GS_FALLING", new GS_Falling());
	addState("GS_CROWDS", new GS_Crowds());
	addState("GS_NOISE_2", new GS_Noise_2());
	addState("GS_NOISE_3", new GS_Noise_3());
	addState("GS_EYE", new GS_Eye());
	addState("GS_FINAL", new GS_Final());
}

void GameStates::runCurrent()
{
	m_gameStates[currentState]->run();
}
