#include "tText.h"



tText::tText()
{
}

tText::tText(const sf::String & string, const sf::Font & font, unsigned int characterSize)
{
	text = sf::Text(string, font, characterSize);
}


tText::~tText()
{
}

void tText::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(text, states);
}
