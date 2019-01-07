#pragma once
class tInterpolator
{
public:
	enum type
	{
		LINEAR,
		QUADRATIC_UP,
		QUADRATIC_DOWN,
		QUADRATIC_UP_DOWN
	};

	tInterpolator();
	tInterpolator(type type_) { m_type = type_; }
	~tInterpolator();

	float interpolate(float first, float last, float fraction);

	void setType(type type_) { m_type = type_; }

private:

	type m_type = LINEAR;
};

