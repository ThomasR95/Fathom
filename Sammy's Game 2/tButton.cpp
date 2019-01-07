#include "tButton.h"
#include "engine.h"
#include "tCursor.h"


tButton::tButton()
{
}

tButton::tButton(sf::Texture & tex, sf::Texture & offTex, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, origin org)
	: tSprite(tex, pos, src, size, org)
{
	m_onTexture = &tex;
	m_offTexture = &offTex;
	m_clickBounds = sprite.getGlobalBounds();

}

tButton::tButton(sf::FloatRect clickBounds, sf::Texture & tex, sf::Texture & offTex, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, origin org)
	: tSprite(tex, pos, src, size, org)
{
	m_onTexture = &tex;
	m_offTexture = &offTex;
	m_clickBounds = clickBounds;
}

tButton::tButton(sf::FloatRect clickBounds)
{
	m_clickBounds = clickBounds;
	visible = false;
}


tButton::~tButton()
{
}

bool tButton::check(sf::Vector2f mousePos)
{
	m_clicked = false;
	m_mouseOver = false;

	if (m_state == bState::ON && m_clickBounds.contains(mousePos))
	{
		m_mouseOver = true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			m_clicked = true;
	}
	
	if (m_mouseCursor != "" && m_mouseOver)
		theEngine->getCursor()->setType(m_mouseCursor);

	return m_clicked;
}

bool tButton::isClicked()
{
	return m_clicked;
}

bool tButton::checkMouseOver(sf::Vector2f mousePos)
{
	return m_mouseOver;
}

void tButton::setEnabled(bState state)
{
	m_state = state;
	visible = true;
	switch (m_state)
	{
	case tButton::OFF:
		if(m_offTexture)
			sprite.setTexture(*m_offTexture, true);
		break;
	case tButton::ON:
		if(m_onTexture)
			sprite.setTexture(*m_onTexture, true);
		break;
	case tButton::INVISIBLE:
		visible = false;
		break;
	default:
		break;
	}

	m_clicked = false;
	m_mouseOver = false;
}
