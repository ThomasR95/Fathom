#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include "Mobile.h"
#include "Player.h"
#include "Monster.h"
#include "Ground.h"
#include "CoinCounter.h"

typedef const std::string& in_str;
class Scene
{
public:
	Scene();
	~Scene();

	void create(in_str backgroundFile, in_str groundFile);

	Ground* getGround() { return (Ground*)m_ground.get(); }

	Player* getPlayer() { return m_player.get(); }

	Monster* getMonster() { return m_monster.get(); }

	int getGroundHeight() { return m_groundHeight; }
	float getGravity() { return m_gravity; }

	void tick(sf::Time& dt);

	CoinCounter* getCoinCounter() { return m_coinCounter.get(); }

private:

	std::shared_ptr<Mobile> m_ground;
	std::shared_ptr<sf::Drawable> m_background;
	std::vector<std::shared_ptr<sf::Drawable>> m_obstacles;

	int m_groundHeight;
	float m_gravity;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<Monster> m_monster;

	std::shared_ptr<CoinCounter> m_coinCounter;

	sf::Time m_timer;

	std::deque<std::shared_ptr<Anim>> m_birds;
	std::shared_ptr<Anim> m_birdAnim;

};

