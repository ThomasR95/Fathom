#include "tInterpolator.h"
#include <math.h>


tInterpolator::tInterpolator()
{
}


tInterpolator::~tInterpolator()
{
}

float tInterpolator::interpolate(float first, float last, float fraction)
{
	float out = first;

	float diff = last - first;

	switch (m_type)
	{
	case tInterpolator::LINEAR:
		out = first + diff*fraction;
		break;
	case tInterpolator::QUADRATIC_UP:
	{
		float y = 2.f * (3.f * (float)pow(fraction * 0.5, 2) - 2.f * (float)pow(fraction * 0.5, 3));
		out = first + diff*y;
		break;
	}
	case tInterpolator::QUADRATIC_DOWN:
	{
		float y = 2.f * (3.f * (float)pow((fraction + 1) * 0.5, 2) - 2.f * (float)pow((fraction + 1) * 0.5, 3)) - 1.f;
		out = first + diff*y;
		break;
	}
	case tInterpolator::QUADRATIC_UP_DOWN:
	{
		float y = 3.f * (float)pow(fraction, 2) - 2.f * (float)pow(fraction, 3);
		out = first + diff*y;

		break;
	}
	default:
		break;
	}


	return out;
}
