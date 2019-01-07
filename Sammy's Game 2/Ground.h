#pragma once
#include <SFML/Graphics.hpp>
#include "resourceManager.h"
#include "Mobile.h"
#include "Anim.h"


class Ground : public Mobile
{
public:
	Ground(in_str id);
	~Ground();

	enum collisionStatus {
		NONE,
		HOLE,
		BOTTOM,
		RIGHT,
		LEFT,
		TOP,
		ADJUST
	};

	struct Coin
	{
		Coin(std::shared_ptr<Mobile> sp, sf::Vector2f origin) : m_origin(origin) {
			if (sp->isAnimated())
				m_sprite = std::make_shared<Anim>(*(Anim*)sp.get());
			else
				m_sprite = std::make_shared<Mobile>(*sp);

			m_bounds.width = sp->getSize().x;
			m_bounds.height = sp->getSize().y;
		}
		std::shared_ptr<Mobile> m_sprite;
		sf::Vector2f m_pos;
		sf::Vector2f m_origin;
		sf::FloatRect m_bounds;
		sf::Sound m_coinSound;

		bool m_collected = false;
		bool checkCollision(sf::FloatRect bBox);

	};

	struct Obstacle
	{
		
		std::string m_name;
		sf::Vector2f m_origin;
		sf::Vector2f m_pos;
		sf::Vector2f m_size;
		std::shared_ptr<Mobile> m_sprite;
		std::vector<sf::FloatRect> m_collisionRects;
		std::vector<std::pair<sf::Vector2f, float>> m_collisionCircles;
		bool m_isHole = false;
		bool m_animated = false;
		bool m_floating = false;

		collisionStatus checkCollision(sf::FloatRect bBox, sf::Vector2f& vel);
	};

	struct Portal
	{

		std::string m_name;
		sf::Vector2f m_origin;
		sf::Vector2f m_pos;
		sf::Vector2f m_size;
		std::shared_ptr<Mobile> m_sprite;
		std::vector<sf::FloatRect> m_collisionRects;
		bool m_animated = false;

		bool checkCollision(sf::FloatRect bBox)
		{
			for (auto b : m_collisionRects)
			{
				b.top = m_pos.y;
				b.left = m_pos.x;
				if (b.intersects(bBox))
				{
					return true;
					
				}
					
			}
				
			return false;
		}
	};

	struct HoleInfo
	{
		const sf::Texture* m_tex;

		bool m_animated = false;
		int m_fps = 24;
		int m_frameHeight = 0;
		bool loop = true;
		float m_startPos = 0;
		float m_endPos = 0;
		sf::Vector2f m_size;
	};

	struct ObstacleInfo
	{
		std::string m_name;
		sf::Vector2f m_pos;
		sf::Vector2f m_size;
		const sf::Texture* m_tex;
		std::vector<sf::FloatRect> m_collisionRects;
		std::vector<std::pair<sf::Vector2f, float>> m_collisionCircles;
		bool m_animated = false;
		bool m_floating = false;
		int m_fps = 24;
		int m_frameHeight = 0;
		bool loop = true;
	};


	struct GroundBlock
	{
		std::string m_name;
		std::shared_ptr<Mobile> m_sprite;
		std::vector<std::shared_ptr<Mobile>> m_details;
		sf::Vector2f m_pos;
		sf::Vector2f m_size;
		std::vector<Obstacle> m_obstacles;
		std::vector<Coin> m_coins;
		bool m_isHole = false;
		bool m_isPortal = false;

		void addObstacle(ObstacleInfo info, sf::Vector2f offset);

	};

	void tick(sf::Time& dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setVelocity(sf::Vector2f vel) { m_velocity = vel; }
	sf::Vector2f getVelocity() { return m_velocity; }

	void addGroundBlock(in_str name, const sf::Texture* blockSprite, sf::Vector2f pos = { 0,0 });
	void addHole(in_str name, HoleInfo info, sf::Vector2f pos);

	void loadObstacle(std::pair<const sf::Texture*, std::string>& obstacleSprite);

	void Ground::loadPortal(const sf::Texture* tex, in_str portalXml);

	void loadHole(std::pair<const sf::Texture*, std::string>& holeSprite);

	void addSprite(const sf::Texture* sprite)
	{ 
		m_spriteList.push_back(sprite);
	}
	void addHoleSprite(const sf::Texture* sprite, in_str xmlFile)
	{
		loadHole(std::make_pair(sprite, xmlFile));
	}

	void addObstacleSprite(const sf::Texture* sprite, in_str xmlFile)
	{ 
		loadObstacle(std::make_pair(sprite, xmlFile));
	}

	Ground::collisionStatus checkObstacleCollisions(sf::FloatRect bBox, sf::Vector2f& vel);

	void setBackground(Anim& background) { 
		m_background.push_back(std::make_shared<Anim>(background));
		background.setPosition({ background.getSize().x, 0 });
		m_background.push_back(std::make_shared<Anim>(background));
	}

	void setCoinSprite(std::shared_ptr<Anim> coin) { m_coinSprite = coin; }

	Portal* getPortal() { return m_portal.get(); }

	void setCoinSound(sf::SoundBuffer* sb) { if(sb) m_coinSound.setBuffer(*sb); }

	void addDetail(std::shared_ptr<Mobile> det) { m_detailsList.push_back(det); }

private:

	int m_blockCounter;

	int m_startBlocks = 3;

	sf::Vector2f m_velocity;

	std::vector<std::shared_ptr<Mobile>> m_background;

	std::vector<GroundBlock> m_groundBlocks;

	std::vector<const sf::Texture*> m_spriteList;
	std::vector<std::shared_ptr<Mobile>> m_detailsList;

	std::vector<ObstacleInfo> m_obstacleBank;

	std::vector<HoleInfo> m_holeBank;

	std::shared_ptr<Mobile> m_coinSprite;

	std::shared_ptr<Portal> m_portal;

	bool m_portalNext = false;
	bool m_portalSet = false;

	sf::Sound m_coinSound;

};

