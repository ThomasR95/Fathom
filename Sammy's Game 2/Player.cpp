#include "Player.h"
#include "engine.h"
#include <iostream>

Player::Player()
{
}

Player::Player(sf::Texture & tex, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 size, origin org) : tSprite(tex, pos, src, size, org)
{
	m_vel = { 0,0 };
}


Player::~Player()
{
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	bool flip = false;
	int dir = m_dir;
	if (dir < 0)
	{
		dir *= -1;
		flip = true;
	}

	if (m_idleAnims.size() > dir && m_speed < 0.5f)
	{
		if (flip)
			m_idleAnims[dir]->sprite.setScale({ -1,1 });
		else
			m_idleAnims[dir]->sprite.setScale({ 1,1 });
		target.draw(*m_idleAnims[dir]);
	}
	else if(m_walkAnims.size() > dir)
	{
		if (flip)
			m_walkAnims[dir]->sprite.setScale({ -1,1 });
		else
			m_walkAnims[dir]->sprite.setScale({ 1,1 });
		target.draw(*m_walkAnims[dir]);
	}
	else
		target.draw(sprite);
}

void Player::tick(sf::Time & dt)
{

	if (dt.asMilliseconds() > 300)
		return; //this is lag

	m_pos = sprite.getPosition();
	m_feetPos = m_pos;
	m_feetPos.y += m_feetOffset;

	if (m_followMouse || m_running)
	{
		if ((m_running || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && theEngine->getWindow()->hasFocus())
		{
			//std::cout << "player mouse input" << std::endl;
			m_isMouseLed = true;
			if (!m_mouseHidden)
			{
				theEngine->getWindow()->setMouseCursorGrabbed(true);
				theEngine->getWindow()->setMouseCursorVisible(false);
				m_mouseHidden = true;
			}

			auto mPos = theEngine->getMousePos();
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
				m_isMouseLed = false;
				m_vel = { 0.f, 0.f };
			}

		}
		else
		{
			m_isMouseLed = false;
			if (m_mouseHidden)
			{
				auto wPos = theEngine->getWindow()->getPosition();
				auto pos = theEngine->worldToScreenCoords(m_pos);
				sf::Mouse::setPosition(wPos + sf::Vector2i(pos));
				theEngine->getWindow()->setMouseCursorGrabbed(false);
				theEngine->getWindow()->setMouseCursorVisible(true);
				m_mouseHidden = false;
			}
			m_vel = { 0.f, 0.f };
		}

	}
	else
	{
		m_isMouseLed = false;
		if (m_mouseHidden)
		{
			auto wPos = theEngine->getWindow()->getPosition();
			auto pos = theEngine->worldToScreenCoords(m_pos);
			sf::Mouse::setPosition(wPos + sf::Vector2i(pos));
			theEngine->getWindow()->setMouseCursorGrabbed(false);
			theEngine->getWindow()->setMouseCursorVisible(true);
			m_mouseHidden = false;
		}
		m_vel = { 0,0 };
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
		auto spawnPos = theEngine->checkWorldGateways(m_pos);
		if (spawnPos != sf::Vector2f(-1, -1))
		{
			m_pos = spawnPos;
			
			if (m_mouseHidden)
			{
				auto wPos = theEngine->getWindow()->getPosition();
				auto pos = theEngine->worldToScreenCoords(m_pos);
				sf::Mouse::setPosition(wPos + sf::Vector2i(pos + (m_vel/2.f)));
			}

			m_vel = { 0,0 };
			
		}

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

}

void Player::setIdleAnims(const std::vector<AnimCfg>& anims)
{
	for (auto& anm : anims)
	{
		m_idleAnims.push_back(std::make_shared<Anim>(anm));
	}
}

void Player::setWalkAnims(const std::vector<AnimCfg>& anims)
{
	for (auto& anm : anims)
	{
		m_walkAnims.push_back(std::make_shared<Anim>(anm));
	}
}

void Player::calculateDirection(_IN_VEC2 vel)
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

void Player::addInventory(_IN_STR id)
{
	if (!checkInventory(id))
	{
		m_inventory.push_back(id);
		theEngine->dialog->checkRequirements(id);
	}
		
}
