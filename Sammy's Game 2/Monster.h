#pragma once

#include "Mobile.h"
#include "Anim.h"
#include "typedefs.h"
#include <memory>
#include <SFML\Audio.hpp>


class Monster :
	public Mobile
{
public:
	Monster(in_str id);
	~Monster();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void tick(sf::Time& dt) override;

	void setRunAnim(Anim& run) 
	{ 
		m_runningAnim = std::make_shared<Anim>(run);
		m_size = m_runningAnim->getSize();
		m_bounds.width = m_size.x;
		m_bounds.height = m_size.y;
	}

	void setAttackAnim(Anim& attack) {m_attackAnim = std::make_shared<Anim>(attack);}


	sf::Vector2f getSize() { return m_size; }

	void stopFollowing() { m_catchUp = false; }

	void setRoarSound(sf::SoundBuffer* sb) { if (sb) m_roarSound.setBuffer(*sb); m_roarSound.setVolume(20); }
	void setWalkSound(sf::Music* music) { if (music) m_walkSound = music; }

private:
	sf::Vector2f m_targetPos;
	sf::Vector2f m_pos;
	sf::Vector2f m_size;
	sf::Vector2f m_lastPos;
	sf::FloatRect m_bounds;

	bool m_caughtUp = false;

	std::shared_ptr<Anim> m_runningAnim;
	std::shared_ptr<Anim> m_attackAnim;

	sf::Sound m_roarSound;
	bool m_roarPlayable = true;
	sf::Music* m_walkSound;
	bool m_walkPlayable = true;

	bool m_catchUp = true;
};

