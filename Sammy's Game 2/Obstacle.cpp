#include "Obstacle.h"
#include "engine.h"


Obstacle::Obstacle(sf::Texture * tex, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, tSprite::origin org)
	: Anim(*tex, pos, 1, 1, 1, src, size, org)
{
	m_texSize = sf::Vector2f(sprite.getTexture()->getSize());
	m_navSize = { 0,0 };
	playerAlwaysBehind = true;
}

Obstacle::Obstacle(sf::Texture* tex, sf::Texture* navMask, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, tSprite::origin org)
	: Anim(*tex, pos, 1, 1, 1, src, size, org)
{

	m_texSize = sf::Vector2f(sprite.getTexture()->getSize());
	if (navMask)
	{
		m_navMask = navMask->copyToImage();
		m_navSize = sf::Vector2f(m_navMask.getSize());
		m_navScale = { m_navSize.x / m_texSize.x, m_navSize.y / m_texSize.y };
	}
	else
		m_navSize = { 0,0 };
}

Obstacle::Obstacle(sf::Texture * tex, int frames, int rows, int cols, sf::Texture * navMask, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, tSprite::origin org)
	: Anim(*tex, pos, frames, rows, cols, src, size, org)
{
	m_texSize = getSize();
	if (navMask)
	{
		m_navMask = navMask->copyToImage();
		m_navSize = sf::Vector2f(m_navMask.getSize());
		m_navScale = { m_navSize.x / m_texSize.x, m_navSize.y / m_texSize.y };
	}
	else
		m_navSize = { 0,0 };
}


Obstacle::~Obstacle()
{
}

bool Obstacle::isPlayerBehind(_IN_VEC2 playerFootPos)
{
	if (!active)
		return false;

	if (playerAlwaysInFront)
		return false;

	if (playerAlwaysBehind)
		return true;

	sf::Vector2f relativePos = playerFootPos - (sprite.getPosition() - sprite.getOrigin());
	relativePos.x *= m_navScale.x;
	relativePos.y *= m_navScale.y;
	
	if (relativePos.y < 0 || relativePos.x < 0 || relativePos.x > m_navSize.x)
		return false;

	for (int y = (int)relativePos.y; y < (int)ceil(m_navSize.y); y++)
	{
		auto col = m_navMask.getPixel((int)relativePos.x, y);
		//can be blue
		if (col.r < 128 && col.g < 128 && col.a > 128)
			return true;
	}

	return false;
}

bool Obstacle::isPlayerBehind(_IN_VEC2 playerFootPos, float width)
{
	if (!active)
		return false;

	if (playerAlwaysInFront)
		return false;

	if (playerAlwaysBehind)
		return true;

	bool behind = false;
	if (isPlayerBehind({ playerFootPos.x + (width / 2.f), playerFootPos.y }))
		behind = true;
	if (isPlayerBehind({ playerFootPos.x - (width / 2.f), playerFootPos.y }))
		behind = true;

	return behind;
}

Obstacle::SIDE Obstacle::testCollision(_IN_VEC2 pos)
{
	if (!collisionEnabled || !active)
		return SIDE();

	sf::Vector2f relativePos = pos - (sprite.getPosition() - sprite.getOrigin());
	relativePos.x *= m_navScale.x;
	relativePos.y *= m_navScale.y;

	sf::Vector2u relPos(relativePos);

	SIDE side;

	if (relPos.y < 0 || relPos.x < 0 || relPos.x > m_navSize.x || relPos.y > m_navSize.y)
		return side;

	sf::Color col;

	if (relPos.y < m_navSize.y-1)
	{
		col = m_navMask.getPixel(relPos.x, relPos.y + 1);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
			side.top = true;
	}
	
	if (relPos.y > 0)
	{
		col = m_navMask.getPixel(relPos.x, relPos.y - 1);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
			side.bottom = true;
	}
	
	if (relativePos.x < m_navSize.x-1)
	{
		col = m_navMask.getPixel(relPos.x + 1, relPos.y);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
			side.left = true;
	}

	if (relativePos.x > 0)
	{
		col = m_navMask.getPixel(relPos.x - 1, relPos.y);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
			side.right = true;
	}

	return side;
}

bool Obstacle::testCollision(sf::Vector2f & pos, sf::Vector2f & vel, GfxBase* actor)
{
	if (!collisionEnabled || !active)
		return false;

	if (m_navSize == sf::Vector2f(0, 0))
		return false;

	sf::Vector2f relativePos = pos - (sprite.getPosition() - sprite.getOrigin());
	relativePos.x *= m_navScale.x;
	relativePos.y *= m_navScale.y;

	auto nextPos = relativePos + vel;
	//test if outside bounds
	if (nextPos.y < 0 ||
		nextPos.x < 0 ||
		nextPos.x > m_navSize.x ||
		nextPos.y > m_navSize.y)
		return false;

	bool collision = false;

	sf::Vector2i relPos(relativePos);
	sf::Color col;

	if (nextPos.y > 0 && nextPos.y < m_navSize.y && vel.y != 0)
	{
		col = m_navMask.getPixel(nextPos.x, nextPos.y);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
		{
			collision = true;
			vel.y = reduceVelocity(relativePos, vel).y;
		}
	}

	nextPos = relativePos + vel;
	//test if outside bounds
	if (nextPos.y < 0 ||
		nextPos.x < 0 ||
		nextPos.x > m_navSize.x ||
		nextPos.y > m_navSize.y)
		return false;

	if (nextPos.x > 0 && nextPos.x < m_navSize.x  && vel.x != 0)
	{
		col = m_navMask.getPixel(nextPos.x, nextPos.y);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
		{
			collision = true;
			vel.x = reduceVelocity(relativePos, vel).x;
		}
	}

	return collision;
}

bool Obstacle::checkInteraction(sf::Vector2f mousePos)
{
	if (!m_clickable || !visible)
		return false;

	auto bounds = sprite.getGlobalBounds();
	bool clicked = false;
	if (bounds.contains(mousePos))
	{
		if (!m_clickTestAlpha)
			clicked = true;
		else
		{
			sf::Vector2f relativePos = mousePos - (sprite.getPosition() - sprite.getOrigin());
			auto tex = sprite.getTexture();
			if (tex->copyToImage().getPixel(relativePos.x, relativePos.y).a > 50)
				clicked = true;
		}
	}

	if (clicked && m_dialogTrigger.size() > 0u)
	{
		theEngine->dialog->startDialog(m_dialogTrigger);
	}
	if(clicked && m_interactFunction)
			m_interactFunction();

	return clicked;
}

sf::Vector2f Obstacle::reduceVelocity(_IN_VEC2 pos, _IN_VEC2 vel)
{
	sf::Vector2f out = vel;

	bool collide = true;

	sf::Color col;
	while (collide)
	{
		auto nextPos = pos + out;
		if (nextPos.y < 0 ||
			nextPos.x < 0 ||
			nextPos.x > m_navSize.x ||
			nextPos.y > m_navSize.y)
		{
			collide = false;
			break;
		}
			

		col = m_navMask.getPixel(nextPos.x, nextPos.y);
		if (col.r < 128u && col.g < 128u && col.b < 128u && col.a > 10u)
		{
			out.x -= out.x * 0.5f;
			out.y -= out.y * 0.5f;

			if (abs(out.x) < 1.f)
				out.x = 0;
			if (abs(out.y) < 1.f)
				out.y = 0;

			if (out.x == 0 && out.y == 0)
				break;

		}
		else
			collide = false;
	}

	return out;
}
