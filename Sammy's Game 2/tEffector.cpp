#include "tEffector.h"
#include "tSprite.h"

tEffector::tEffector(transforms t, activity a, int time, tInterpolator::type interp, mType type)
{
	m_input = t;
	m_activity = a;
	m_times.colour = time;
	m_times.position = time;
	m_times.rotation = time;
	m_times.scale = time;

	m_maxTime = time;

	m_interp.setType(interp);
}

tEffector::tEffector(transforms t, activity a, times time, tInterpolator::type interp, mType type)
{
	m_input = t;
	m_activity = a;
	m_times = time;

	m_maxTime = m_times.getMax();

	m_interp.setType(interp);
}

tEffector::tEffector(transforms t, activity a, int time, times delay, tInterpolator::type interp, mType type)
{
	m_input = t;
	m_activity = a;
	m_times.colour = time;
	m_times.position = time;
	m_times.rotation = time;
	m_times.scale = time;

	m_delays = delay;

	m_maxTime = time + m_delays.getMax();

	m_interp.setType(interp);
}

tEffector::tEffector(transforms t, activity a, times time, times delay, tInterpolator::type interp, mType type)
{
	m_input = t;
	m_activity = a;
	m_times = time;
	m_delays = delay;

	m_maxTime = m_times.getMax() + m_delays.getMax();

	m_interp.setType(interp);
}


tEffector::~tEffector()
{
}

bool tEffector::applyEffects(sf::Sprite * obj)
{
	if (m_first)
	{
		m_end = m_input;
		m_start.colour = obj->getColor();
		m_start.position = obj->getPosition();
		m_start.rotation = obj->getRotation();
		m_start.scale = obj->getScale();
		if (m_type == OFFSET)
			m_end += m_start;

		m_first = false;

		m_timer.restart();
	}

	int elapsed = m_timer.getElapsedTime().asMilliseconds();

	if (elapsed >= m_maxTime)
	{
		if (m_activity.position)
		{
			obj->setPosition(m_end.position);
		}
		if (m_activity.rotation)
		{
			obj->setRotation(m_end.rotation);
		}
		if (m_activity.scale)
		{
			obj->setScale(m_end.scale);
		}
		if (m_activity.colour)
		{
			obj->setColor(m_end.colour);
		}
		return true;
	}

	if (m_activity.position)
	{
		float time = (float)elapsed - m_delays.position;
		if (time > 0)
		{
			float fraction = time / m_times.position;
			if (fraction >= 0 && fraction <= 1)
			{
				float x = m_interp.interpolate(m_start.position.x, m_end.position.x, fraction);
				float y = m_interp.interpolate(m_start.position.y, m_end.position.y, fraction);
				obj->setPosition({ x, y });
			}
			else if(fraction > 1)
				obj->setPosition(m_end.position);
		}
	}
	if (m_activity.rotation)
	{
		float time = (float)elapsed - m_delays.rotation;
		if (time > 0)
		{
			float fraction = time / m_times.rotation;
			if (fraction >= 0 && fraction <= 1)
			{
				float r = m_interp.interpolate(m_start.rotation, m_end.rotation, fraction);
				obj->setRotation(r);
			}
			else if (fraction > 1)
				obj->setRotation(m_end.rotation);
		}
	}
	if (m_activity.scale)
	{
		float time = (float)elapsed - m_delays.scale;
		if (time > 0)
		{
			float fraction = time / m_times.scale;
			if (fraction >= 0 && fraction <= 1)
			{
				float x = m_interp.interpolate(m_start.scale.x, m_end.scale.x, fraction);
				float y = m_interp.interpolate(m_start.scale.y, m_end.scale.y, fraction);
				obj->setScale({ x, y });
			}
			else if (fraction > 1)
				obj->setScale(m_end.scale);
		}
	}
	if (m_activity.colour)
	{
		float time = (float)elapsed - m_delays.colour;
		if (time > 0)
		{
			float fraction = time / m_times.colour;
			if (fraction >= 0 && fraction <= 1)
			{
				sf::Uint8 r = (sf::Uint8)m_interp.interpolate(m_start.colour.r, m_end.colour.r, fraction);
				sf::Uint8 g = (sf::Uint8)m_interp.interpolate(m_start.colour.g, m_end.colour.g, fraction);
				sf::Uint8 b = (sf::Uint8)m_interp.interpolate(m_start.colour.b, m_end.colour.b, fraction);
				sf::Uint8 a = (sf::Uint8)m_interp.interpolate(m_start.colour.a, m_end.colour.a, fraction);
				obj->setColor({ r,g,b,a });
			}
			else if (fraction > 1)
				obj->setColor(m_end.colour);
		}
	}
		

	return false;

}
