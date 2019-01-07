#pragma once
#include "Mobile.h"
#include <memory>

class CoinCounter : public Mobile
{
public:
	CoinCounter();
	~CoinCounter();

	void setBarSprite(const sf::Texture& sp) { m_fuelBar = sf::Sprite(sp); }
	void setHolderSprite(const sf::Texture& sp) { m_fuelHolder = sf::Sprite(sp); }

	void addCoin() { m_totalCoins++; }

	void collectCoin() { m_coinsCollected++; }
	void tick(sf::Time& dt) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setBarPosition(const sf::Vector2f& pos);

	void setMaxCoins(int coins) { m_maxCoins = coins; }

	bool noMoreCoins() { return m_totalCoins >= m_maxCoins; }

	void saveHighScore();

	int getHighScore() {return m_highScore;}
	int getCurrentScore() { return m_coinsCollected; }

private:
	int m_maxCoins;
	int m_totalCoins;
	int m_coinsCollected;
	int m_highScore;

	sf::Sprite m_fuelHolder;
	sf::Sprite m_fuelBar;

	bool m_highscoreSaved = false;

};

