#include "GS_Level_2.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_2::GS_Level_2()
{
	m_rockBreak = Anim(*resMan->getTexture("Levels/Level_2/room 2 rock break_2.png"), { 0,0 }, 8, 4, 2, { 0,0 }, { -1,-1 }, tSprite::TOP_LEFT, 12.0f, false);
	m_rockBreak.pause();
	m_rockBreak.visible = false;
	m_rockBreak.active = false;
	m_rockBreak.m_UID = "rock_break_anim";

	m_houndSwipe = Anim(*resMan->getTexture("NPCs/Hound break rock.png"), { 0,0 }, 8, 2, 4, { 0,0 }, { -1,-1 }, tSprite::BOTTOM_CENTRE, 12.0f, false);
	m_houndSwipe.pause();
	m_houndSwipe.visible = false;
	m_houndSwipe.active = false;
	m_houndSwipe.m_UID = "hound_swipe_anim";

	m_state = UNBROKEN;
}


GS_Level_2::~GS_Level_2()
{
}

void GS_Level_2::enter()
{
	theEngine->setNextState("GS_EXIT");
	if (m_first)
	{
		auto pRock = (tSprite*)theEngine->getCurrentWorld()->getAsset("rock_0");
		auto bounds = pRock->sprite.getGlobalBounds();
		m_rockButton = tButton(bounds);
		m_rockButton.m_UID = "rock_button";
		m_rockButton.setCursorType("EYE");
		theEngine->addButton(&m_rockButton);

		m_first = false;
	}

	if(m_state == UNBROKEN)
		m_rockButton.setEnabled(tButton::ON);

	if (m_state == BROKEN)
	{
		auto pRock = (tSprite*)theEngine->getCurrentWorld()->getAsset("rock_0");
		pRock->active = false;
		pRock->visible = false;
	}

	if (theEngine->getPlayer()->checkInventory("Hound"))
	{
		GameObjects::hound.visible = true;
		GameObjects::hound.active = true;
		GameObjects::hound.setInteraction("", false, false);
		
		if (m_state == UNBROKEN)
		{
			m_rockButton.setCursorType("KEY");
			GameObjects::hound.setPosition({ 2332, 2149 });
			//GameObjects::hound.moveTo({1771, 781});
		}
		else
		{
			m_rockButton.setCursorType("");
			GameObjects::hound.setPosition({ 1308, 781 });
			m_rockButton.setEnabled(tButton::OFF);
		}
	}
	
}

void GS_Level_2::run()
{

	switch (m_state)
	{
	case GS_Level_2::UNBROKEN:
		if (theEngine->getPlayer()->checkInventory("Hound") && m_firstRun)
		{
			m_firstRun = false;
			GameObjects::hound.setPosition({ 2332, 2149 });
			GameObjects::hound.moveTo({ 1771, 781 });
		}

		if (m_rockButton.isClicked())
		{
			if (theEngine->getPlayer()->checkInventory("Hound"))
			{
				theEngine->inhibitPlayerMovement(true);
				auto pRock = (tSprite*)theEngine->getCurrentWorld()->getAsset("rock_0");
				pRock->active = false;
				pRock->visible = false;

				m_rockBreak.sprite.setPosition(pRock->sprite.getPosition());
				m_rockBreak.visible = true;
				m_rockBreak.active = true;
				m_rockBreak.Zlevel = pRock->Zlevel;
				m_rockBreak.restart();
				theEngine->addtoRenderList(&m_rockBreak);
				theEngine->sortRenderList();

				GameObjects::hound.visible = false;

				m_houndSwipe.sprite.setPosition(GameObjects::hound.sprite.getPosition());
				m_houndSwipe.visible = true;
				m_houndSwipe.active = true;
				m_houndSwipe.Zlevel = GameObjects::hound.Zlevel;
				m_houndSwipe.restart();
				theEngine->addtoRenderList(&m_houndSwipe);
				theEngine->sortRenderList();

				m_rockButton.setCursorType("");
				m_rockButton.active = false;
				m_rockButton.setEnabled(tButton::bState::OFF);
				theEngine->removeButton(&m_rockButton);

				resMan->getSound("../sounds/Triggered/rock break.ogg")->play();

				m_state = BREAKING;
			}
			else
				theEngine->dialog->startDialog("L2_Rock");

		}
		break;
	case GS_Level_2::BREAKING:
		if (m_rockBreak.isFinished())
		{
			theEngine->inhibitPlayerMovement(false);
			m_houndSwipe.visible = false;
			m_houndSwipe.active = false;
			GameObjects::hound.visible = true;
			m_state = BROKEN;
			m_rockBreak.visible = false;
			theEngine->dialog->startDialog("L2_GoodBoy");
		}
		break;
	case GS_Level_2::BROKEN:
		break;
	default:
		break;
	}

}

void GS_Level_2::exit()
{
	m_rockBreak.visible = false;
	m_rockBreak.active = false;
	m_rockButton.setEnabled(tButton::OFF);

	GameObjects::hound.visible = false;
	GameObjects::hound.active = false;
}
