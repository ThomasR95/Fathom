#include "Scene.h"
#include "engine.h"
#include "resourceManager.h"
#include "Ground.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::create(in_str backgroundFile, in_str groundPath)
{

	m_gravity = 500;

	m_groundHeight = 265;

	auto bg = std::make_shared<Anim>("Background", *resMan->getTexture(backgroundFile), 300, 12);
	m_background = bg;
	theEngine->addSprite(bg);

	m_coinCounter = std::make_shared<CoinCounter>();
	m_coinCounter->setBarSprite(*resMan->getTexture("Fuel bar/fuel bar filling.png"));
	m_coinCounter->setHolderSprite(*resMan->getTexture("Fuel bar/fuel bar.png"));
	m_coinCounter->setMaxCoins(50);

	Ground ground("Ground");
	int f = 1;
	sf::Texture* tex = nullptr;
	while (tex = resMan->getTexture(groundPath + "ground_"+std::to_string(f++)+".png"))
	{
		ground.addSprite(tex);
	}
	f = 1;
	ground.addHoleSprite(resMan->getTexture(groundPath + "hole_1.png"), groundPath + "hole_1.xml");
	ground.addHoleSprite(resMan->getTexture(groundPath + "hole_2.png"), groundPath + "hole_2.xml");
	ground.addHoleSprite(resMan->getTexture(groundPath + "hole_3.png"), groundPath + "hole_3.xml");
	ground.addHoleSprite(resMan->getTexture(groundPath + "hole_4.png"), groundPath + "hole_4.xml");
	while (tex = resMan->getTexture(groundPath + "obstacle_" + std::to_string(f) + ".png"))
	{
		ground.addObstacleSprite(tex, groundPath + "obstacle_" + std::to_string(f) + ".xml");
		f++;
	}
	ground.setBackground(Anim("background", *resMan->getTexture("anims/Background_anim.png"), 300));
	ground.setCoinSprite(std::make_shared<Anim>(Anim("orb", *resMan->getTexture("anims/energy_orb.png"), 204.f / 12.f, 12, true)));
	ground.loadPortal(resMan->getTexture("foreground/End_portal.png"), "foreground/End_portal.xml");
	ground.setCoinSound(resMan->getSoundBuffer("orb collect.ogg"));

	Anim bug("bug", *resMan->getTexture("anims/bug.png"), 20);
	Anim grass("grass", *resMan->getTexture("anims/grass.png"), 52, 6);
	ground.addDetail(std::make_shared<Anim>(bug));
	ground.addDetail(std::make_shared<Anim>(grass));
	m_ground = std::make_shared<Ground>(ground);

	m_player = std::make_shared<Player>("Player");
	m_player->setVelocity({ 0,0 });
	Anim run("char_run", *resMan->getTexture("Anims/char_run.png"), 36, 24);
	m_player->setRunAnim(run);
	Anim idle("char_idle", *resMan->getTexture("Anims/char_idle.png"), 37, 12);
	m_player->setIdleAnim(idle);
	Anim jump("char_jump", *resMan->getTexture("Anims/char_jump.png"), 36, 12, false);
	m_player->setJumpAnim(jump);
	m_player->setLandSound(resMan->getSoundBuffer("Jump landing.ogg"));
	Anim dead("char_death", *resMan->getTexture("Anims/char_death.png"), 36, 12, false, 0.5);
	m_player->setDeathAnim(dead);
	m_player->setDeathSound(resMan->getSoundBuffer("Death grunt.ogg"));
	Anim slide("char_slide", *resMan->getTexture("Anims/char_slide.png"), 36, 12, false);
	slide.setPauseFrame(7, true);
	m_player->setSlideAnim(slide);
	m_player->setSlideSound(resMan->getSoundBuffer("Skid sound.ogg"));
	m_player->setPortalSound(resMan->getSoundBuffer("Through the portal.ogg"));

	m_player->setPosition({ 150, (float)m_groundHeight - m_player->getSize().y});
	

	m_monster = std::make_shared<Monster>("Monster");
	m_monster->setPosition({ -100, (float)m_groundHeight - 111 });
	Anim monsterRun("monster_running", *resMan->getTexture("Anims/monster_running.png"), 111, 24);
	m_monster->setRunAnim(monsterRun);
	Anim attack("monster_attack", *resMan->getTexture("Anims/monster_attack.png"), 111, 12, false);
	m_monster->setAttackAnim(attack);
	m_monster->setRoarSound(resMan->getSoundBuffer("Monster noise.ogg"));
	m_monster->setWalkSound(resMan->getMusic("Monster running.ogg"));

	m_birdAnim = std::make_shared<Anim>("bird", *resMan->getTexture("anims/Bird.png"), 22, 24);

	theEngine->addSprite(m_ground);
	theEngine->addSprite(m_player);
	theEngine->addSprite(m_monster);
	
	theEngine->addSprite(m_coinCounter);

	m_player->addVelocity({ 80, 0 });
}

void Scene::tick(sf::Time & dt)
{
	if (m_player.get())
	{
		if (dt + m_timer >= sf::seconds(1))
		{
			if (m_player->getVelocity().x >= 50 && m_player->getVelocity().x < 150)
				m_player->addVelocity({ 1,0 });
			m_timer += dt - sf::seconds(1);


			int bird = rand() % 500;
			if (bird < 50)
			{
				Anim* bird = nullptr;
				for (auto& b : m_birds)
				{
					if (b->getPosition().x < 0 - b->getSize().x)
					{
						bird = b.get();
						break;
					}	
				}
				if (!bird)
				{
					m_birds.push_back(std::make_shared<Anim>(*m_birdAnim.get()));
					bird = m_birds.back().get();
					theEngine->addSprite(m_birds.back());
				}

				int extraHeight = rand() % 50;
				bird->setPosition({ (float)theEngine->getScrW(), 20.f + extraHeight });
			}
				
		}
		else
			m_timer += dt;
	}
	for (auto& b : m_birds)
	{
		auto pos = b->getPosition();
		if (pos.x > -100)
		{
			pos.x -= (80 + m_player->getVelocity().x) * dt.asSeconds();
			b->setPosition(pos);
		}
		
	}


}
