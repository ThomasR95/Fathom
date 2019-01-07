#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "GfxBase.h"
#include "tText.h"
#include "GameStates.h"
#include "Dialog.h"

class World;

class Obstacle;

class tButton;

class Player;

class tCursor;


float angleBetween(_IN_VEC2 a, _IN_VEC2 b);

float len(_IN_VEC2 a);

sf::Vector2f norm(_IN_VEC2 a);

int randInt(int from, int lessThan);

class Engine
{
public:
	Engine() {}
	~Engine();

	void init(const sf::Vector2i& size = { 1080, 720 }, const sf::Vector2i& view = { 3840, 2160 });
	void handleEvents();
	void render();
	void manageStates();
	void close();

	int getScrW();
	int getScrH();

	void addtoRenderList(GfxBase* _sprite);

	sf::RenderWindow* getWindow() { return &m_window; }

	sf::Vector2f stringToVec2(std::string& coord);

	static float distanceToLine(sf::Vector2f a, sf::Vector2f b, sf::Vector2f point)
	{
		return distanceToLine(a.x, a.y, b.x, b.y, point.x, point.y);
	}

	static float distanceToLine(float x1, float y1, float x2, float y2, float pointX, float pointY)
	{
		float diffX = x2 - x1;
		float diffY = y2 - y1;
		if ((diffX == 0) && (diffY == 0))
		{
			diffX = pointX - x1;
			diffY = pointY - y1;
			return sqrt(diffX * diffX + diffY * diffY);
		}

		float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);

		if (t < 0)
		{
			//point is nearest to the first point i.e x1 and y1
			diffX = pointX - x1;
			diffY = pointY - y1;
		}
		else if (t > 1)
		{
			//point is nearest to the end point i.e x2 and y2
			diffX = pointX - x2;
			diffY = pointY - y2;
		}
		else
		{
			//if perpendicular line intersect the line segment.
			diffX = pointX - (x1 + t * diffX);
			diffY = pointY - (y1 + t * diffY);
		}

		//returning shortest distance
		return sqrt(diffX * diffX + diffY * diffY);
	}

	GameStates States;

	void setNextState(_IN_STR state) { States.nextState = state; }
	void exitState() { States.signalExit(); }

	void clearRenderList() { m_renderList.clear(); }
	void removeFromRenderList(GfxBase* item);

	void setLoadingScreen(sf::Sprite& s) { m_loadingScreen = s; }
	void showLoadingText(_IN_STR string, bool clear = true);
	void hideLoadingText();

	sf::Vector2f getViewScale();

	sf::Vector2f getMousePos();
	sf::Vector2f worldToScreenCoords(_IN_VEC2);

	void sortRenderList();

	struct {
		bool operator()(GfxBase * a, GfxBase * b)
		{
			if (a->Zlevel > b->Zlevel)
				return true;
			return false;
		}
	} ZLessThan;

	void clearCollisionList() { m_collisionList.clear(); }
	void addCollisionObject(GfxBase* addThis) { m_collisionList.push_back(addThis); }
	void removeCollisionObject(GfxBase* removeThis)
	{
		for(auto c_it = m_collisionList.begin(); c_it != m_collisionList.end(); c_it++)
			if ((*c_it) == removeThis)
			{
				m_collisionList.erase(c_it);
				return;
			}
	}

	void clearButtonList() { m_buttons.clear(); }
	void addButton(tButton* addThis) { m_buttons.push_back(addThis); }
	void removeButton(tButton* removeThis)
	{
		for (auto b_it = m_buttons.begin(); b_it != m_buttons.end(); b_it++)
			if ((*b_it) == removeThis)
			{
				m_buttons.erase(b_it);
				return;
			}
	}

	bool testCollision(sf::Vector2f& pos, sf::Vector2f& vel, GfxBase* actor);

	void checkPlayerBehind(_IN_VEC2 pos, int& Z);

	void setCurrentWorld(World* curr) { m_currentWorld = curr; }
	World* getCurrentWorld() { return m_currentWorld; }
	sf::Vector2f checkWorldGateways(_IN_VEC2 charPos);

	void debugViewer(sf::Time& dt);

	void setPlayer(Player* p) { m_player = p; }
	Player* getPlayer() { return m_player; }

	std::shared_ptr<Dialog> dialog;

	void inhibitPlayerMovement(bool inhibit) { m_playerMoveInhibited = inhibit; }

	sf::Font* getFont(unsigned idx) { if (m_fonts.size() >= idx) return &m_fonts[idx]; return nullptr; }

	void setCursor(tCursor* c) { m_cursor = c; }
	tCursor* getCursor() { return m_cursor; }

	void enableMouse(bool enable) { m_mouseEnabled = enable; }

private:

	tCursor* m_cursor;

	Player* m_player;
	bool m_playerMoveInhibited = false;
	bool m_mouseEnabled = true;

	sf::Vector2i m_screenSize;
	float m_leftOffset = 0;

	sf::RectangleShape m_leftBlackout;
	sf::RectangleShape m_rightBlackout;

	sf::Sprite m_loadingScreen;
	tText m_loadingText;

	//sf::Sprite			m_renderSprite;
	//sf::RenderTexture	m_renderTexture;
	sf::RenderWindow	m_window;
	sf::RenderTarget* m_renderTarget;
	//bool m_usingRenderTexture;

	std::vector<GfxBase*> m_renderList;

	std::list<GfxBase*> m_collisionList;

	std::list<tButton*> m_buttons;

	World* m_currentWorld;

	sf::Clock m_tickClock;

	std::vector<sf::Font> m_fonts;

	bool m_debugViewActive = false;
	bool m_awaitingDebugRender = false;
	bool dbv_showVisibleOnly = false;
	bool dbv_stealTick = false;
	sf::Clock dbv_switchClock;

};

extern std::shared_ptr<Engine> theEngine;

