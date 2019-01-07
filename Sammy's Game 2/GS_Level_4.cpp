#include "typedefs.h"
#include "GS_Level_4.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_4::GS_Level_4()
{
}


GS_Level_4::~GS_Level_4()
{
}

void GS_Level_4::enter()
{
	theEngine->setNextState("GS_EXIT");

	if (m_first)
	{
		m_hutDoor = tButton({ 2892, 1674, 415, 801 });
		m_hutDoor.setCursorType("KEY");
		m_hutDoor.m_UID = "hutDoorBtn";
		theEngine->addButton(&m_hutDoor);
		
		m_combiLockGUI = resMan->addSprite("L4_CombiLockMain", "GUI/lock.png");
		m_combiLockGUI->visible = false;
		m_combiLockGUI->Zlevel = -100;
		m_combiLockGUI->sprite.setPosition(1298, 657);
		theEngine->addtoRenderList(m_combiLockGUI);

		std::vector<sf::Vector2f> reelPosns = {
			{1845, 945},
			{ 2076, 945 },
			{ 2308, 945 },
			{ 2544, 945 },
		};

		m_lockReels.resize(4);
		int i = 0;
		for (auto& l : m_lockReels)
		{
			l.init({
				"GUI/no 0.png",
				"GUI/no 1.png",
				"GUI/no 2.png",
				"GUI/no 3.png",
				"GUI/no 4.png",
				"GUI/no 5.png",
				"GUI/no 6.png",
				"GUI/no 7.png",
				"GUI/no 8.png",
				"GUI/no 9.png",
			}, reelPosns[i]);

			l.m_UID = "lockReel_" + std::to_string(i);
			l.Zlevel = -90;
			theEngine->addtoRenderList(&l);
			i++;
		}
		m_first = false;
	}
	
	if (theEngine->getPlayer()->checkInventory("CalendarPage") && !m_combiLockDone)
	{
		m_hutDoor.setEnabled(tButton::bState::ON);
		m_hutDoor.setCursorType("KEY");
	}
	else
		m_hutDoor.setCursorType("");
}

void GS_Level_4::run()
{
	if (m_hutDoor.isClicked() && !m_combiLockDone)
	{
		m_showingCombiLock = true;
		m_combiLockGUI->visible = true;
		for (auto& l : m_lockReels)
		{
			l.active = true;
			l.visible = true;
		}
		tEffector::activity a; a.colour = true;
		tEffector::transforms t; t.colour = { 255,255,255,255 };
		tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN);

		auto blackout = resMan->getSprite("Dialog_greyout");
		blackout->clearEffectors();
		blackout->addEffector(fade);
		blackout->active = true;
		blackout->visible = true;

		theEngine->dialog->setEnabled(false);
		theEngine->inhibitPlayerMovement(true);
			
	}

	if (m_showingCombiLock)
	{
		if (m_lockReels[0].getNum() == 9 &&
			m_lockReels[1].getNum() == 3 &&
			m_lockReels[2].getNum() == 4 &&
			m_lockReels[3].getNum() == 1 && !m_combiLockDone)
		{
			for (auto& l : m_lockReels)
			{
				l.disableButtons();
			}

			m_combiLockDone = true;
			m_lockWaitTimer.restart();

			tEffector::activity a; a.colour = true;
			tEffector::transforms t; t.colour = { 255,255,255,0 };
			tEffector fade(t, a, 500, tInterpolator::QUADRATIC_UP_DOWN);

			auto blackout = resMan->getSprite("Dialog_greyout");
			blackout->clearEffectors();
			blackout->addEffector(fade);
			blackout->active = true;
			blackout->visible = true;

			resMan->getSound("../sounds/Triggered/activate door.ogg")->play();
		}

		if (m_combiLockDone && m_lockWaitTimer.getElapsedTime().asMilliseconds() > 1000)
		{
			m_combiLockGUI->visible = false;
			for (auto& l : m_lockReels)
			{
				l.active = false;
				l.visible = false;
				l.disableButtons();
			}
			theEngine->getPlayer()->addInventory("CombiLockOpened");

			m_showingCombiLock = false;
			m_hutDoor.setEnabled(tButton::bState::OFF);
			theEngine->dialog->setEnabled(true);
			theEngine->inhibitPlayerMovement(false);
			m_hutDoor.setCursorType("");
		}
	}
}

void GS_Level_4::exit()
{
}

void lockReel::tick(sf::Time & dt)
{
	if (active && !m_upButton.isEnabled())
	{
		m_upButton.setEnabled(tButton::bState::ON);
		m_downButton.setEnabled(tButton::bState::ON);
	}

	if (m_upButton.isClicked() && !m_numbers[0].getNumEffectors())
		up();

	if (m_downButton.isClicked() && !m_numbers[0].getNumEffectors())
		down();


	for (auto& n : m_numbers)
		n.tick(dt);
}

void lockReel::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto& n : m_numbers)
		n.draw(target, states);
}

void lockReel::init(const std::vector<std::string>& numGfx, _IN_VEC2 centrePosn)
{
	m_texNames = numGfx;
		
	m_numSize = sf::Vector2f(resMan->getTexture(m_texNames[0])->getSize());
	m_currentPosition = 0;

	for (int p = 0; p < 5; p++)
	{
		auto pos = centrePosn;
		pos.y += (p - 2)*m_numSize.y;
		m_positions.push_back(pos);
	}

	int tex = 9;
	for (int i = 1; i < 5; i++)
	{
		m_numbers.push_back(tSprite(*resMan->getTexture(m_texNames[tex]), m_positions[i]));
		m_numbers.back().m_UID = "lock" + std::to_string(centrePosn.x) + "Num" + std::to_string(i);
		tex++;
		if (tex >= m_texNames.size())
			tex = 0;
	}

	m_upButton = tButton({ m_positions[3].x, m_positions[3].y, m_numSize.x, m_numSize.y });
	m_downButton = tButton({ m_positions[1].x, m_positions[1].y, m_numSize.x, m_numSize.y });

	theEngine->addButton(&m_upButton);
	theEngine->addButton(&m_downButton);
}

void lockReel::disableButtons()
{
	m_upButton.setEnabled(tButton::bState::OFF);
	m_downButton.setEnabled(tButton::bState::OFF);
}

void lockReel::up()
{
	//move sprites to positions 1-4
	int pos = 1;

	//reassign the textures to match
	int tex = m_currentPosition - 1;
	if (tex < 0)
		tex += m_texNames.size();

	int i = 0;
	for (auto& n : m_numbers)
	{
		n.sprite.setPosition(m_positions[pos]);
		n.sprite.setTexture(*resMan->getTexture(m_texNames[tex]));
		n.sprite.setColor({ 255,255,255,255 });

		tEffector::transforms moveUpT; moveUpT.position = m_positions[pos - 1];
		tEffector::activity moveUpA; moveUpA.position = true;
		if (i == 0)
		{
			moveUpT.colour = { 255,255,255,0 };
			moveUpA.colour = true;
		}
		tEffector moveUp(moveUpT, moveUpA, 200, tInterpolator::QUADRATIC_UP_DOWN);

		n.addEffector(moveUp);

		tex++;
		if (tex >= m_texNames.size())
			tex -= m_texNames.size();
		pos++;
		i++;
	}
	m_currentPosition++;
	if(m_currentPosition >= m_texNames.size())
		m_currentPosition -= m_texNames.size();
}

void lockReel::down()
{
	//move sprites to positions 0-3
	int pos = 0;

	//reassign the textures to match
	int tex = m_currentPosition - 2;
	if (tex < 0)
		tex += m_texNames.size();

	int i = 0;
	for (auto& n : m_numbers)
	{
		n.sprite.setPosition(m_positions[pos]);
		n.sprite.setTexture(*resMan->getTexture(m_texNames[tex]));
		n.sprite.setColor({ 255,255,255,255 });
		
		tEffector::transforms moveDownT; moveDownT.position = m_positions[pos + 1];
		tEffector::activity moveDownA; moveDownA.position = true;

		if (i == 0)
		{
			n.sprite.setColor({ 255,255,255,0 });
			moveDownT.colour = { 255,255,255,255 };
			moveDownA.colour = true;
		}
		tEffector moveDown(moveDownT, moveDownA, 200, tInterpolator::QUADRATIC_UP_DOWN);

		n.addEffector(moveDown);

		tex++;
		if (tex >= m_texNames.size())
			tex -= m_texNames.size();
		pos++;
		i++;
	}
	m_currentPosition--;
	if (m_currentPosition < 0)
		m_currentPosition += m_texNames.size();

}
