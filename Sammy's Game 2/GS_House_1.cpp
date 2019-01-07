#include "GS_House_1.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_House_1::GS_House_1()
{
	if (!m_video.video.openFromFile("../images/Cutscenes/Flashback Cutscene(v2)_cropped.mkv"))
		__debugbreak();
}


GS_House_1::~GS_House_1()
{
}

void GS_House_1::enter()
{
	theEngine->setNextState("GS_EXIT");
}

void GS_House_1::run()
{
	if (!m_moviePlayed && theEngine->dialog->isDialogFinished() && theEngine->dialog->getEntry("H1_Note4")->readOnce)
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

	if (m_moviePlayed && m_video.video.getStatus() == sfe::Status::Stopped)
	{
		theEngine->enableMouse(true);
		theEngine->dialog->active = true;
		m_video.visible = false;
		m_video.active = false;
	}
}

void GS_House_1::exit()
{
}
