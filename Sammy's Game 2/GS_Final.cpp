#include "GS_Final.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Final::GS_Final()
{
}


GS_Final::~GS_Final()
{
}

void GS_Final::enter()
{
	if (!m_video.video.openFromFile("../images/Cutscenes/Ending Cutscene_cropped.mkv"))
		__debugbreak();
	theEngine->setNextState("GS_EXIT");

	auto pRock = (Anim*)theEngine->getCurrentWorld()->getAsset("Enid_rock");
	auto bounds = pRock->sprite.getGlobalBounds();
	m_enidButton = tButton(bounds);
	m_enidButton.setCursorType("");
	theEngine->addButton(&m_enidButton);
	m_enidButton.setEnabled(tButton::bState::ON);

	m_enidPosition = pRock->sprite.getPosition();
	m_enidPosition += {bounds.width / 2, bounds.height / 2};
}

void GS_Final::run()
{

	auto toEnid = GameObjects::thePlayer.getFeetPosition() - m_enidPosition;

	if (!m_moviePlayed && m_enidButton.isClicked() && len(toEnid) < 500)
	{
		m_video.Zlevel = -2000;
		theEngine->addtoRenderList(&m_video);
		m_video.visible = true;
		m_video.video.fit(0, 0, 4096, 2897, true);
		m_video.video.play();
		m_moviePlayed = true;

		theEngine->enableMouse(false);
		theEngine->dialog->active = false;
	}
}

void GS_Final::exit()
{
}
