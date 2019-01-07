#include "typedefs.h"

#include "tSprite.h"
#include "engine.h"
#include "tCursor.h"

tSprite::tSprite()
{
}

tSprite::tSprite(sf::Texture & tex, _IN_VEC2 pos, _IN_VEC2 src, _IN_VEC2 in_size, origin org)
{
	sprite.setTexture(tex);
	sprite.setPosition(pos);
	sf::Vector2i size;
	if (in_size == sf::Vector2f(-1, -1))
		size = sf::Vector2i(tex.getSize());
	else
		size = sf::Vector2i(in_size);

	m_size = sf::Vector2f(size);

	sprite.setTextureRect(sf::IntRect(sf::Vector2i(src), size));

	setOrigin(org);

}


tSprite::~tSprite()
{
	m_image.clear();
}

void tSprite::tick(sf::Time & dt)
{
	if(m_effectors.size())
	{
		if (m_effectors[0].applyEffects(&sprite))
			m_effectors.erase(m_effectors.begin());
	}

	for (auto& c : m_childSprites)
	{
		if (!c.pChild->active)
			continue;
		c.pChild->tick(dt);
	}
}

void tSprite::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);

	for (auto& c : m_childSprites)
	{
		if (!c.pChild->visible)
			continue;

		auto cStates = states;
		
		auto share = c.share;
		if(share & GfxBase::SHARE_TF::TRANSLATE)
			cStates.transform.translate(sprite.getPosition());
		if (share & GfxBase::SHARE_TF::ROTATE)
			cStates.transform.rotate(sprite.getRotation());
		if (share & GfxBase::SHARE_TF::SCALE)
			cStates.transform.scale(sprite.getScale());

		c.pChild->draw(target, cStates);
	}
}

void tSprite::setOrigin(origin org)
{
	switch (org)
	{
	case tSprite::TOP_LEFT:
		sprite.setOrigin({ 0,0 });
		break;
	case tSprite::TOP_CENTRE:
		sprite.setOrigin({ m_size.x / 2, 0 });
		break;
	case tSprite::TOP_RIGHT:
		sprite.setOrigin({ m_size.x, 0 });
		break;
	case tSprite::CENTRE_LEFT:
		sprite.setOrigin({ 0,m_size.y/2 });
		break;
	case tSprite::CENTRE_CENTRE:
		sprite.setOrigin(m_size/2.f);
		break;
	case tSprite::CENTRE_RIGHT:
		sprite.setOrigin({ m_size.x,m_size.y / 2 });
		break;
	case tSprite::BOTTOM_LEFT:
		sprite.setOrigin({ 0,m_size.y });
		break;
	case tSprite::BOTTOM_CENTRE:
		sprite.setOrigin({ m_size.x /2, m_size.y });
		break;
	case tSprite::BOTTOM_RIGHT:
		sprite.setOrigin(m_size);
		break;
	default:
		break;
	}
}


bool tSprite::checkInteraction(sf::Vector2f mousePos)
{
	if (!m_clickable || !visible)
		return false;

	auto bounds = sprite.getGlobalBounds();
	bool clicked = false;
	if (bounds.contains(mousePos))
	{
		if (!m_clickTestAlpha)
			clicked = true;
		else
		{
			sf::Vector2f relativePos = mousePos - (sprite.getPosition() - sprite.getOrigin());
			auto tex = sprite.getTexture();
			if (tex->copyToImage().getPixel(relativePos.x, relativePos.y).a > 50u)
				clicked = true;
		}
	}

	if (clicked && m_dialogTrigger.size() > 0u)
	{
		theEngine->dialog->startDialog(m_dialogTrigger);
	}
	if (clicked && m_interactFunction)
		m_interactFunction();

	return clicked;
}

bool tSprite::checkMouseOver(sf::Vector2f mousePos)
{
	if (!m_clickable || !visible)
		return false;

	auto bounds = sprite.getGlobalBounds();
	bool mouseOver = false;
	if (bounds.contains(mousePos))
	{
		if (!m_clickTestAlpha)
			mouseOver = true;
		else
		{
			sf::Vector2f relativePos = mousePos - (sprite.getPosition() - sprite.getOrigin());
			if (m_image.size() < 1u)
			{
				auto tex = sprite.getTexture();
				m_image.push_back(tex->copyToImage());
			}
			if (m_image[0].getPixel(relativePos.x, relativePos.y).a > 50u)
				mouseOver = true;
		}
	}
	else
		m_image.clear();

	if (mouseOver && m_mouseCursor != "" && (m_dialogTrigger.size() == 0 || theEngine->dialog->getEntry(m_dialogTrigger)->active))
	{
		theEngine->getCursor()->setType(m_mouseCursor);
	}

	return mouseOver;
}
