#include "GS_Level_5.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_5::GS_Level_5()
{
	m_state = IDLE;
}


GS_Level_5::~GS_Level_5()
{
}

void GS_Level_5::enter()
{
	theEngine->setNextState("GS_EXIT");

	if (first)
	{
		m_gardenerBlock = Anim(*resMan->getTexture("NPCs/Gardener block.png"), { 0,0 }, 12, 4, 3, { 0,0 }, { -1,-1 }, tSprite::TOP_LEFT, 12.0f, false);
		m_gardenerBlock.pause();
		m_gardenerBlock.visible = false;
		m_gardenerBlock.active = false;
		m_gardenerBlock.m_UID = "gardener_block";
		m_gardenerBlock.sprite.setPosition(3441, 161);

		m_gardenerTransform = Anim(*resMan->getTexture("NPCs/Gardener transform.png"), { 0,0 }, 6, 1, 6, { 0,0 }, { -1,-1 }, tSprite::TOP_LEFT, 12.0f, false);
		m_gardenerTransform.pause();
		m_gardenerTransform.visible = false;
		m_gardenerTransform.active = false;
		m_gardenerTransform.m_UID = "gardener_transform";
		m_gardenerTransform.sprite.setPosition(3441, 161);

		pGardener = (Anim*)theEngine->getCurrentWorld()->getAsset("Gardener");
		auto size = pGardener->getSize();
		m_gardenerPosition = pGardener->sprite.getPosition();
		m_gardenerPosition.x += size.x / 2;
		m_gardenerPosition.y += size.y;

		theEngine->addtoRenderList(&m_gardenerBlock);
		theEngine->addtoRenderList(&m_gardenerTransform);

		first = false;
	}
	
}

void GS_Level_5::run()
{
	switch (m_state)
	{
	case GS_Level_5::IDLE:
	{
		auto toGardener = theEngine->getPlayer()->sprite.getPosition() - m_gardenerPosition;
		if (len(toGardener) < 300 && m_blockClock.getElapsedTime().asMilliseconds() > 3000)
		{
			pGardener->visible = false;
			m_gardenerBlock.visible = true;
			m_gardenerBlock.active = true;
			m_gardenerBlock.Zlevel = pGardener->Zlevel;
			m_gardenerBlock.restart();
			theEngine->dialog->startDialog("L3_GardenerBlock");
			m_state = BLOCKING;
			theEngine->inhibitPlayerMovement(true);
			theEngine->getPlayer()->stopMouseInput();
		}

		if (theEngine->dialog->getEntry("L3_GardenerAngry")->readOnce && theEngine->dialog->isDialogFinished())
		{
			theEngine->dialog->getEntry("L3_Daffodils")->active = false;
			theEngine->dialog->getEntry("L3_Mushrooms")->active = false;
			theEngine->dialog->getEntry("L3_Cart")->active = false;
			theEngine->dialog->getEntry("L3_GardenerBlock")->active = false;
			m_state = TRANSFORM;
			pGardener->visible = false;
			m_gardenerTransform.visible = true;
			m_gardenerTransform.active = true;
			m_gardenerTransform.Zlevel = pGardener->Zlevel;
			m_gardenerTransform.restart();
		}

		break;
	}
	case GS_Level_5::BLOCKING:
	{
		auto pos = theEngine->getPlayer()->sprite.getPosition();
		auto toGardener = pos - m_gardenerPosition;
		if (len(toGardener) < 500)
		{
			auto dir = norm(toGardener);
			dir.x *= 5;
			dir.y *= 5;
			theEngine->getPlayer()->setPosition(pos + dir);
		}

		if (m_gardenerBlock.isFinished() && !pGardener->visible)
		{
			m_gardenerBlock.visible = false;
			pGardener->visible = true;
			m_blockClock.restart();
		}
		else if (m_gardenerBlock.isFinished() && theEngine->dialog->isDialogFinished())
		{
			m_state = IDLE;
			theEngine->inhibitPlayerMovement(false);
		}
		break;
	}
	case GS_Level_5::TRANSFORM:
		if (m_gardenerTransform.isFinished())
		{
			m_state = CHASING;
			pGardener->visible = false;
			pGardener->active = false;
			pGardener->setCollisionEnabled(false);
			m_gardenerTransform.visible = false;
			GameObjects::gardener.visible = true;
			GameObjects::gardener.setPosition(m_gardenerPosition);
			GameObjects::gardener.active = true;
			GameObjects::gardener.moveTo(theEngine->getPlayer()->sprite.getPosition());
			theEngine->inhibitPlayerMovement(false);
			m_blockClock.restart();
			auto block = (Anim*)theEngine->getCurrentWorld()->getAsset("BLOCK");
			block->visible = false;
			block->active = false;
			theEngine->getPlayer()->run(true);
			sf::Mouse::setPosition(sf::Vector2i(theEngine->worldToScreenCoords({ 948,1926 })), *theEngine->getWindow());

			m_playerRespawn = theEngine->getPlayer()->sprite.getPosition();
		}
		break;
	case GS_Level_5::CHASING:
	{
		if (m_blockClock.getElapsedTime().asMilliseconds() > 300)
		{
			GameObjects::gardener.clearTargetQ();
			GameObjects::gardener.moveTo(theEngine->getPlayer()->sprite.getPosition());
			m_blockClock.restart();
		}
		auto pos = GameObjects::thePlayer.sprite.getPosition();
		auto gPos = GameObjects::gardener.sprite.getPosition();
		auto btwn = pos - gPos;
		float dist = len(btwn);
		if (dist < 300)
		{
			tEffector::activity a; a.colour = true;
			tEffector::transforms t; t.colour = { 255,255,255,255 };
			tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN);
			GameObjects::m_blackout->addEffector(fade);
			GameObjects::m_blackout->visible = true;
			GameObjects::m_blackout->active = true;
			m_state = FADEOUT;
		}

		break;
	}	
	case GS_Level_5::FADEOUT:
		if (GameObjects::m_blackout->getNumEffectors() == 0)
		{
			GameObjects::gardener.setPosition(m_gardenerPosition);
			GameObjects::thePlayer.setPosition(m_playerRespawn);
			sf::Mouse::setPosition(sf::Vector2i(theEngine->worldToScreenCoords({ 948,1926 })), *theEngine->getWindow());
			m_state = CHASING;

			GameObjects::m_blackout->clearEffectors();
			tEffector::activity a; a.colour = true;
			tEffector::transforms t; t.colour = { 255,255,255,0 };
			tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN, tEffector::POINT_TO_POINT);
			GameObjects::m_blackout->addEffector(fade);
		}
		break;
	default:
		break;
	}
}

void GS_Level_5::exit()
{
	if (m_state == CHASING)
	{
		GameObjects::gardener.clearTargetQ();
		GameObjects::gardener.setPosition({ -100, 2800 });
		GameObjects::thePlayer.active = false;
		GameObjects::thePlayer.setVelocity({ 0,0 });
	}

	theEngine->getCurrentWorld()->unloadLevel("Level_1");
	theEngine->getCurrentWorld()->unloadLevel("Level_2");
	theEngine->getCurrentWorld()->unloadLevel("Level_3");
	theEngine->getCurrentWorld()->unloadLevel("Level_4");
		
}
