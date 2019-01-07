#pragma once
#include "sfeMovie\Movie.hpp"
#include "GfxBase.h"
class tMovie : public GfxBase
{
public:
	tMovie();
	~tMovie();

	sfe::Movie video;

	virtual void tick(sf::Time& dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

