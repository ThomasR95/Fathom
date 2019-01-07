#pragma once
#include "tSprite.h"
#include "Anim.h"
#include "typedefs.h"
#include <memory>
#include <SFML/Audio.hpp>

class Player : public tSprite
{
public:
	Player();
	Player(sf::Texture& tex, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT);
	~Player();

	void setVelocity(sf::Vector2f vel) { m_vel = vel; }
	void addVelocity(sf::Vector2f add) { m_vel += add; }
	sf::Vector2f getVelocity() { return m_vel; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void tick(sf::Time& dt) override;

	sf::Vector2f getSize() { return m_size; }

	sf::FloatRect getBounds() { return m_bounds; }

	void setPlayerWidth(float w) { m_width = w; }
	float getPlayerWidth() { return m_width; }

	void setPlayerFeetOffset(float f) { m_feetOffset = f; }

	sf::Vector2f getFeetPosition() { return { m_pos.x, m_pos.y + m_feetOffset }; }

	void setIdleAnims(const std::vector<AnimCfg>& anims);
	void setWalkAnims(const std::vector<AnimCfg>& anims);

	void calculateDirection(_IN_VEC2 vel);

	bool isMouseLed() { return m_isMouseLed; }
	void stopMouseInput() { m_isMouseLed = false; }
	void followMouse(bool follow) 
	{ 
		m_followMouse = follow; 
	}

	void addInventory(_IN_STR id);
	bool checkInventory(_IN_STR id) 
	{
		for (auto& i : m_inventory)
			if (i == id)
				return true;
		return false;
	}

	void setPosition(_IN_VEC2 pos) {
		m_pos = pos;
		sprite.setPosition(m_pos);
		for (auto& i : m_idleAnims)
			i->sprite.setPosition(m_pos);
		for (auto& w : m_walkAnims)
			w->sprite.setPosition(m_pos);
	}

	void run(bool run) {
		m_running = run;
	}

	void setMaxSpeed(float max) { m_maxSpeed = max; }

	struct animType {
		enum type {
			WALK,
			IDLE,
			BOTH
		};
	};

	void addEffector(const tEffector& e, animType::type which = animType::BOTH)
	{
		if (which == animType::WALK || which == animType::BOTH)
			for (auto& a : m_walkAnims)
				a->addEffector(e);
		if (which == animType::IDLE || which == animType::BOTH)
			for (auto& a : m_idleAnims)
				a->addEffector(e);
	}

	int getNumEffectors(animType::type which = animType::BOTH)
	{
		int num = 0;
		if (which == animType::WALK || which == animType::BOTH)
			for (auto& a : m_walkAnims)
				if (a->getNumEffectors() > num) num = a->getNumEffectors();
		if (which == animType::IDLE || which == animType::BOTH)
			for (auto& a : m_idleAnims)
				if (a->getNumEffectors() > num) num = a->getNumEffectors();
		return num;
	}

	void setScale(_IN_VEC2 s, animType::type which = animType::BOTH)
	{
		if (which == animType::WALK || which == animType::BOTH)
			for (auto& a : m_walkAnims)
				a->sprite.setScale(s);
		if (which == animType::IDLE || which == animType::BOTH)
			for (auto& a : m_idleAnims)
				a->sprite.setScale(s);
	}

	void setColour(const sf::Color& c, animType::type which = animType::BOTH)
	{
		if (which == animType::WALK || which == animType::BOTH)
			for (auto& a : m_walkAnims)
				a->sprite.setColor(c);
		if (which == animType::IDLE || which == animType::BOTH)
			for (auto& a : m_idleAnims)
				a->sprite.setColor(c);
	}

	void setDir(int dir) { m_dir = dir; }

private:
	sf::Vector2f m_vel;

	sf::Vector2f m_pos;
	sf::Vector2f m_feetPos;
	sf::Vector2f m_size;
	sf::Vector2f m_lastPos;

	sf::FloatRect m_bounds;

	int m_dir = 0;

	std::vector<std::shared_ptr<Anim>> m_walkAnims;
	std::vector<std::shared_ptr<Anim>> m_idleAnims;

	float m_width;

	float m_maxSpeed = 1000;
	float m_speed;

	bool m_followMouse = true;

	float m_feetOffset = 0;

	bool m_mouseHidden = false;

	bool m_isMouseLed = true;

	bool m_running = false;

	std::vector<std::string> m_inventory;

};

