#include "GS_Level_7.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_7::GS_Level_7()
{
	m_first = true;
}


GS_Level_7::~GS_Level_7()
{
}

void GS_Level_7::enter()
{
	theEngine->setNextState("GS_EXIT");
	m_blockClock.restart();
	if (m_first)
	{
		m_dragonHead = (tSprite*)theEngine->getCurrentWorld()->getAsset("dragonhead");
		auto bounds = m_dragonHead->sprite.getGlobalBounds();
		sf::Vector2f size(bounds.width, bounds.height);
		m_dragonHead->setOrigin(tSprite::BOTTOM_RIGHT);
		m_dragonHead->sprite.move(size);
		m_dragonHead->active = true;

		tEffector::activity a; a.colour = true;
		tEffector::transforms t; t.colour = { 255,255,255,0 };
		tEffector fade(t, a, 1500, tInterpolator::QUADRATIC_UP_DOWN, tEffector::POINT_TO_POINT);
		GameObjects::gardener.addEffector(fade);
		m_first = false;
	}
}

void GS_Level_7::run()
{
	if (m_dragonHead->getNumEffectors() == 0)
	{
		tEffector::activity a; a.rotation = true;
		tEffector::transforms t; t.rotation = 5;
		tEffector rotUp(t, a, 3000, tInterpolator::QUADRATIC_UP_DOWN, tEffector::POINT_TO_POINT);
		t.rotation = 0;
		tEffector rotDown(t, a, 3000, tInterpolator::QUADRATIC_UP_DOWN, tEffector::POINT_TO_POINT);
		m_dragonHead->addEffector(rotUp);
		m_dragonHead->addEffector(rotDown);
		m_dragonHead->active = true;
		
	}

	if (GameObjects::gardener.getColour().a <= 1)
	{
		GameObjects::gardener.active = false;
		GameObjects::gardener.visible = false;
		GameObjects::thePlayer.run(false);
	}


	if (GameObjects::gardener.active && m_blockClock.getElapsedTime().asMilliseconds() > 300)
	{
		GameObjects::gardener.clearTargetQ();
		GameObjects::gardener.moveTo(theEngine->getPlayer()->sprite.getPosition());
		m_blockClock.restart();
	}
}

void GS_Level_7::exit()
{
	GameObjects::thePlayer.visible = false;
	GameObjects::gardener.visible = false;
	GameObjects::gardener.active = false;
	GameObjects::thePlayer.run(false);
}
