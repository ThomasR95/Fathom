#pragma once
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tSprite.h"

class ResMan
{
public:
	ResMan();
	~ResMan();

	sf::Texture* getTexture(_IN_STR _path);

	void unloadTexture(const sf::Texture* ptr);

	sf::SoundBuffer* getSoundBuffer(_IN_STR _path);
	sf::Sound* getSound(_IN_STR _path);
	sf::Music* getMusic(_IN_STR  _path);

	tSprite* addSprite(_IN_STR id, _IN_STR texture, _IN_VEC2 pos = { 0,0 }, tSprite::origin org = tSprite::origin::TOP_LEFT, _IN_VEC2 src = { 0,0 }, _IN_VEC2 size = { -1,-1 });
	tSprite* getSprite(_IN_STR id);

	void clear();

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, sf::SoundBuffer> m_soundbuffers;
	std::map<std::string, sf::Sound> m_sounds;
	std::map<std::string, sf::Music> m_musics;
	std::map<std::string, tSprite> m_sprites;
};

extern std::shared_ptr<ResMan> resMan;