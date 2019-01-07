#pragma once

#include "typedefs.h"
#include <string>
#include <map>
#include "SFML\Graphics.hpp"
#include "tSprite.h"
#include "tButton.h"
#include "tText.h"

class Dialog
{
public:
	Dialog();
	~Dialog();

	enum EndAction
	{
		REWIND,
		DEACTIVATE
	};

	struct Entry
	{
		Entry() {};
		Entry(_IN_STR tr, _IN_STR c, _IN_STR sp, sf::Color col, EndAction end = REWIND, _IN_STR endTrig = "", _IN_STR invItem = "", tSprite* gfx = nullptr, _IN_STR requires = "", int duration = 0, _IN_STR sfx = "") :
			trigger(tr),
			content(c),
			speaker(sp),
			colour(col),
			endAct(end),
			active(true),
			readOnce(false),
			endTrigger(endTrig),
			inventoryItem(invItem),
			m_gfx(gfx),
			m_requires(requires),
			m_duration(duration),
			m_sfx(sfx)
		{
			if (requires.size() > 0u) active = false;
		}

		std::string trigger;
		std::string content;
		std::string speaker;
		sf::Color colour;
		EndAction endAct;
		bool active;
		bool readOnce;
		std::string endTrigger;
		std::string inventoryItem;
		tSprite* m_gfx;
		std::string m_requires;
		int m_duration;
		std::string m_sfx;
	};

	void addEntry(_IN_STR trigger, _IN_STR content, _IN_STR speaker, sf::Color colour = { 255,255,255,255 }, EndAction end = REWIND, _IN_STR endTrig = "", sf::FloatRect buttonRect = { 0,0,-1,-1 }, _IN_STR buttonLvl = "", _IN_STR inventoryItem = "", tSprite* gfx = nullptr, _IN_STR requires = "", int duration = 0, _IN_STR sfx = "");

	void initGraphics(_IN_STR bgImage, _IN_STR buttonOn, _IN_STR buttonOff, _IN_VEC2 bgPos, _IN_VEC2 buttonPos);

	void setBGImage(tSprite& newImage);

	void startDialog(_IN_STR trigger);

	void rewindDialog(_IN_STR trigger);

	void update(sf::Time& dt);

	bool isDialogFinished() { return m_currentEntry == nullptr; }

	Entry* getEntry(_IN_STR trigger) { if (m_entries.count(trigger)) return &m_entries[trigger]; else return nullptr; }

	void setFont(sf::Font& f);

	void setEnabled(bool enabled) { m_enabled = enabled; }

	void setTrigBtnState(_IN_STR trigger, tButton::bState state);

	void checkRequirements(_IN_STR item);

	bool active = true;

private:

	std::map<std::string, Entry> m_entries;

	Entry* m_currentEntry = nullptr;
	int m_cursorPos;

	tSprite m_dialogBox;
	tButton m_continueButton;

	float m_border = 100;

	std::string m_displayString;

	int m_letterTime = 10;

	int m_lineCount = 0;
	int m_maxLines = 4;
	int m_elapsed = 0;

	bool m_paused = false;

	bool m_enabled = true;
	std::map<std::string, std::pair<std::string, tButton>> m_triggerButtons;

	tText m_text;
	tText m_nameText;
	sf::Font m_font;

	tSprite* m_blackout;

	sf::Clock m_durationClock;
};

