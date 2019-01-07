#include "tCursor.h"
#include "resourceManager.h"
#include "engine.h"

tCursor::tCursor()
{
}


tCursor::~tCursor()
{
}

void tCursor::tick(sf::Time & dt)
{
	auto mPos = theEngine->getMousePos();
	m_eyeIcon->sprite.setPosition(mPos);
	m_keyIcon->sprite.setPosition(mPos);
}

void tCursor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	switch (m_type)
	{
	case tCursor::NORMAL:
		break;
	case tCursor::EYE:
		m_eyeIcon->draw(target, states);
		break;
	case tCursor::KEY:
		m_keyIcon->draw(target, states);
		break;
	default:
		break;
	}
}

void tCursor::init(_IN_STR eyeIcon, _IN_STR keyIcon)
{
	Zlevel = -1000;
	visible = true;
	active = true;
	m_eyeIcon = resMan->addSprite("cursor_eye", eyeIcon);
	m_eyeIcon->setOrigin(tSprite::origin::BOTTOM_RIGHT);
	m_keyIcon = resMan->addSprite("cursor_key", keyIcon);
	m_keyIcon->setOrigin(tSprite::origin::BOTTOM_RIGHT);
	theEngine->addtoRenderList(this);

	m_type = NORMAL;
}

void tCursor::setType(_IN_STR type)
{
	if (type == "EYE")
		m_type = EYE;
	else if (type == "KEY")
		m_type = KEY;
}
