#include "Dialog.h"

#include "resourceManager.h"
#include "engine.h"
#include "Player.h"
#include "World.h"

Dialog::Dialog()
{
}


Dialog::~Dialog()
{
}

void Dialog::addEntry(_IN_STR trigger, _IN_STR content, _IN_STR speaker, sf::Color colour, EndAction end, _IN_STR endTrig, sf::FloatRect buttonRect, _IN_STR buttonLvl, _IN_STR inventoryItem, tSprite* gfx, _IN_STR requires, int duration, _IN_STR sfx)
{
	m_entries[trigger] = { trigger, content, speaker, colour, end, endTrig, inventoryItem, gfx, requires, duration, sfx};
	if (buttonRect != sf::FloatRect(0, 0, -1, -1))
	{
		m_triggerButtons[trigger].first = buttonLvl;
		m_triggerButtons[trigger].second = tButton(buttonRect);
		m_triggerButtons[trigger].second.m_UID = trigger + "_btn";
		m_triggerButtons[trigger].second.setCursorType("EYE");
	}
	if (gfx)
	{
		gfx->Zlevel = -50;
	}
}

void Dialog::initGraphics(_IN_STR bgImage, _IN_STR buttonOn, _IN_STR buttonOff, _IN_VEC2 bgPos, _IN_VEC2 buttonPos)
{
	m_dialogBox = *resMan->addSprite("dialogBG", bgImage);
	m_dialogBox.sprite.setPosition(bgPos);
	m_dialogBox.Zlevel = -100;
	m_continueButton = tButton(*resMan->getTexture(buttonOn), *resMan->getTexture(buttonOff), buttonPos);
	m_continueButton.Zlevel = -110;
	m_continueButton.m_UID = "dialogContinueBtn";
	theEngine->addButton(&m_continueButton);

	m_text.text.setPosition(m_dialogBox.sprite.getPosition());
	m_text.text.move({ m_border,m_border });
	m_text.Zlevel = -105;
	m_text.m_UID = "dialogText";
	m_text.text.setCharacterSize(80);

	m_nameText.text.setPosition(m_dialogBox.sprite.getPosition());
	m_nameText.text.move({ m_border, -10 });
	m_nameText.Zlevel = -106;
	m_nameText.m_UID = "dialogTitleText";
	m_nameText.text.setCharacterSize(90);

	theEngine->addtoRenderList(&m_dialogBox);
	theEngine->addtoRenderList(&m_continueButton);
	theEngine->addtoRenderList(&m_text);
	theEngine->addtoRenderList(&m_nameText);

	m_blackout = resMan->addSprite("Dialog_greyout", "GUI/greyout.png", { 0,0 });
	m_blackout->sprite.setScale({ 100,100 });
	m_blackout->Zlevel = -49;
	theEngine->addtoRenderList(m_blackout);
}

void Dialog::setBGImage(tSprite & newImage)
{
	theEngine->removeFromRenderList(&m_dialogBox);
	m_dialogBox = newImage;
	m_dialogBox.m_UID = "dialogBG";
	m_dialogBox.Zlevel = -100;
	theEngine->addtoRenderList(&m_dialogBox);
}

void Dialog::startDialog(_IN_STR trigger)
{
	if (!m_enabled)
		return;

	if (m_currentEntry)
		return;

	if (m_entries.count(trigger) && m_entries[trigger].active)
	{
		theEngine->inhibitPlayerMovement(true);

		m_currentEntry = &m_entries[trigger];
		m_cursorPos = 0;
		m_lineCount = 0;

		if (m_currentEntry->m_duration == 0)
		{
			m_continueButton.visible = true;
			m_continueButton.active = true;
			m_continueButton.setEnabled(tButton::bState::ON);
		}

		if (m_currentEntry->m_sfx.size() > 0u)
			resMan->getSound(m_currentEntry->m_sfx)->play();
		

		if (m_currentEntry->content.size() > 0u)
		{
			m_text.text.setString("");
			m_text.text.setFillColor(m_currentEntry->colour);
			m_text.text.setOutlineColor({ 0,0,0,80 });
			m_text.text.setOutlineThickness(5);
			m_displayString = "";

			m_nameText.text.setString(m_currentEntry->speaker);
			m_nameText.text.setFillColor(m_currentEntry->colour);
			auto col = m_currentEntry->colour;
			col.a = 80;
			m_nameText.text.setOutlineColor(col);
			m_nameText.text.setOutlineThickness(5);

			m_dialogBox.visible = true;
			
			m_text.visible = true;
			m_nameText.visible = true;
		}
		else
		{
			m_nameText.visible = false;
			m_text.visible = false;
			m_dialogBox.visible = false;
		}
		
		m_elapsed = 0;
		m_paused = false;

		if (m_currentEntry->inventoryItem != "")
			theEngine->getPlayer()->addInventory(m_currentEntry->inventoryItem);

		if (m_currentEntry->m_gfx)
		{
			auto gfx = m_currentEntry->m_gfx;
			theEngine->removeFromRenderList(gfx);
			theEngine->addtoRenderList(gfx);
			gfx->visible = true;
			gfx->active = true;
			gfx->sprite.setColor({ 255,255,255,0 });
			tEffector::activity a; a.colour = true;
			tEffector::transforms t; t.colour = { 255,255,255,255 };
			tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN);
			gfx->addEffector(fade);
			m_blackout->clearEffectors();
			m_blackout->addEffector(fade);
			m_blackout->active = true;
			m_blackout->visible = true;
		}
	}
}

void Dialog::rewindDialog(_IN_STR trigger)
{
	if (m_entries.count(trigger))
		m_entries[trigger].active = true;
}

void Dialog::update(sf::Time& dt)
{
	for (auto& b : m_triggerButtons)
		if (b.second.first == theEngine->getCurrentWorld()->getCurrentLevel() && b.second.second.isClicked())
		{
			startDialog(b.first);
			break;
		}	

	if (!m_currentEntry)
		return;

	m_elapsed += dt.asMilliseconds();
	if (!m_paused && m_cursorPos < m_currentEntry->content.size())
	{
		if(!m_currentEntry->m_duration)
			m_continueButton.setEnabled(tButton::bState::OFF);
		while (m_elapsed > m_letterTime && m_cursorPos < m_currentEntry->content.size())
		{
			m_elapsed -= m_letterTime;
			m_displayString.append({ m_currentEntry->content[m_cursorPos] });
			if (m_displayString.size() && m_lineCount == 0)
				m_lineCount = 1;

			if (m_currentEntry->content[m_cursorPos] == '\n')
			{
				m_paused = true;
				if (!m_continueButton.isEnabled())
					m_continueButton.setEnabled(tButton::bState::ON);

				m_lineCount++;
				if (m_lineCount > m_maxLines)
				{
					//erase the first line
					int lPos = m_displayString.find('\n');
					m_displayString.erase(0, lPos + 1);
					m_lineCount--;
				}
				break;
			}
				
			m_text.text.setString(m_displayString);
			//add a newline if the text exceeds the length of the box
			if (m_text.text.findCharacterPos(m_cursorPos).x > (m_dialogBox.sprite.getPosition().x + m_dialogBox.sprite.getLocalBounds().width) - m_border * 2)
			{
				//find the most recent space and replace it with "\n"
				int spPos = m_displayString.rfind(" ");
				m_displayString.erase(spPos, 1);
				m_displayString.insert(spPos, "\n");
				m_lineCount++;
				if (m_lineCount > m_maxLines)
				{
					//erase the first line
					int lPos = m_displayString.find('\n');
					m_displayString.erase(0, lPos+1);
					m_lineCount--;
				}
				m_text.text.setString(m_displayString);
			}

			m_cursorPos++;
			if (m_cursorPos >= m_currentEntry->content.size() && m_currentEntry->m_duration > 0)
			{
				m_durationClock.restart();
			}
				
		}
	}
	else if (m_paused)
	{
		if (!m_continueButton.isEnabled())
			m_continueButton.setEnabled(tButton::bState::ON);

		if (m_continueButton.isClicked())
		{
			m_paused = false;
			m_continueButton.setEnabled(tButton::bState::OFF);
			m_elapsed = 0;
			m_cursorPos++;
		}	
	}
	else
	{
		if(!m_continueButton.isEnabled() && m_currentEntry->m_duration == 0)
			m_continueButton.setEnabled(tButton::bState::ON);

		if (m_continueButton.isClicked() || (m_currentEntry->m_duration > 0 && m_durationClock.getElapsedTime().asMilliseconds() > m_currentEntry->m_duration))
		{
			switch (m_currentEntry->endAct)
			{
			case REWIND:
				break;
			case DEACTIVATE:
				m_currentEntry->active = false;
				break;
			default:
				break;
			}
			m_currentEntry->readOnce = true;

			if (m_currentEntry->m_gfx)
			{
				auto gfx = m_currentEntry->m_gfx;
				gfx->visible = true;
				gfx->active = true;
				gfx->sprite.setColor({ 255,255,255,255 });
				tEffector::activity a; a.colour = true;
				tEffector::transforms t; t.colour = { 255,255,255,0 };
				tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN);
				gfx->addEffector(fade);
				m_blackout->addEffector(fade);
			}

			auto nextTrig = m_currentEntry->endTrigger;

			//reset
			m_currentEntry = nullptr;
			m_cursorPos = 0;
			m_lineCount = 0;
			m_text.text.setString("");
			m_nameText.text.setString("");
			m_displayString = "";

			m_dialogBox.visible = false;
			m_continueButton.visible = false;
			m_continueButton.active = false;
			m_text.visible = false;
			m_nameText.visible = false;
			m_elapsed = 0;

			if (nextTrig.size())
				startDialog(nextTrig);
			else
				theEngine->inhibitPlayerMovement(false);
		}
	}

	

}

void Dialog::setFont(sf::Font& f)
{
	m_text.text.setFont(f);
	m_nameText.text.setFont(f);
}

void Dialog::setTrigBtnState(_IN_STR trigger, tButton::bState state)
{
	if (m_triggerButtons.count(trigger))
	{
		if(state == tButton::bState::ON)
			theEngine->addButton(&m_triggerButtons[trigger].second);
		else
			theEngine->removeButton(&m_triggerButtons[trigger].second);
		m_triggerButtons[trigger].second.setEnabled(state);
	}
		
}

void Dialog::checkRequirements(_IN_STR item)
{
	for (auto& e : m_entries)
	{
		if (e.second.m_requires == item)
			e.second.active = true;
	}
}
