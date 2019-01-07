#include "GS_Level_6.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_6::GS_Level_6()
{
}


GS_Level_6::~GS_Level_6()
{
}

void GS_Level_6::enter()
{
	theEngine->setNextState("GS_EXIT");
	m_blockClock.restart();

	GameObjects::gardener.setCollisionEnabled(false);
}

void GS_Level_6::run()
{
	if (m_firstRun)
	{
		GameObjects::thePlayer.active = true;
		sf::Mouse::setPosition(sf::Vector2i(theEngine->worldToScreenCoords({ 2000,1000 })), *theEngine->getWindow());
		m_firstRun = false;
	}
	if (m_blockClock.getElapsedTime().asMilliseconds() > 300)
	{
		GameObjects::gardener.clearTargetQ();
		GameObjects::gardener.moveTo(theEngine->getPlayer()->sprite.getPosition());
		m_blockClock.restart();
	}
}

void GS_Level_6::exit()
{
	GameObjects::gardener.clearTargetQ();
	GameObjects::gardener.setPosition({ -100, 1600 });
}
