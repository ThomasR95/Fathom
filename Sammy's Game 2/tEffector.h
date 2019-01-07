#pragma once

#include "SFML\Graphics.hpp"
#include "tInterpolator.h"

class tSprite;

class tEffector
{
public:

	struct transforms
	{
		transforms() {}
		transforms(const sf::Vector2f& pos, float rot, const sf::Vector2f& sca, const sf::Color& col) : position(pos), rotation(rot), scale(sca), colour(col) {}

		sf::Vector2f	position;
		float			rotation = 0.f;
		sf::Vector2f	scale;
		sf::Color		colour;

		transforms operator+(transforms& rhs)
		{
			return transforms(position + rhs.position, rotation + rhs.rotation, scale + rhs.scale, colour + rhs.colour);
		}

		transforms& operator+=(transforms& rhs)
		{
			position += rhs.position;
			rotation += rhs.rotation;
			scale += rhs.scale;
			colour += rhs.colour;

			return (*this);
		}
	};

	struct activity
	{
		bool position = false;
		bool rotation = false;
		bool scale = false;
		bool colour = false;
	};

	struct times
	{
		int position = 0;
		int rotation = 0;
		int scale = 0;
		int colour = 0;

		int getMax()
		{
			int max = 0;
			if (position > max)
				max = position;
			if (rotation > max)
				max = rotation;
			if (scale > max)
				max = scale;
			if (colour > max)
				max = colour;

			return max;
		}
	};

	enum mType
	{
		OFFSET,
		POINT_TO_POINT
	};

	tEffector(transforms t, activity a, int time, tInterpolator::type interp = tInterpolator::type::QUADRATIC_UP_DOWN, mType type = POINT_TO_POINT);
	tEffector(transforms t, activity a, times time, tInterpolator::type interp = tInterpolator::type::QUADRATIC_UP_DOWN, mType type = POINT_TO_POINT);
	tEffector(transforms t, activity a, int time, times delay, tInterpolator::type interp = tInterpolator::type::QUADRATIC_UP_DOWN, mType type = POINT_TO_POINT);
	tEffector(transforms t, activity a, times time, times delay, tInterpolator::type interp = tInterpolator::type::QUADRATIC_UP_DOWN, mType type = POINT_TO_POINT);
	~tEffector();

	bool applyEffects(sf::Sprite* obj);

private:

	transforms m_start;
	transforms m_end;
	transforms m_input;

	activity m_activity;

	times m_times;
	times m_delays;
	tInterpolator m_interp;
	mType m_type;

	sf::Clock m_timer;

	bool m_first;

	int m_maxTime = 0;

	bool m_finished = false;
};

