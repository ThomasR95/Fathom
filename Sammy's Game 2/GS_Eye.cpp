#include "GS_Eye.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Eye::GS_Eye()
{
	m_dialog = {
		"Eye_1",
		"Eye_2",
		"Eye_3",
		"Eye_4",
		"Eye_5",
		"Eye_6",
		"Eye_7",
		"Eye_8"
	};

	m_lastDialog = "";
}


GS_Eye::~GS_Eye()
{
}

void GS_Eye::enter()
{
	theEngine->setNextState("GS_EXIT");

	m_pupil = (tSprite*)theEngine->getCurrentWorld()->getAsset("pupil");
	m_pupil->setOrigin(tSprite::CENTRE_CENTRE);

	m_pupil->sprite.setPosition(GameObjects::thePlayer.getFeetPosition());

	m_pupil->sprite.setScale({ 1.2f,1.2f });

	GameObjects::thePlayer.run(true);
	GameObjects::thePlayer.setPosition({ 2000,1000 });
	sf::Mouse::setPosition(sf::Vector2i(theEngine->worldToScreenCoords({ 2000,2000 })), *theEngine->getWindow());

	m_dialogPos = 0;
}

void GS_Eye::run()
{
	if (m_first)
	{
		GameObjects::thePlayer.setPosition({ 2000,1000 });
		sf::Mouse::setPosition(sf::Vector2i(theEngine->worldToScreenCoords({ 2000,2000 })), *theEngine->getWindow());
		m_pupil->sprite.setPosition({2000,1000});
		m_first = false;
	}

	auto playerPos = GameObjects::thePlayer.getFeetPosition();
	auto pupilPos = m_pupil->sprite.getPosition();
	auto toPlayer = playerPos - pupilPos;
	m_pupil->sprite.setPosition(pupilPos + (norm(toPlayer)*len(toPlayer)*0.02f));

	if (m_lastDialog == "" && m_waitClock.getElapsedTime().asMilliseconds() > 3000 && !m_dialogEnded)
	{

		tEffector::transforms t; t.scale = { 1.2f, 1.2f };
		tEffector::activity a; a.scale = true;
		tEffector::times times; times.scale = 800;

		if (GameObjects::thePlayer.getVelocity() == sf::Vector2f(0, 0) || m_dialogPos < 3)
		{
			theEngine->dialog->startDialog(m_dialog[m_dialogPos]);
			m_lastDialog = m_dialog[m_dialogPos];
			if (m_dialogPos >= m_dialog.size() - 1)
			{
				m_dialogEnded = true;
				GameObjects::thePlayer.run(false);
				GameObjects::thePlayer.followMouse(false);
			}
			m_dialogPos++;
		}
		else
		{
			theEngine->dialog->startDialog("Eye_6");
			m_lastDialog = "Eye_6";

			times.scale = 300;
			t.scale = { 1,1 };
		}

		m_pupil->addEffector(tEffector(t, a, times));
		
	}
	else if(m_lastDialog != "" && theEngine->dialog->isDialogFinished() && !m_dialogEnded)
	{
		m_waitClock.restart();
		m_lastDialog = "";
	}

	if (m_dialogEnded && theEngine->dialog->isDialogFinished() && theEngine->dialog->getEntry("Eye_12")->readOnce && !m_fallen)
	{
		GameObjects::thePlayer.followMouse(false);
		tEffector::transforms t; 
		t.scale = { 0.5f, 0.5f }; 
		t.colour = { 255,255,255,0 }; 
		t.position = GameObjects::thePlayer.getFeetPosition();
		t.position.y += 500;
		tEffector::activity a; 
		a.scale = true;
		a.colour = true;
		a.position = true;

		GameObjects::thePlayer.addEffector(tEffector(t, a, 2000, tInterpolator::type::QUADRATIC_UP));
		resMan->getSound("../sounds/Triggered/Fall through pupil noise (transition to next place).ogg")->play();
		m_fallen = true;
	}

	if (m_fallen && GameObjects::thePlayer.getNumEffectors() == 0)
	{
		GameObjects::thePlayer.visible = false;
		GameObjects::thePlayer.setColour({ 255,255,255,255 });
		GameObjects::thePlayer.setScale({ 1.f,1.f });
		GameObjects::thePlayer.setPosition({1100, 2096});

		theEngine->getCurrentWorld()->setCurrentLevel("Lead to final room");
	}
}

void GS_Eye::exit()
{
	GameObjects::thePlayer.visible = true;
}
