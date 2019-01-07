#include "Anim.h"



Anim::Anim(sf::Texture& tex, _IN_VEC2 pos, int frames, int rows, int cols, _IN_VEC2 src, _IN_VEC2 size, origin org, float fps, bool loop, float delay) : tSprite(tex,pos,src,size,org)
{
	m_fps = fps;
	m_frameTime = sf::seconds(1) / fps;

	m_currentFrame = 0;

	auto tsize = sprite.getTexture()->getSize();

	float fHeight;
	float fWidth;
	if (size == sf::Vector2f{ -1, -1 })
	{
		fHeight = tsize.y / rows;
		fWidth = tsize.x / cols;
	}
	else
	{
		fHeight = size.y;
		fWidth = size.x;
	}

	m_frameCount = frames;

	int framesAdded = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (framesAdded >= m_frameCount)
				break;

			sf::IntRect frame;
			frame.height = fHeight;
			frame.width = fWidth;
			frame.left = fWidth*col;
			frame.top = fHeight*row;

			m_frames.push_back(frame);

			framesAdded++;
		}
	}
	m_size = { fWidth, fHeight };

	sprite.setTextureRect(m_frames[0]);

	m_looping = loop;
	m_finished = false;

	m_delay = delay;
	m_delayLeft = sf::seconds(m_delay);

	setOrigin(org);
}

Anim::Anim(const AnimCfg & cfg) : tSprite(*cfg.tex, cfg.pos, cfg.src, cfg.size, cfg.org)
{
	m_fps = cfg.fps;
	m_frameTime = sf::seconds(1) / m_fps;

	m_currentFrame = 0;

	auto tsize = sprite.getTexture()->getSize();

	float fHeight;
	float fWidth;
	if (cfg.size == sf::Vector2f{-1, -1})
	{
		fHeight = tsize.y / cfg.rows;
		fWidth = tsize.x / cfg.cols;
	}
	else
	{
		fHeight = cfg.size.y;
		fWidth = cfg.size.x;
	}
	m_size = { fWidth, fHeight };
	m_frameCount = cfg.frames;

	int framesAdded = 0;
	for (int row = 0; row < cfg.rows; row++)
	{
		for (int col = 0; col < cfg.cols; col++)
		{
			if (framesAdded >= m_frameCount)
				break;

			sf::IntRect frame;
			frame.height = fHeight;
			frame.width = fWidth;
			frame.left = fWidth*col;
			frame.top = fHeight*row;

			m_frames.push_back(frame);

			framesAdded++;
		}
	}

	sprite.setTextureRect(m_frames[0]);


	m_looping = cfg.loop;
	m_finished = false;

	m_delay = cfg.delay;
	m_delayLeft = sf::seconds(m_delay);

	setOrigin(cfg.org);
}


Anim::~Anim()
{
}

void Anim::tick(sf::Time & dt)
{
	if (m_effectors.size())
	{
		if (m_effectors[0].applyEffects(&sprite))
			m_effectors.erase(m_effectors.begin());
	}


	if (m_frameCount <= 1)
		return;

	m_delayLeft -= dt;
	if (m_delayLeft <= sf::seconds(0))
	{
		m_timeElapsed += dt * m_speed;
		if (m_timeElapsed.asSeconds() > m_frameTime.asSeconds() * m_frameCount)
			m_timeElapsed = sf::seconds(0);

		while (m_timeElapsed > m_frameTime)
		{
			m_timeElapsed -= m_frameTime;
			if (m_playing)
			{
				m_currentFrame++;
				if (m_pauseFrame >= 0 && m_currentFrame == m_pauseFrame)
				{
					pause();
				}
				if (m_currentFrame > (m_frameCount - 1))
				{
					m_currentFrame = 0;
					if (!m_looping)
					{
						m_finished = true;
					}
				}
				if (!m_finished)
				{
					m_size = sf::Vector2f((float)m_frames[m_currentFrame].width, (float)m_frames[m_currentFrame].height);
					sprite.setTextureRect(m_frames[m_currentFrame]);
				}
			}
			else if (!m_finished && m_alternatePaused)
			{
				if (m_currentFrame == m_pauseFrame)
					m_currentFrame--;
				else
					m_currentFrame++;
				m_size = sf::Vector2f((float)m_frames[m_currentFrame].width, (float)m_frames[m_currentFrame].height);
				sprite.setTextureRect(m_frames[m_currentFrame]);
			}
		}
	}
}

void Anim::setOrigin(tSprite::origin org)
{
	switch (org)
	{
	case tSprite::TOP_LEFT:
		sprite.setOrigin({ 0,0 });
		break;
	case tSprite::TOP_CENTRE:
		sprite.setOrigin({ m_size.x / 2, 0 });
		break;
	case tSprite::TOP_RIGHT:
		sprite.setOrigin({ m_size.x, 0 });
		break;
	case tSprite::CENTRE_LEFT:
		sprite.setOrigin({ 0,m_size.y / 2 });
		break;
	case tSprite::CENTRE_CENTRE:
		sprite.setOrigin(m_size / 2.f);
		break;
	case tSprite::CENTRE_RIGHT:
		sprite.setOrigin({ m_size.x,m_size.y / 2 });
		break;
	case tSprite::BOTTOM_LEFT:
		sprite.setOrigin({ 0,m_size.y });
		break;
	case tSprite::BOTTOM_CENTRE:
		sprite.setOrigin({ m_size.x / 2, m_size.y });
		break;
	case tSprite::BOTTOM_RIGHT:
		sprite.setOrigin(m_size);
		break;
	default:
		break;
	}
}
