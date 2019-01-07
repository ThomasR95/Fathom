#include "typedefs.h"

#include "resourceManager.h"

std::shared_ptr<ResMan> resMan;

ResMan::ResMan()
{
}


ResMan::~ResMan()
{
	//m_musics.clear();
	//m_soundbuffers.clear();
	//m_textures.clear();
	//m_sprites.clear();
}

sf::Texture* ResMan::getTexture(_IN_STR _path)
{
	auto path = _path;
	if (path.find("../images/") != std::string::npos)
		path.erase(0, 10);

	if (path.find(".png") == std::string::npos && path.find(".jpg") == std::string::npos)
		return nullptr;

	if (m_textures.find(path) == m_textures.end())
	{
		sf::Texture newTex;
		if (newTex.loadFromFile("../images/" + path))
		{
			newTex.setSmooth(true);
			m_textures[path] = newTex;
		}
		else
			return nullptr;
			
	}
	return &m_textures[path];
}

void ResMan::unloadTexture(const sf::Texture * ptr)
{
	for(auto& t : m_textures)
		if ((&t.second) == ptr)
		{
			m_textures.erase(t.first);
			break;
		}
}

sf::SoundBuffer * ResMan::getSoundBuffer(_IN_STR _path)
{
	if (m_soundbuffers.find(_path) == m_soundbuffers.end())
	{
		sf::SoundBuffer buffer;
		if (buffer.loadFromFile("../sounds/" + _path))
		{
			m_soundbuffers[_path] = buffer;
		}
		else
			return nullptr;
	}
	return &m_soundbuffers[_path];
}

sf::Sound * ResMan::getSound(_IN_STR _path)
{
	if (m_sounds.count(_path))
		return &m_sounds[_path];

	auto sb = getSoundBuffer(_path);
	if (sb)
	{
		m_sounds[_path].setBuffer(*sb);
		return &m_sounds[_path];
	}
		
	return nullptr;
}

sf::Music * ResMan::getMusic(_IN_STR _path)
{
	if (m_musics.find(_path) == m_musics.end())
	{
		sf::Music music;
		if (!m_musics[_path].openFromFile("../sounds/" + _path))
			return nullptr;
	}
	return &m_musics[_path];
}

tSprite * ResMan::addSprite(_IN_STR id, _IN_STR texture, _IN_VEC2 pos, tSprite::origin org, _IN_VEC2 src, _IN_VEC2 size)
{
	if (!m_sprites.count(id))
	{
		m_sprites[id] = tSprite(*getTexture(texture), pos, src, size, org);
		m_sprites[id].m_UID = id;
		return &m_sprites[id];
	}
	return nullptr;
}

tSprite * ResMan::getSprite(_IN_STR id)
{
	if (m_sprites.count(id))
	{
		return &m_sprites[id];
	}
	return nullptr;
}

void ResMan::clear()
{
	m_musics.clear();
	m_soundbuffers.clear();
	m_sounds.clear();
	m_textures.clear();
	m_sprites.clear();
}
