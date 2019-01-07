#pragma once
#include "typedefs.h"

#include "GfxBase.h"
#include "tEffector.h"

class tSprite : public GfxBase
{
public:
	enum origin {
		TOP_LEFT,
		TOP_CENTRE,
		TOP_RIGHT,
		CENTRE_LEFT,
		CENTRE_CENTRE,
		CENTRE_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTRE,
		BOTTOM_RIGHT
	};

	tSprite();
	tSprite(sf::Texture& tex, _IN_VEC2 pos, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT);
	~tSprite();

	virtual void tick(sf::Time& dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual int getNumEffectors(){ return m_effectors.size(); }
	virtual void addEffector(const tEffector& eff) { m_effectors.push_back(eff); }
	virtual void clearEffectors() { m_effectors.clear(); }

	sf::Sprite sprite;

	void setOrigin(origin org);

	bool checkInteraction(sf::Vector2f mousePos);
	void setCursorType(_IN_STR cursor) { m_mouseCursor = cursor; }
	bool checkMouseOver(sf::Vector2f mousePos) override;

protected:

	std::vector<tEffector> m_effectors;
	std::string m_mouseCursor = "";
	sf::Vector2f m_size;
	std::vector<sf::Image> m_image;
};

