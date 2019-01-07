#include "tMovie.h"



tMovie::tMovie()
{
	active = true;
}


tMovie::~tMovie()
{
}

void tMovie::tick(sf::Time & dt)
{
	video.update();
}

void tMovie::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(video);
}
