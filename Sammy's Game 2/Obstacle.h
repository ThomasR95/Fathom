#pragma once
#include "tSprite.h"
#include "Anim.h"
class Obstacle : public Anim
{
public:
	Obstacle() {};
	Obstacle(sf::Texture* tex, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, tSprite::origin org = tSprite::TOP_LEFT);
	Obstacle(sf::Texture* tex, sf::Texture* navMask, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, tSprite::origin org = tSprite::TOP_LEFT);
	Obstacle(sf::Texture* tex, int frames, int rows, int cols, sf::Texture* navMask, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, tSprite::origin org = tSprite::TOP_LEFT);
	~Obstacle();

	typedef struct {
		bool left = false;
		bool right = false;
		bool top = false;
		bool bottom = false;

		bool anyCollision() {return (left || right || top || bottom);}
	}SIDE;

	bool isPlayerBehind(_IN_VEC2 playerFootPos);
	bool isPlayerBehind(_IN_VEC2 playerFootPos, float width);

	SIDE testCollision(_IN_VEC2 pos);

	bool testCollision(sf::Vector2f& pos, sf::Vector2f& vel, GfxBase* actor);

	void setPlayerAlwaysInFront(bool inFront) { playerAlwaysInFront = inFront; }

	void setCollisionEnabled(bool collide) { collisionEnabled = collide; }

	bool checkInteraction(sf::Vector2f mousePos);

protected:

	sf::Vector2f reduceVelocity(_IN_VEC2 pos, _IN_VEC2 vel);

	sf::Image m_navMask;

	sf::Vector2f m_navSize;
	sf::Vector2f m_texSize;
	sf::Vector2f m_navScale;
};

