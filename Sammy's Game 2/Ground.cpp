#include "Ground.h"
#include "engine.h"

#include "tinyxml2\tinyxml2.h"

Ground::Ground(in_str id) : Mobile(id)
{
	m_velocity = { 0,0 };
	srand(std::time(0));
	m_blockCounter = 0;
}


Ground::~Ground()
{
}

void Ground::tick(sf::Time & dt)
{
	m_velocity = { -theEngine->getScene()->getPlayer()->getVelocity().x, 0.f };

	//erase ground blocks that are off the back of the screen
	if (m_groundBlocks.size() > 0 && m_groundBlocks[0].m_pos.x < (0 - (int)m_groundBlocks[0].m_size.x))
	{
		m_groundBlocks.erase(m_groundBlocks.begin());
	}

	while (m_groundBlocks.size() == 0 || m_groundBlocks.back().m_pos.x + m_groundBlocks.back().m_size.x < theEngine->getScrW())
	{
		int randHole = rand() % 150;
		randHole /= 100;
		if (randHole && !m_startBlocks && m_holeBank.size() && theEngine->getScene()->getPlayer()->getVelocity().x > 90)
		{
			size_t randInt = rand() % (m_holeBank.size() * 20);
			randInt /= 20;
			auto& hole = m_holeBank[randInt];
			float offset = theEngine->getScrH() - hole.m_size.y;
			if (m_groundBlocks.size() > 0)
				addHole("holeBlock" + std::to_string(m_blockCounter++), hole, { m_groundBlocks.back().m_pos.x + m_groundBlocks.back().m_size.x, offset });
			else
				addHole("holeBlock" + std::to_string(m_blockCounter++), hole, { 0, offset });
		}
		else
		{
			size_t randInt = rand() % (m_spriteList.size() * 20);
			randInt /= 20;
			auto& sp = m_spriteList[randInt];
			float offset = theEngine->getScrH() - sp->getSize().y;
			if (m_groundBlocks.size() > 0)
				addGroundBlock("groundBlock" + std::to_string(m_blockCounter++), sp, { m_groundBlocks.back().m_pos.x + m_groundBlocks.back().m_size.x, offset });
			else
				addGroundBlock("groundBlock" + std::to_string(m_blockCounter++), sp, { 0, offset });
		}
		
	}

	size_t idx = 0;
	for (auto& b : m_groundBlocks)
	{
		auto pos = b.m_sprite->getPosition();
		pos += m_velocity * (float)dt.asSeconds();
		b.m_sprite->setPosition(pos);
		b.m_sprite->tick(dt);
		b.m_pos = pos;
		for (auto& o : b.m_obstacles)
		{
			if (o.m_sprite.get())
			{
				o.m_sprite->setPosition(b.m_pos + o.m_origin);
				o.m_pos = o.m_sprite->getPosition();
				o.m_sprite->tick(dt);
			}
			else
			{
				o.m_pos = b.m_pos + o.m_origin;
			}
		}
		for (auto& d : b.m_details)
		{
			d->setPosition(b.m_pos);
			d->tick(dt);
		}
		for (auto& c : b.m_coins)
		{
			c.checkCollision(theEngine->getScene()->getPlayer()->getBounds());

			if (c.m_sprite.get())
			{
				c.m_sprite->setPosition(b.m_pos + c.m_origin);
				c.m_pos = c.m_sprite->getPosition();
				c.m_bounds.left = c.m_pos.x;
				c.m_bounds.top = c.m_pos.y;
				c.m_sprite->tick(dt);
			}
		}
		if (b.m_isPortal)
		{
			m_portal->m_sprite->setPosition(b.m_pos + m_portal->m_origin);
			m_portal->m_pos = m_portal->m_sprite->getPosition();
			m_portal->m_sprite->tick(dt);
			if (idx < 2)
			{
				theEngine->getScene()->getMonster()->stopFollowing();
			}
		}
		idx++;
	}

	for (auto& b : m_background)
	{
		b->setPosition(b->getPosition() + m_velocity*dt.asSeconds() / 3.f);
		if (b->getPosition().x <= 0 - b->getSize().x)
			b->setPosition({ b->getSize().x, 0 });
		b->tick(dt);
	}
	

}

void Ground::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto& b : m_background)
	{
		theEngine->getWindow()->draw(*b.get());
	}

	for (auto& b : m_groundBlocks)
	{
		theEngine->getWindow()->draw(*b.m_sprite);

		if (b.m_isPortal)
		{
			theEngine->getWindow()->draw(*m_portal->m_sprite);
		}

		for (auto& o : b.m_obstacles)
		{
			if(o.m_sprite.get())
				theEngine->getWindow()->draw(*o.m_sprite.get());

			if (theEngine->debugHitboxes)
			{
				sf::RectangleShape hBox;
				hBox.setOutlineColor({ 255,0,0,255 });
				hBox.setFillColor({ 0,0,0,0 });
				hBox.setOutlineThickness(0.5);

				sf::CircleShape hCircle;
				hCircle.setOutlineColor({ 255,0,0,255 });
				hCircle.setFillColor({ 0,0,0,0 });
				hCircle.setOutlineThickness(0.5);
				for (auto r : o.m_collisionRects)
				{
					r.left += o.m_pos.x;
					r.top += o.m_pos.y;
					hBox.setPosition(sf::Vector2f(r.left, r.top));
					hBox.setSize(sf::Vector2f(r.width, r.height));
					theEngine->getWindow()->draw(hBox);
				}
				for (auto c : o.m_collisionCircles)
				{
					c.first += o.m_pos;
					hCircle.setPosition(c.first);
					hCircle.setOrigin(c.second, c.second);
					hCircle.setRadius(c.second);
					theEngine->getWindow()->draw(hCircle);
				}
			}
			
		}
		for (auto& d : b.m_details)
		{
			theEngine->getWindow()->draw(*d.get());
		}
		for (auto& c : b.m_coins)
		{
			if (c.m_sprite.get())
				theEngine->getWindow()->draw(*c.m_sprite.get());


			if (theEngine->debugHitboxes)
			{
				sf::RectangleShape hBox;
				hBox.setFillColor({ 255,255,0,50 });

				hBox.setPosition(sf::Vector2f(c.m_bounds.left, c.m_bounds.top));
				hBox.setSize(sf::Vector2f(c.m_bounds.width, c.m_bounds.height));
				theEngine->getWindow()->draw(hBox);
			}
		}
	}

}


void Ground::addGroundBlock(in_str name, const sf::Texture*  blockSprite, sf::Vector2f pos)
{
	GroundBlock newBlock;
	newBlock.m_name = name;
	newBlock.m_sprite = std::make_shared<Mobile>(name, *blockSprite);
	newBlock.m_pos = pos;
	newBlock.m_sprite->setPosition(pos);
	newBlock.m_size = newBlock.m_sprite->getSize();

	if (m_portalNext && !m_portalSet)
	{
		newBlock.m_isPortal = true;
		float yOffset = newBlock.m_size.y - m_portal->m_size.y - 20;
		m_portal->m_origin = { 0, yOffset };
		m_portalNext = false;
		m_portalSet = true;
	}
	else
	{
		if (m_startBlocks > 0)
			m_startBlocks--;
		else
		{
			if (m_obstacleBank.size())
			{
				size_t numObstacles = rand() % 40;
				numObstacles /= 20;
				for (size_t i = 0; i < numObstacles; i++)
				{
					size_t randInt = rand() % m_obstacleBank.size();
					auto& ob = m_obstacleBank[randInt];
					sf::Vector2f offset = { (float)(newBlock.m_obstacles.size() + 1) * 40 ,newBlock.m_size.y - ob.m_size.y - 20 };
					newBlock.addObstacle(ob, offset);
					newBlock.m_obstacles.back().m_pos = newBlock.m_obstacles.back().m_origin + newBlock.m_pos;
				}
			}
			size_t numCoins = rand() % 80;
			numCoins /= 20;
			for (size_t c = 0; c < numCoins; c++)
			{
				sf::Vector2f origin;
				bool noGo = false;
				origin.y = rand() % 70 - 40;
				origin.x = rand() % int(newBlock.m_size.x);
				for (auto& o : newBlock.m_obstacles)
				{
					while (o.checkCollision(sf::FloatRect(origin + newBlock.m_pos, { 20.f,30.f }), sf::Vector2f()))
					{
						if(o.m_floating)
							origin.y += 5;
						else
							origin.y -= 5;
					}
				}
				Coin newCoin(m_coinSprite, origin);
				newCoin.m_coinSound = m_coinSound;
				newBlock.m_coins.push_back(newCoin);
				theEngine->getScene()->getCoinCounter()->addCoin();
				if (theEngine->getScene()->getCoinCounter()->noMoreCoins())
					m_portalNext = true;
			}
		}
		
	}

	int chance = rand() % 300;
	if (chance < 50)
	{
		if (m_detailsList.size())
		{
			size_t randInt = rand() % m_detailsList.size();
			auto& det = m_detailsList[randInt];
			sf::Vector2f offset = { (float)(newBlock.m_details.size() + 1) * 20 ,newBlock.m_size.y - det->getSize().y - 20 };
			det->setOrigin(-offset);
			if (det->isAnimated())
			{
				newBlock.m_details.push_back(std::make_shared<Anim>(*(Anim*)det.get()));
			}
			else
			{
				newBlock.m_details.push_back(std::make_shared<Mobile>(*det.get()));
			}
		}
	}
	


	m_groundBlocks.push_back(newBlock);
}

void Ground::addHole(in_str name, HoleInfo info, sf::Vector2f pos)
{
	GroundBlock newBlock;
	newBlock.m_name = name;
	if (info.m_animated)
	{
		newBlock.m_sprite = std::make_shared<Anim>(name, *info.m_tex, info.m_frameHeight, info.m_fps, info.loop);
	}
	else
	{
		newBlock.m_sprite = std::make_shared<Mobile>(name, *info.m_tex);
	}
	newBlock.m_pos = pos;
	newBlock.m_sprite->setPosition(pos);
	newBlock.m_size = newBlock.m_sprite->getSize();
	newBlock.m_isHole = true;

	Obstacle hole;
	sf::FloatRect holeBox;
	holeBox.left = info.m_startPos;
	holeBox.top = theEngine->getScene()->getGroundHeight() - newBlock.m_pos.y - 0.5;
	holeBox.width = info.m_endPos - info.m_startPos;
	holeBox.height = 200;
	hole.m_isHole = true;
	hole.m_collisionRects.push_back(holeBox);
	newBlock.m_obstacles.push_back(hole);

	Obstacle landing;
	sf::FloatRect landBox;
	landBox.left = info.m_endPos - 5;
	landBox.top = holeBox.top - 1.5;
	landBox.width = 30;
	landBox.height = 10;
	landing.m_collisionRects.push_back(landBox);
	newBlock.m_obstacles.push_back(landing);

	m_groundBlocks.push_back(newBlock);
}

void Ground::loadObstacle(std::pair<const sf::Texture*, std::string>& obstacleSprite)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(("../images/" + obstacleSprite.second).c_str());

	Ground::ObstacleInfo newObstacle;

	auto root = doc.FirstChildElement("root");
	if (root)
	{
		auto obst = root->FirstChildElement("Obstacle");
		if (obst)
		{
			bool floating = obst->BoolAttribute("floating");
			newObstacle.m_floating = floating;
			bool animated = obst->BoolAttribute("animated");
			newObstacle.m_animated = animated;
			newObstacle.m_tex = obstacleSprite.first;
			if (animated)
			{
				sf::Int64 frameHeight = obst->IntAttribute("frameHeight");
				int fps = obst->IntAttribute("fps");
				bool loop = obst->BoolAttribute("loop");

				newObstacle.m_frameHeight = frameHeight;
				newObstacle.m_fps = fps;
				newObstacle.loop = loop;
				newObstacle.m_size = sf::Vector2f(newObstacle.m_tex->getSize().x, frameHeight);
			}
			else
			{
				newObstacle.m_size = sf::Vector2f(newObstacle.m_tex->getSize());
			}


			auto hitbox = obst->FirstChildElement("Hitbox");
			while (hitbox)
			{
				int x = hitbox->IntAttribute("startX");
				int y = hitbox->IntAttribute("startY");
				int width = hitbox->IntAttribute("width");
				int height = hitbox->IntAttribute("height");
				sf::FloatRect hBox(x, y, width, height);
				newObstacle.m_collisionRects.push_back(hBox);

				hitbox = hitbox->NextSiblingElement("Hitbox");
			}

			auto hitcircle = obst->FirstChildElement("Hitcircle");
			while (hitcircle)
			{
				float x = hitcircle->IntAttribute("centreX");
				float y = hitcircle->IntAttribute("centreY");
				float radius = hitcircle->IntAttribute("radius");
				auto circle = std::make_pair(sf::Vector2f(x, y), radius);
				newObstacle.m_collisionCircles.push_back(circle);

				hitcircle = hitcircle->NextSiblingElement("Hitcircle");
			}
			m_obstacleBank.push_back(newObstacle);
		}
	}
}

void Ground::loadPortal(const sf::Texture* tex, in_str portalXml)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(("../images/" + portalXml).c_str());

	Ground::Portal newPortal;

	auto root = doc.FirstChildElement("root");
	if (root)
	{
		auto obst = root->FirstChildElement("Portal");
		if (obst)
		{
			bool animated = obst->BoolAttribute("animated");
			newPortal.m_animated = animated;
			if (animated)
			{
				sf::Int64 frameHeight = obst->IntAttribute("frameHeight");
				int fps = obst->IntAttribute("fps");
				bool loop = obst->BoolAttribute("loop");

				Anim sprite("portal", *tex, frameHeight, fps, loop);
				newPortal.m_sprite = std::make_shared<Anim>(sprite);
				newPortal.m_size = newPortal.m_sprite->getSize();
			}
			else
			{
				newPortal.m_sprite = std::make_shared<Mobile>("portal", *tex);
				newPortal.m_size = newPortal.m_sprite->getSize();
			}


			auto hitbox = obst->FirstChildElement("Hitbox");
			while (hitbox)
			{
				int x = hitbox->IntAttribute("startX");
				int y = hitbox->IntAttribute("startY");
				int width = hitbox->IntAttribute("width");
				int height = hitbox->IntAttribute("height");
				sf::FloatRect hBox(x, y, width, height);
				newPortal.m_collisionRects.push_back(hBox);

				hitbox = hitbox->NextSiblingElement("Hitbox");
			}
			m_portal = std::make_shared<Portal>(newPortal);
			m_portal->m_pos = m_portal->m_origin = { (float)theEngine->getScrW(), (float)theEngine->getScrH() };
		}
	}
}

void Ground::loadHole(std::pair<const sf::Texture*, std::string>& holeSprite)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(("../images/" + holeSprite.second).c_str());

	Ground::HoleInfo newHole;

	auto root = doc.FirstChildElement("root");
	if (root)
	{
		auto obst = root->FirstChildElement("Hole");
		if (obst)
		{
			bool animated = obst->BoolAttribute("animated");
			newHole.m_animated = animated;
			newHole.m_tex = holeSprite.first;
			if (animated)
			{
				sf::Int64 frameHeight = obst->IntAttribute("frameHeight");
				int fps = obst->IntAttribute("fps");
				bool loop = obst->BoolAttribute("loop");

				newHole.m_frameHeight = frameHeight;
				newHole.m_fps = fps;
				newHole.loop = loop;

				newHole.m_size = sf::Vector2f( newHole.m_tex->getSize().x, frameHeight );
			}
			else
			{
				newHole.m_size = sf::Vector2f(newHole.m_tex->getSize());
			}

			float startPos = obst->FloatAttribute("holeStart");
			float endPos = obst->FloatAttribute("holeEnd");
			newHole.m_startPos = startPos;
			newHole.m_endPos = endPos;

			m_holeBank.push_back(newHole);
		}
	}
}

Ground::collisionStatus Ground::checkObstacleCollisions(sf::FloatRect bBox, sf::Vector2f& vel)
{
	auto player = theEngine->getScene()->getPlayer();

	if (theEngine->debugHitboxes)
	{
		sf::RectangleShape hBox;
		hBox.setOutlineColor({ 0,255,0,255 });
		hBox.setFillColor({ 0,0,0,0 });
		hBox.setOutlineThickness(0.5);
		hBox.setPosition(bBox.left, bBox.top);
		hBox.setSize({ bBox.width, bBox.height });
		theEngine->getWindow()->draw(hBox);
	}

	collisionStatus status = NONE;

	for (auto& g : m_groundBlocks)
	{
		for (auto& o : g.m_obstacles)
		{
			collisionStatus temp = NONE;
			if (temp = o.checkCollision(bBox, vel))
				if(temp > status)
					status = temp;
		}
	}
	return status;
}

void Ground::GroundBlock::addObstacle(ObstacleInfo info, sf::Vector2f offset)
{
	if (info.m_floating)
		offset.y -= 40;
	Obstacle newObst;
	newObst.m_floating = info.m_floating;
	if (info.m_animated)
	{
		newObst.m_sprite = std::make_shared<Anim>("anim", *info.m_tex, info.m_frameHeight, info.m_fps, info.loop);
	}
	else
	{
		newObst.m_sprite = std::make_shared<Mobile>("mob", *info.m_tex);
	}
	newObst.m_size = info.m_size;
	newObst.m_collisionCircles = info.m_collisionCircles;
	newObst.m_collisionRects = info.m_collisionRects;
	newObst.m_origin = offset;
	m_obstacles.push_back(newObst);
}

Ground::collisionStatus Ground::Obstacle::checkCollision(sf::FloatRect bBox, sf::Vector2f& vel)
{
	sf::Vector2f centre = { bBox.left + bBox.width / 2, bBox.top + bBox.height / 2 };

	collisionStatus status = NONE;

	for (auto r : m_collisionRects)
	{
		collisionStatus temp = NONE;
		r.left += m_pos.x;
		r.top += m_pos.y;
		if (r.intersects(bBox))
			if (m_isHole)
				return HOLE;
			else if (vel.y > 0 && centre.y < r.top)
				temp =  TOP;
			else if (vel.y <= 0 && centre.y > r.top)
				temp =  BOTTOM;
			else if (vel.x > 0 && centre.x < r.left)
				temp = LEFT;
			else if (vel.x < 0)
				temp = RIGHT;

			if ((int)temp > (int)status)
				status = temp;
	}
	for (auto c : m_collisionCircles)
	{
		c.first += m_pos;

		collisionStatus temp = NONE;

		if (bBox.contains(c.first))
		{
			if (m_isHole)
				return HOLE;
			else if (vel.x > 0 && ((bBox.top + bBox.height) < (c.first.y - c.second / 4)) && (bBox.left + bBox.width < c.first.x))
				temp = ADJUST;
			else if (vel.y > 0 && centre.y < c.first.y - c.second)
				temp = TOP;
			else if (vel.y <= 0 && centre.y > c.first.y - c.second)
				temp = BOTTOM;
			else if (vel.x > 0 && centre.x < c.first.x - c.second)
				temp = LEFT;
			else if (vel.x < 0)
				temp = RIGHT;

			if ((int)temp >(int)status)
				status = temp;
			continue;
		}

		sf::Vector2f pts[] = {
			sf::Vector2f(bBox.left, bBox.top),
			sf::Vector2f(bBox.left + bBox.width, bBox.top),
			sf::Vector2f(bBox.left + bBox.width, bBox.top + bBox.height),
			sf::Vector2f(bBox.left, bBox.top + bBox.height)
		};

		for (size_t i = 0; i < 4; i++)
		{
			auto& pt = pts[i];
			float dist = sqrt(pow((c.first.x - pt.x), 2) + pow((c.first.y - pt.y), 2));
			if (dist <= c.second)
			{
				if (m_isHole)
					return HOLE;
				else if (vel.x > 0 && ((bBox.top + bBox.height) < (c.first.y - c.second / 2)) && (bBox.left + bBox.width < c.first.x))
					temp = ADJUST;
				else if (vel.y > 0 && centre.y < c.first.y - c.second)
					temp = TOP;
				else if (vel.y <= 0 && centre.y > c.first.y - c.second)
					temp = BOTTOM;
				else if (vel.x > 0 && centre.x < c.first.x - c.second)
					temp = LEFT;
				else if (vel.x < 0)
					temp = RIGHT;

				if ((int)temp >(int)status)
					status = temp;
				continue;
			}

			size_t b = i + 1;
			if (b >= 4)
				b = 0;

			if (Engine::distanceToLine(pt, pts[b], c.first) <= c.second)
			{
				if (m_isHole)
					return HOLE;
				else if (vel.x > 0 && ((bBox.top + bBox.height) < (c.first.y - c.second / 2)) && (bBox.left + bBox.width < c.first.x))
					temp = ADJUST;
				else if (vel.y > 0 && centre.y < c.first.y - c.second)
					temp = TOP;
				else if (vel.y <= 0 && centre.y > c.first.y - c.second)
					temp = BOTTOM;
				else if (vel.x > 0 && centre.x < c.first.x - c.second)
					temp = LEFT;
				else if (vel.x < 0)
					temp = RIGHT;

				if ((int)temp >(int)status)
					status = temp;
				continue;
			}
				
		}

	}

	return status;
}

bool Ground::Coin::checkCollision(sf::FloatRect bBox)
{
	m_bounds.left = m_pos.x;
	m_bounds.top = m_pos.y;
	if (m_bounds.intersects(bBox) && !m_collected)
	{
		m_sprite->setColor({ 0,0,0,0 });
		theEngine->getScene()->getCoinCounter()->collectCoin();
		m_collected = true;
		m_coinSound.play();
		return true;
	}
	return false;
}
