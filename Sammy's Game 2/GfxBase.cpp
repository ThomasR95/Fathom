#include "GfxBase.h"



GfxBase::GfxBase()
{
}


GfxBase::~GfxBase()
{
}

void GfxBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*this, states);

	if (m_childSprites.size())
	{
		for (auto& c : m_childSprites)
			c.pChild->draw(target, states);
	}
}

void GfxBase::addChildSprite(GfxBase * child, SHARE_TF share_)
{
	if (!getChildSprite(child->m_UID))
	{
		childSprite newchild;
		newchild.UID = child->m_UID;
		newchild.pChild = child;
		newchild.share = share_;
		m_childSprites.push_back(newchild);
		child->m_parents.push_back(this);
	}
}

GfxBase * GfxBase::getChildSprite(_IN_STR id)
{
	for(auto& c : m_childSprites)
		if(c.UID == id)
			return c.pChild;

	return nullptr;
}

void GfxBase::removeChildSprite(_IN_STR id)
{
	int idx = 0;
	for (auto& c : m_childSprites)
	{
		if (c.UID == id)
		{
			auto& parents = c.pChild->m_parents;
			for (uint8_t p = 0; p < parents.size(); p++)
				if (parents[p] == this)
					parents.erase(parents.begin() + p);

			m_childSprites.erase(m_childSprites.begin() + idx);
			break;
		}
		idx++;
	}
}

void GfxBase::removeChildSprite(GfxBase * ptr)
{
	std::string match;
	for (auto& c : m_childSprites)
	{
		if (c.pChild == ptr)
		{
			match = c.UID;
			break;
		}	
	}

	removeChildSprite(match);
}
