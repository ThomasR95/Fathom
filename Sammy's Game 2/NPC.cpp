#include "NPC.h"
#include "engine.h"
#include <iostream>

NPC::NPC()
{
}

NPC::NPC(sf::Texture & tex, sf::Texture* navMask, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, origin org) : tSprite(tex, pos, src, size, org)
{
	m_vel = { 0,0 };


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


NPC::~NPC()
{
}

void NPC::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_currentAnim)
		target.draw(*m_currentAnim);
	else
		target.draw(sprite);
}

void NPC::tick(sf::Time & dt)
{

	if (dt.asMilliseconds() > 300)
		return; //this is lag

	m_pos = sprite.getPosition();
	m_feetPos = m_pos;
	m_feetPos.y += m_feetOffset;


	if (m_targetQueue.size())
	{
		auto mPos = m_targetQueue.back();
		sf::Vector2f dir;
		dir = mPos - m_pos;
		float length = sqrt(pow(dir.x, 2) + pow(dir.y, 2));
		dir.x /= length;
		dir.y /= length;

		if (length > m_width / 2)
		{
			m_vel = { (m_vel.x*0.9f) + dir.x * length * 0.15f, (m_vel.y*0.9f) + dir.y * length * 0.15f };
			if (m_vel.x > m_maxSpeed)
				m_vel.x = m_maxSpeed;
			if (m_vel.x < -m_maxSpeed)
				m_vel.x = -m_maxSpeed;

			if (m_vel.y > m_maxSpeed)
				m_vel.y = m_maxSpeed;
			if (m_vel.y < -m_maxSpeed)
				m_vel.y = -m_maxSpeed;
		}
		else
		{
			m_targetQueue.pop_back();
			m_vel = { 0.f, 0.f };
		}

	}
	else
	{
		m_vel = { 0.f, 0.f };
	}


	auto velocity = m_vel * dt.asSeconds();
	m_speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	calculateDirection(velocity);

	auto right = m_feetPos;
	auto left = right;
	left.x -= m_width / 2;
	right.x += m_width / 2;
	theEngine->testCollision(left, velocity, this);
	theEngine->testCollision(m_feetPos, velocity, this);
	theEngine->testCollision(right, velocity, this);

	m_speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	m_pos += velocity;

	if (m_pos != m_lastPos)
	{
		sprite.setPosition(m_pos);
		for (auto& i : m_idleAnims)
			i->sprite.setPosition(m_pos);
		for (auto& w : m_walkAnims)
			w->sprite.setPosition(m_pos);

		m_lastPos = m_pos;
		
		theEngine->checkPlayerBehind(left, Zlevel);
		int z2 = Zlevel;
		theEngine->checkPlayerBehind(right, z2);
		Zlevel = std::max(Zlevel, z2);
		theEngine->sortRenderList();
	}
	
	for (auto& i : m_idleAnims)
		i->tick(dt);

	for (auto& w : m_walkAnims)
		w->tick(dt);

	bool flip = false;
	int dir = m_dir;
	if (dir < 0)
	{
		dir *= -1;
		flip = true;
	}

	if (m_idleAnims.size() > dir && m_speed < 0.5f)
	{
		m_currentAnim = m_idleAnims[dir].get();
		if (flip)
			m_currentAnim->sprite.setScale({ -1,1 });
		else
			m_currentAnim->sprite.setScale({ 1,1 });

	}
	else if (m_walkAnims.size() > dir)
	{
		m_currentAnim = m_walkAnims[dir].get();
		if (flip)
			m_currentAnim->sprite.setScale({ -1,1 });
		else
			m_currentAnim->sprite.setScale({ 1,1 });
	}

}

void NPC::setIdleAnims(const std::vector<AnimCfg>& anims)
{
	for (auto& anm : anims)
	{
		m_idleAnims.push_back(std::make_shared<Anim>(anm));
	}
	if (m_idleAnims.size() > 0u)
	{
		m_texSize = m_idleAnims[0]->getSize();
		m_navScale = { m_navSize.x / m_texSize.x, m_navSize.y / m_texSize.y };
	}
		
}

void NPC::setWalkAnims(const std::vector<AnimCfg>& anims)
{
	for (auto& anm : anims)
	{
		m_walkAnims.push_back(std::make_shared<Anim>(anm));
	}
}

void NPC::calculateDirection(_IN_VEC2 vel)
{
	//normalize
	auto dir = vel;
	dir.x /= m_speed;
	dir.y /= m_speed;

	//too slow?
	if (m_speed < 0.5f)
		return;

	//get directon from angle
	float angle = angleBetween(dir, { 0,-1 });
	//std::cout << angle << std::endl;
	if (abs(angle) <= 22.5) //n
	{
		m_dir = 0;
	}
	if (abs(angle) > 22.5 && abs(angle) <= 67.5) // ne / nw
	{
		m_dir = -1;
		if (angle < 0)
			m_dir = 1;
	}
	if (abs(angle) > 67.5 && abs(angle) <= 112.5) // e
	{
		m_dir = -2;
		if (angle < 0)
			m_dir = 2;
	}
	if (abs(angle) > 112.5 && abs(angle) <= 157.5) // se / sw
	{
		m_dir = -3;
		if (angle < 0)
			m_dir = 3;
	}
	if (abs(angle) > 157.5) // se / sw
	{
		m_dir = 4;
	}
}

bool NPC::isPlayerBehind(_IN_VEC2 playerFootPos)
{
	if (playerFootPos.x > m_feetPos.x - m_width / 2 &&
		playerFootPos.x < m_feetPos.x + m_width / 2 &&
		playerFootPos.y < m_feetPos.y)
		return true;

	return false;
}

bool NPC::isPlayerBehind(_IN_VEC2 playerFootPos, float width)
{
	bool behind = false;
	auto pos = playerFootPos;
	pos.x += width / 2;
	behind |= isPlayerBehind(pos);
	pos = playerFootPos;
	pos.x -= width / 2;
	behind |= isPlayerBehind(pos);

	return behind;
}

bool NPC::testCollision(sf::Vector2f & pos, sf::Vector2f & vel, GfxBase* actor)
{
	if (!collisionEnabled || !active)
		return false;

	if (actor == this)
		return false;

	if (m_navSize == sf::Vector2f(0, 0))
		return false;

	sf::Vector2f relativePos;
	if(m_currentAnim)
		relativePos = pos - (m_currentAnim->sprite.getPosition() - m_currentAnim->sprite.getOrigin());
	else
		relativePos = pos - (sprite.getPosition() - sprite.getOrigin());
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

bool NPC::checkInteraction(sf::Vector2f mousePos)
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

	return clicked;
}

sf::Vector2f NPC::reduceVelocity(_IN_VEC2 pos, _IN_VEC2 vel)
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
