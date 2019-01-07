#pragma once
class GameStateBase
{
public:
	GameStateBase();
	~GameStateBase();

	virtual void enter();
	virtual void run();
	virtual void exit();
};

