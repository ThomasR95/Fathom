#pragma once
#include "typedefs.h"

#include "SFML\Graphics.hpp"
#include <unordered_map>
#include <functional>

class GfxBase : public sf::Drawable
{
public:
	GfxBase();
	~GfxBase();

	enum SHARE_TF {
		NONE = 0,
		TRANSLATE = 1,
		SCALE = 2,
		ROTATE = 4,
		ALL = 1 | 2 | 4
	};
	

	struct childSprite {
		std::string UID;
		GfxBase* pChild;
		SHARE_TF share;
	};

	virtual void tick(sf::Time& dt) {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	bool active = false;
	bool visible = false;

	int Zlevel;

	void addChildSprite(GfxBase* child, SHARE_TF share = SHARE_TF::ALL);

	GfxBase* getChildSprite(_IN_STR id);
	void removeChildSprite(_IN_STR id);
	void removeChildSprite(GfxBase* ptr);

	std::string m_UID;

	virtual bool isPlayerBehind(_IN_VEC2 playerFootPos) { return false; }
	virtual bool isPlayerBehind(_IN_VEC2 playerFootPos, float width) { return false; }

	virtual bool testCollision(sf::Vector2f& pos, sf::Vector2f& vel, GfxBase* actor) { return false; }

	virtual void setPlayerAlwaysInFront(bool inFront) { playerAlwaysInFront = inFront; }

	virtual void setCollisionEnabled(bool collide) { collisionEnabled = collide; }

	virtual void setInteraction(_IN_STR dialogTrigger, bool click, bool useAlpha = true) { m_dialogTrigger = dialogTrigger; m_clickable = click; m_clickTestAlpha = useAlpha; }

	virtual void setInteraction(std::function<void()> func, bool click, bool useAlpha = true) { m_interactFunction = func; m_clickable = click; m_clickTestAlpha = useAlpha; }

	virtual bool checkInteraction(sf::Vector2f mousePos) { if(m_interactFunction) m_interactFunction();  return false; }

	virtual bool checkMouseOver(sf::Vector2f mousePos) { return false; }

protected:
	std::vector<GfxBase*> m_parents;

	std::vector<childSprite> m_childSprites;

	bool playerAlwaysInFront = false;
	bool playerAlwaysBehind = false;
	bool collisionEnabled = true;

	bool m_clickable = false;
	bool m_clickTestAlpha = false;
	std::string m_dialogTrigger;

	std::function<void()> m_interactFunction;
};

