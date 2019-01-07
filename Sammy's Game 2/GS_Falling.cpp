#include "GS_Falling.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Falling::GS_Falling()
{
}


GS_Falling::~GS_Falling()
{
}

void GS_Falling::enter()
{
	theEngine->setNextState("GS_EXIT");
	m_fallingTimer.restart();

	if (!m_video.video.openFromFile("../images/Cutscenes/falling scene (edit)_cropped.mkv"))
		__debugbreak();

	theEngine->dialog->setBGImage(*resMan->addSprite("overloadBG", "GUI/overload dialog box.png", { 414, 2000 }));

	theEngine->addtoRenderList(&m_video);
}

void GS_Falling::run()
{
	if (first)
	{
		m_video.video.fit(0, 0, 4096, 2897, true);
		m_video.video.play();
		m_video.visible = true;
		first = false;
	}

	if (m_video.video.getStatus() == sfe::Status::Stopped)
	{
		theEngine->getCurrentWorld()->setCurrentLevel("pre-crowds");
		theEngine->getPlayer()->sprite.setPosition({ 1824, 1622 });
	}
}

void GS_Falling::exit()
{
	theEngine->getCurrentWorld()->unloadLevel("Level_1");
	theEngine->getCurrentWorld()->unloadLevel("Level_2");
	theEngine->getCurrentWorld()->unloadLevel("Level_3");
	theEngine->getCurrentWorld()->unloadLevel("Level_4");
	theEngine->getCurrentWorld()->unloadLevel("Level_5");
	theEngine->getCurrentWorld()->unloadLevel("Level_6");
	theEngine->getCurrentWorld()->unloadLevel("Level_7");
	theEngine->getCurrentWorld()->unloadLevel("House");
	theEngine->getCurrentWorld()->unloadLevel("House_2");
	theEngine->getCurrentWorld()->unloadLevel("House_3");
	theEngine->getCurrentWorld()->unloadLevel("House_4");

	GameObjects::thePlayer.visible = true;

	m_video.video.stop();
	m_video.visible = false;
	m_video.active = false;
}
