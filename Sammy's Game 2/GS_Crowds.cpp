#include "GS_Crowds.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Crowds::GS_Crowds()
{
	m_crowdMoves = {
		{"group_32", {2440,159}},
		{"group_16", {2986,708}},
		{"group_26", {2002,1040}},
		{"group_24", {1683,1071}},
		{"group_17", {2719,880}},
		{"group_5",	 {1088,485}},
		{"group_7",	 {493,849}},
		{"group_13", {1652,1121}},
		{"group_2",	 {1848,1140}},
		{"group_19", {1893,1298}},
		{"group_14", {1793,1857}},
		{"group_20", {912,1388}}
	};
}


GS_Crowds::~GS_Crowds()
{
}

void GS_Crowds::enter()
{
	theEngine->setNextState("GS_EXIT");
}

void GS_Crowds::run()
{
	int i = 0;
	for (auto& c : m_crowdMoves)
	{
		auto gfx = ((Obstacle*)theEngine->getCurrentWorld()->getAsset(c.first));
		auto pos = gfx->sprite.getPosition();
		auto bounds = gfx->sprite.getGlobalBounds();
		pos += {bounds.width / 2, bounds.height};
		auto pFeet = GameObjects::thePlayer.getFeetPosition();
		auto left = pFeet; left.x -= GameObjects::thePlayer.getPlayerWidth() / 2;
		auto right = pFeet; right.x += GameObjects::thePlayer.getPlayerWidth() / 2;
		auto toPos = pFeet - pos;
		if (fabs(toPos.x) < 450 && (toPos.y < 300 && toPos.y > -600) 
			&& !gfx->testCollision(pFeet,sf::Vector2f(),&GameObjects::thePlayer) 
			&& !gfx->testCollision(left, sf::Vector2f(), &GameObjects::thePlayer) 
			&& !gfx->testCollision(right, sf::Vector2f(), &GameObjects::thePlayer))
		{

			gfx->setCollisionEnabled(false);
			//tEffector::transforms t; t.position = c.second;
			//tEffector::activity a; a.position = true;
			//gfx->addEffector(tEffector(t, a, 2000));

			auto A = gfx->sprite.getPosition();
			auto B = c.second;
			auto AB = B - A;
			gfx->sprite.setPosition(A + norm(AB)*0.5f);

			gfx->active;
			
			if (len(AB) == 0)
			{
				m_crowdMoves.erase(m_crowdMoves.begin() + i);
				break;
			}
			
		}
		i++;
	}
}

void GS_Crowds::exit()
{
}
