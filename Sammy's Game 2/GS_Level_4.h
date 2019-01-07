#pragma once

#include "GameStateBase.h"
#include "engine.h"

class lockReel : public GfxBase
{
public:
	virtual void tick(sf::Time& dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	void init(const std::vector<std::string>& numGfx, _IN_VEC2 centrePosn);
	int getNum() { return m_currentPosition; }
	void disableButtons();

	bool animFinished() 
	{
		bool done = true;
		for (auto& n : m_numbers)
			if (n.getNumEffectors() > 0u)
				done = false;
		return done;
	}

private:

	void up();
	void down();

	std::vector<std::string> m_texNames;
	std::vector<tSprite> m_numbers;
	std::vector<sf::Vector2f> m_positions;
	std::vector<int> m_ids;
	int m_currentPosition;

	sf::Vector2f m_numSize;
	tButton m_upButton;
	tButton m_downButton;
};



class GS_Level_4 : public GameStateBase
{
public:
	GS_Level_4();
	~GS_Level_4();

	void enter();
	void run();
	void exit();

	tButton m_hutDoor;

	bool m_showingCombiLock = false;
	bool m_combiLockDone = false;
	sf::Clock m_lockWaitTimer;

	tSprite* m_combiLockGUI;
	std::vector<lockReel> m_lockReels;

	bool m_first = true;
};

