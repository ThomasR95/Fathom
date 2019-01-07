#pragma once
#include "tSprite.h"
class tCursor : public GfxBase
{
public:
	tCursor();
	~tCursor();

	virtual void tick(sf::Time& dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	enum cType
	{
		NORMAL,
		EYE,
		KEY
	};

	void init(_IN_STR eyeIcon, _IN_STR keyIcon);

	void setType(cType type) { m_type = type; }
	void setType(_IN_STR type);
	void normal() { m_type = NORMAL; }

private:

	tSprite* m_eyeIcon;
	tSprite* m_keyIcon;

	cType m_type;
};

