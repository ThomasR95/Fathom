#pragma once

#include "GfxBase.h"
#include "SFML\Graphics.hpp"

class tText : public GfxBase
{
public:
	tText();
	tText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30);
	~tText();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	sf::Text text;
};

