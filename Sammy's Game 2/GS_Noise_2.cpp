#include "GS_Noise_2.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Noise_2::GS_Noise_2()
{
}


GS_Noise_2::~GS_Noise_2()
{
}

void GS_Noise_2::enter()
{
	if (first)
	{
		first = false;
		for (int r = 1; r < 9; r++)
		{
			m_runes.push_back((Obstacle*)theEngine->getCurrentWorld()->getAsset("rune_" + std::to_string(r)));
			m_hints.push_back((Anim*)theEngine->getCurrentWorld()->getAsset("hint_" + std::to_string(r)));
		}
	}

	for (auto& h : m_hints)
	{
		h->setFrame(0);
		h->setPauseFrame(7, true);
		h->active = false;
		h->visible = false;
	}
	for (auto& r : m_runes)
	{
		r->setInteraction("", true, false);
	}
	theEngine->setNextState("GS_EXIT");
}

void GS_Noise_2::run()
{
	for (int r = 0; r < 8; r++)
	{
		if (m_runes[r]->checkInteraction(GameObjects::thePlayer.getFeetPosition()))
		{
			m_hints[r]->visible = true;
			m_hints[r]->active = true;
		}
	}
}

void GS_Noise_2::exit()
{
}
