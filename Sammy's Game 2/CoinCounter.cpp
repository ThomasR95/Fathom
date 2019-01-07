#include "CoinCounter.h"
#include "tinyxml2\tinyxml2.h"
#include <iostream>


CoinCounter::CoinCounter() : Mobile("coinCounter")
{
	m_coinsCollected = 0;
	m_totalCoins = 0;

	tinyxml2::XMLDocument doc;
	doc.LoadFile("../data/hs.xml");

	auto root = doc.FirstChildElement("root");
	if (root)
	{
		auto hs = root->FirstChildElement("Highscore");
		m_highScore = hs->IntAttribute("score");
	}
	std::cout << "high score = " << m_highScore << std::endl;

}


CoinCounter::~CoinCounter()
{
}

void CoinCounter::tick(sf::Time & dt)
{
	float ratio = (float)m_coinsCollected / m_maxCoins;
	m_fuelBar.setTextureRect({ 0, 0, (int)(m_fuelBar.getTexture()->getSize().x * ratio), (int)m_fuelBar.getTexture()->getSize().y });
}

void CoinCounter::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_fuelBar);
	target.draw(m_fuelHolder);
}

void CoinCounter::setBarPosition(const sf::Vector2f & pos)
{
	setPosition(pos);
	m_fuelBar.setPosition(pos);
	m_fuelHolder.setPosition(pos);
}

void CoinCounter::saveHighScore()
{
	if (m_coinsCollected > m_highScore && !m_highscoreSaved)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile("../data/hs.xml");

		auto root = doc.FirstChildElement("root");
		if (root)
		{
			auto hs = root->FirstChildElement("Highscore");

			hs->SetAttribute("score", m_coinsCollected);

			doc.SaveFile("../data/hs.xml");
			m_highscoreSaved = true;
		}

	}
}
