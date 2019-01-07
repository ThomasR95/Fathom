#include "Monster.h"
#include "engine.h"


Monster::Monster(in_str id) : Mobile(id)
{
}


Monster::~Monster()
{
}

void Monster::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	auto w = theEngine->getWindow();

	if (m_caughtUp)
		w->draw(*m_attackAnim.get());
	else
		w->draw(*m_runningAnim.get());
}


void Monster::tick(sf::Time & dt)
{
	m_pos = getPosition();


	auto player = theEngine->getScene()->getPlayer();
	if (!player->isDead())
	{
		if (m_catchUp)
		{
			m_targetPos = m_pos;
			m_targetPos.x = -30 - player->getVelocity().x;

		}
		else
		{
			m_targetPos = m_pos;
			m_targetPos.x = -m_size.x;
		}
	}
	


	float vel = m_targetPos.x - m_pos.x;
	float speed = (player->getVelocity().x + vel) / 100;
	if (speed > 1) speed = 1;
	m_runningAnim->setSpeed(speed);

	m_pos.x += vel * dt.asSeconds()*2;
	m_bounds.left = m_pos.x;
	m_bounds.top = m_pos.y;

	if (speed > 0 && m_walkSound->getStatus() != sf::Music::Playing)
	{
		m_walkSound->play();
	}
	else if (speed <= 0)
	{
		m_walkSound->stop();
	}
	float volume = (m_pos.x + m_size.x) / 4;
	if (volume > 50) volume = 50;
	if (volume < 0) volume = 0;
	if (speed >= 0)
	{
		volume *= speed;
	}
	m_walkSound->setVolume(volume);
	
	if (theEngine->debugHitboxes)
	{
		sf::RectangleShape hBox;
		hBox.setOutlineColor({ 0,255,0,255 });
		hBox.setFillColor({ 0,0,0,0 });
		hBox.setOutlineThickness(0.5);
		hBox.setPosition(m_bounds.left, m_bounds.top);
		hBox.setSize({ m_bounds.width, m_bounds.height });
		theEngine->getWindow()->draw(hBox);
	}

	if (m_bounds.intersects(player->getBounds()) && !m_caughtUp)
	{
		if (m_roarPlayable && m_roarSound.getStatus() != sf::Sound::Playing)
		{
			m_roarSound.play();
			m_roarPlayable = false;
		}
		
		m_caughtUp = true;
		m_attackAnim->restart();
		player->die();
	}
		

	setPosition(m_pos);

	if (m_runningAnim.get())
		m_runningAnim->setPosition(getPosition());
	if (m_attackAnim.get())
		m_attackAnim->setPosition(getPosition());

	if (m_runningAnim.get())
		m_runningAnim->tick(dt);
	if (m_attackAnim.get())
		m_attackAnim->tick(dt);


}