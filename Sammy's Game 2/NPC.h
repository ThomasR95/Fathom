#pragma once
#include "tSprite.h"
#include "Anim.h"
#include "typedefs.h"
#include <memory>
#include <deque>
#include <SFML/Audio.hpp>

class NPC : public tSprite
{
public:
	NPC();
	NPC(sf::Texture& tex, sf::Texture* navMask, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT);
	~NPC();

	void setVelocity(sf::Vector2f vel) { m_vel = vel; }
	void addVelocity(sf::Vector2f add) { m_vel += add; }
	sf::Vector2f getVelocity() { return m_vel; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void tick(sf::Time& dt) override;

	sf::Vector2f getSize() { return m_size; }

	sf::FloatRect getBounds() { return m_bounds; }

	void setNPCWidth(float w) { m_width = w; }
	float getNPCWidth() { return m_width; }

	void setNPCFeetOffset(float f) { m_feetOffset = f; }

	sf::Vector2f getFeetPosition() { return { m_pos.x, m_pos.y + m_feetOffset }; }

	void setIdleAnims(const std::vector<AnimCfg>& anims);
	void setWalkAnims(const std::vector<AnimCfg>& anims);

	void calculateDirection(_IN_VEC2 vel);

	bool isMouseLed() { return m_isMouseLed; }
	void followMouse(bool follow) 
	{ 
		m_followMouse = follow; 
	}

	void addInventory(_IN_STR id) { if (!checkInventory(id)) m_inventory.push_back(id); }
	bool checkInventory(_IN_STR id) 
	{
		for (auto& i : m_inventory)
			if (i == id)
				return true;
		return false;
	}

	void moveTo(_IN_VEC2 pos) { m_targetQueue.push_front(pos); }
	void clearTargetQ() { m_targetQueue.clear(); }

	virtual bool isPlayerBehind(_IN_VEC2 playerFootPos);
	virtual bool isPlayerBehind(_IN_VEC2 playerFootPos, float width);

	virtual bool testCollision(sf::Vector2f& pos, sf::Vector2f& vel, GfxBase* actor);

	virtual bool checkInteraction(sf::Vector2f mousePos);

	void setPosition(_IN_VEC2 pos) {
		m_pos = pos;
		sprite.setPosition(m_pos);
		for (auto& i : m_idleAnims)
			i->sprite.setPosition(m_pos);
		for (auto& w : m_walkAnims)
			w->sprite.setPosition(m_pos);
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
		if(which == animType::WALK || which == animType::BOTH)
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

	sf::Color getColour() { if (m_currentAnim) return m_currentAnim->sprite.getColor(); else return {255,255,255,255}; }

private:
	sf::Vector2f m_vel;

	sf::Vector2f m_pos;
	sf::Vector2f m_feetPos;
	sf::Vector2f m_size;
	sf::Vector2f m_lastPos;

	sf::FloatRect m_bounds;

	int m_dir = 0;

	Anim* m_currentAnim = nullptr;

	std::vector<std::shared_ptr<Anim>> m_walkAnims;
	std::vector<std::shared_ptr<Anim>> m_idleAnims;

	std::deque<sf::Vector2f> m_targetQueue;

	float m_width;

	float m_maxSpeed = 1000;
	float m_speed;

	bool m_followMouse = false;

	float m_feetOffset = 0;

	bool m_mouseHidden = false;

	bool m_isMouseLed = false;

	std::vector<std::string> m_inventory;

	//obstacle stuff
	sf::Vector2f reduceVelocity(_IN_VEC2 pos, _IN_VEC2 vel);

	sf::Image m_navMask;

	sf::Vector2f m_navSize;
	sf::Vector2f m_texSize;
	sf::Vector2f m_navScale;

};

