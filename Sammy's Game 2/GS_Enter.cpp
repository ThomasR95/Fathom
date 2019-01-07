#include "GS_Enter.h"
#include "GameObjects.h"
#include "Dialog.h"
#include "resourceManager.h"

GS_Enter::GS_Enter()
{
	theEngine->setNextState("GS_IDLE");
	theEngine->hideLoadingText();
	m_menuScreen = resMan->addSprite("Menu_BG", "GUI/Main menu screen.jpg");
	m_menuScreen->visible = true;
	theEngine->addtoRenderList(m_menuScreen);
}


GS_Enter::~GS_Enter()
{
}

void GS_Enter::enter()
{
	auto music = resMan->getMusic("../sounds/Ambiance/Menu screen crickets.ogg");
	music->play();
	music->setLoop(true);
}

void GS_Enter::run()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && theEngine->getWindow()->hasFocus())
	{
		GameObjects::m_blackout->visible = true;
		GameObjects::m_blackout->sprite.setColor({ 255,255,255,255 });
		GameObjects::thePlayer.setDir(3);
		GameObjects::thePlayer.active = true;
		GameObjects::thePlayer.visible = true;
		GameObjects::theWorld.setCurrentLevel("Level_1");
		m_menuScreen->visible = false;
		theEngine->sortRenderList();
	}
}

void GS_Enter::exit()
{
	resMan->getMusic("../sounds/Ambiance/Menu screen crickets.ogg")->stop();
}
