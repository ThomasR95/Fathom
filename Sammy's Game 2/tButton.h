#pragma once
#include "tSprite.h"
class tButton : public tSprite
{
public:
	tButton();
	tButton(sf::Texture& tex, sf::Texture& offTex, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT);
	tButton(sf::FloatRect clickBounds, sf::Texture& tex, sf::Texture& offTex, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT);
	tButton(sf::FloatRect clickBounds);
	~tButton();

	enum bState {
		OFF,
		ON,
		INVISIBLE
	};

	void reset() { m_clicked = false; m_mouseOver = false; }
	bool check(sf::Vector2f mousePos);
	bool isClicked();
	bool checkMouseOver(sf::Vector2f mousePos) override;

	void setEnabled(bState state);
	bool isEnabled() { return m_state == bState::ON; }

private:

	bState m_state;

	const sf::Texture* m_onTexture = nullptr;
	const sf::Texture* m_offTexture = nullptr;

	sf::FloatRect m_clickBounds;

	bool m_clicked = false;
	bool m_mouseOver = false;
};

