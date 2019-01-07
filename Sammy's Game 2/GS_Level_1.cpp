#include "GS_Level_1.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_1::GS_Level_1()
{
	m_noteButton = tButton({ 3882, 1872, 200,200 });
	m_noteButton.setCursorType("EYE");
	theEngine->addButton(&m_noteButton);

	if (!m_video.video.openFromFile("../images/Cutscenes/Intro Cutscene_cropped.mkv"))
		__debugbreak();
}


GS_Level_1::~GS_Level_1()
{
}

void GS_Level_1::enter()
{
	theEngine->setNextState("GS_EXIT");

	if (m_first)
	{
		
		m_video.Zlevel = -2000;
		theEngine->addtoRenderList(&m_video);
		m_video.visible = true;

		theEngine->enableMouse(false);
		theEngine->dialog->active = false;

		m_first = false;
	}

	m_noteButton.setEnabled(tButton::bState::ON);
}

void GS_Level_1::run()
{
	if (firstRun)
	{
		m_video.video.fit(0, 0, 4096, 2897, true);
		m_video.video.play();
		m_moviePlayed = true;
		firstRun = false;
	}

	if (GameObjects::m_blackout->visible && m_video.video.getPlayingOffset() > m_video.video.getDuration() / 2.f)
	{
		GameObjects::m_blackout->visible = false;
	}

	if (!started && m_moviePlayed && m_video.video.getStatus() == sfe::Status::Stopped)
	{
		theEngine->enableMouse(true);
		theEngine->dialog->active = true;
		started = true;
		m_video.visible = false;
		m_video.active = false;
	}

	if (m_noteButton.isClicked())
	{
		theEngine->dialog->startDialog("L1_NoteOpen");
	}
}

void GS_Level_1::exit()
{

	m_noteButton.setEnabled(tButton::bState::OFF);
}
