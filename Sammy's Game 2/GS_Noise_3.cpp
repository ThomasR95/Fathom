#include "GS_Noise_3.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Noise_3::GS_Noise_3()
{
}


GS_Noise_3::~GS_Noise_3()
{
}

void GS_Noise_3::enter()
{
	theEngine->setNextState("GS_EXIT");
	if (first)
	{
		first = false;
		m_teeth = ((Anim*)theEngine->getCurrentWorld()->getAsset("teeth"));
		m_teeth->setFrame(0);
		m_teeth->setPauseFrame(1, true);
		m_teeth->setLoop(false);
		m_eyes = ((Anim*)theEngine->getCurrentWorld()->getAsset("eyes"));
		m_eyes->active = false;
		m_eyes->setLoop(false);
		m_eyes->setFrame(m_eyes->getNofFrames()-1);
		m_eyeClock.restart();
		m_blinkTime = randInt(1000, 2001);

		for (int r = 1; r < 9; r++)
		{
			m_hints.push_back((Anim*)theEngine->getCurrentWorld()->getAsset("hint_" + std::to_string(r)));
		}

		for (int b = 0; b < 4; b++)
		{
			auto btn = (Obstacle*)theEngine->getCurrentWorld()->getAsset("button_"+std::to_string(b));
			btn->setInteraction([&, b, btn] 
			{
				m_phoneCode += std::to_string(b+1);
				std::cout << m_phoneCode << std::endl;

				btn->sprite.setColor({ 100,100,100,255 });
				tEffector::activity a; a.colour = true;
				tEffector::transforms t;
				t.colour = { 255,255,255,255 };
				btn->clearEffectors();
				btn->addEffector(tEffector(t, a, 200, tInterpolator::type::LINEAR, tEffector::POINT_TO_POINT));

				resMan->getSound("../sounds/Triggered/phone buttons.ogg")->play();

			}, true);
			m_buttons.push_back(btn);
		}
	}

	for (auto& h : m_hints)
	{
		h->setFrame(0);
		h->setPauseFrame(7, true);
		h->active = false;
		h->visible = false;
	}
	m_phoneCode = "";
}

void GS_Noise_3::run()
{
	if (m_eyeClock.getElapsedTime().asMilliseconds() > m_blinkTime)
	{
		m_eyes->restart();
		m_eyes->active = true;
		m_eyeClock.restart();
		m_blinkTime = randInt(1000, 2001);
	}

	if (!m_phoneUnlocked)
	{
		if (m_phoneCode == m_solution)
		{
			for (auto& b : m_buttons)
			{
				b->setInteraction((std::function<void()>)nullptr, false, false);
			}
			GameObjects::thePlayer.addInventory("phoneUnlocked");
			m_teeth->active = true;
			m_teeth->setPauseFrame(0, false);
			m_teeth->restart();
			m_teeth->setCollisionEnabled(false);
			m_phoneUnlocked = true;
			for (auto& h : m_hints)
			{
				h->visible = true;
				h->active = true;
				h->setPauseFrame(0, false);
				h->restart();
			}
		}
		else
		{
			if (m_phoneCode == m_solution.substr(0, m_phoneCode.size()))
			{
				for (int h = 0; h < m_phoneCode.size(); h++)
					if (!m_hints[h]->visible)
					{
						m_hints[h]->visible = true;
						m_hints[h]->active = true;
						m_hints[h]->restart();
					}
			}
			else
			{
				m_phoneCode = "";
				for (auto& h : m_hints)
				{
					h->visible = false;
					h->active = false;
					h->restart();
				}
			}
		}

		if (m_phoneCode.size() > 8)
			m_phoneCode = "";
	}
	

}

void GS_Noise_3::exit()
{
}
