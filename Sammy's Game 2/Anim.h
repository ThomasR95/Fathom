#pragma once
#include "tSprite.h"
#include "typedefs.h"

struct AnimCfg
{
	AnimCfg() {};
	AnimCfg(sf::Texture* tex_, int frames_, int fps_, int rows_, int cols_, sf::Vector2f pos_ = { 0,0 }, tSprite::origin org_ = tSprite::TOP_LEFT, bool loop_ = true, float delay_ = 0, sf::Vector2f size_ = { -1,-1 })
	:	tex(tex_), 
		pos(pos_),
		org(org_),
		frames(frames_),
		fps(fps_),
		rows(rows_),
		cols(cols_),
		loop(loop_),
		delay(delay_),
		size(size_)
		{}

	sf::Texture* tex;
	sf::Vector2f pos = { 0,0 };
	int frames;
	int fps = 12;
	int rows;
	int cols;
	sf::Vector2f src = { 0,0 };
	sf::Vector2f size = { -1,-1 };
	tSprite::origin org = tSprite::TOP_LEFT;
	bool loop = true;
	float delay = 0;
};

class Anim : public tSprite
{
public:
	Anim() {}
	Anim(sf::Texture& tex, _IN_VEC2 pos, int frames, int rows, int cols, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 }, origin org = TOP_LEFT, float fps = 12, bool loop = true, float delay = 0);
	Anim(const AnimCfg& cfg);
	~Anim();

	void tick(sf::Time& dt) override;

	void restart() 
	{ 
		m_currentFrame = 0; 
		m_finished = false; 
		m_playing = true; 
		m_size = sf::Vector2f((float)m_frames[m_currentFrame].width, (float)m_frames[m_currentFrame].height);
		sprite.setTextureRect(m_frames[m_currentFrame]);
		m_delayLeft = sf::seconds(m_delay);
	}

	void setFPS(sf::Int64 fps) {
		m_fps = (float)fps;
		m_frameTime = sf::seconds(1) / fps;
	}

	float getFPS() { return m_fps; }

	bool isFinished() { return m_finished; }

	int getCurrentFrame() { return m_currentFrame; }

	void pause() { m_playing = false; }

	void play() { m_playing = true; }

	void setFrame(int f) { m_currentFrame = f; }
	int getFrame() { return m_currentFrame; }
	int getNofFrames() { return m_frames.size(); }
	void setLoop(bool loop) { m_looping = loop; }

	void setPauseFrame(int frame, bool alternate) { m_pauseFrame = frame; m_alternatePaused = alternate; }

	bool isDelayed() { return m_delayLeft > sf::seconds(0); }

	void setSpeed(float speed) { m_speed = speed; }

	bool isAnimated() { return true; }

	sf::Vector2f getSize() { return m_size; }

	void setOrigin(tSprite::origin org);

	virtual int getNumEffectors() { return m_effectors.size(); }
	virtual void addEffector(const tEffector& eff) { m_effectors.push_back(eff); }
	virtual void clearEffectors() { m_effectors.clear(); }

protected:

	std::vector<tEffector> m_effectors;


	sf::Vector2f m_size;
	float m_speed = 1.0;
	float m_delay;
	sf::Time m_delayLeft;
	int m_currentFrame;
	std::vector<sf::IntRect> m_frames;
	int m_frameHeight;
	int m_frameCount;
	float m_fps;
	sf::Time m_frameTime;
	sf::Time m_timeElapsed;
	bool m_looping;
	bool m_finished = false;
	bool m_playing = true;
	int m_pauseFrame = -1;
	bool m_alternatePaused = false;
};

