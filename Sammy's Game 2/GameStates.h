#pragma once
#include "typedefs.h"

#include <string>
#include <map>
#include <memory>
#include "GameStateBase.h"


class GameStates
{
public:
	GameStates();
	~GameStates();

	std::string lastState;
	std::string currentState;
	std::string nextState;

	void signalExit();
	void signalExitTo(_IN_STR state);

	void addState(_IN_STR id, GameStateBase* pState) { m_gameStates[id] = std::shared_ptr<GameStateBase>(pState); }

	void initStates();

	void runCurrent();

private:
	std::map<std::string, std::shared_ptr<GameStateBase>> m_gameStates;
};

