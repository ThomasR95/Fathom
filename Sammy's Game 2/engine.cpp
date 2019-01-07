#include "typedefs.h"

#include "engine.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "Obstacle.h"
#include "imgui\imgui.h"
#include "imgui-sfml\imgui-SFML.h"
#include <iostream>
#include "tButton.h"
#include "Dialog.h"
#include "tCursor.h"
#include <random>

std::shared_ptr<Engine> theEngine;

float angleBetween(_IN_VEC2 a, _IN_VEC2 b)
{
	float dot = a.x*b.x + a.y*b.y;     // dot product between[x1, y1] and [x2, y2]
	float det = a.x*b.y - a.y*b.x;     // determinant
	float rad = std::atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
	return rad / 3.14159f * 180;
}

float len(_IN_VEC2 a)
{
	return sqrt((a.x*a.x) + (a.y*a.y));
}

sf::Vector2f norm(_IN_VEC2 a)
{
	float l = len(a);
	if (l != 0)
		return { a.x / l, a.y / l };
	else
		return{ 0,0 };
}

int randInt(int from, int lessThan)
{
	int diff = lessThan - from;
	return rand() % diff + from;
}

Engine::~Engine()
{
}

void Engine::init(const sf::Vector2i& size,const sf::Vector2i& view)
{
	srand(time(0));

	m_window.create(sf::VideoMode(size.x, size.y), "Fathom");

	int diff = m_window.getSize().x - size.x;
	resMan = std::make_shared<ResMan>();

	m_screenSize = { view };

#ifdef DEBUG
	ImGui::SFML::Init(m_window);
	ImGui::SetNextWindowSize(sf::Vector2f(m_window.getSize()) * 0.2f);
	ImGui::SetNextWindowPos({ 0,0 });
#endif

	m_renderTarget = &m_window;

	sf::View wview;

	wview.setSize(sf::Vector2f(m_screenSize));
	wview.setCenter((float)m_screenSize.x / 2, (float)m_screenSize.y / 2);
	m_window.setView(wview);

	m_leftBlackout.setFillColor({ 0,0,0,255 });
	m_leftBlackout.setSize({ 0.f, (float)m_screenSize.y });
	m_rightBlackout = m_leftBlackout;
	m_rightBlackout.setPosition({ (float)m_screenSize.x, 0.f });
	

	m_tickClock.restart();

	m_fonts.emplace_back();
	m_fonts[0].loadFromFile("../data/OCRAEXT.TTF");
	m_fonts.emplace_back();
	m_fonts[1].loadFromFile("../data/KALAM-REGULAR.TTF");
	m_loadingText.text.setFont(m_fonts[0]);
	m_loadingText.text.setString("Loading...");
	m_loadingText.text.setCharacterSize(12);
	m_loadingText.text.setPosition(0, 300);
	m_loadingText.visible = true;
	m_loadingText.active = true;

	States.initStates();
	States.nextState = "GS_ENTER";
	States.signalExitTo("GS_ENTER");

	GameObjects::setupVisuals();

}

void Engine::handleEvents()
{
	for (auto& b : m_buttons)
		b->reset();

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			close();
		if (event.type == sf::Event::Resized)
		{
			sf::Vector2f size = { (float)event.size.width, (float)event.size.height };
			auto yMult = m_screenSize.y / size.y;
			float width = size.x * yMult;
			sf::Vector2f newSize(width, m_screenSize.y);

			float xDiff = width - m_screenSize.x;
			m_leftOffset = xDiff / 2;

			m_leftBlackout.setSize({ m_leftOffset, (float)m_screenSize.y });
			m_leftBlackout.setPosition({ 0-m_leftOffset, 0.f });
			m_rightBlackout.setSize({ m_leftOffset, (float)m_screenSize.y });
			m_rightBlackout.setPosition({(float)m_screenSize.x, 0.f });

			sf::View wview;

			wview.setSize(sf::Vector2f(newSize));
			wview.setCenter((float)m_screenSize.x / 2, (float)newSize.y / 2);
			m_window.setView(wview);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			close();

		if (event.type == sf::Event::MouseMoved && m_mouseEnabled)
		{
			m_cursor->normal();
			if (!m_player->isMouseLed())
			{
				for (auto& b : m_buttons)
					if(b->check(getMousePos()))
						break;

				for (auto& s : m_renderList)
				{
					if (s->checkMouseOver(getMousePos()))
						break;
				}
			}
			if(!dialog->isDialogFinished())
				m_cursor->normal();
		}

		if (event.type == sf::Event::MouseButtonPressed && m_mouseEnabled)
		{
			bool canPlayerMove = true;
			if (!m_player->isMouseLed())
			{
				for (auto& b : m_buttons)
					if (b->check(getMousePos()))
					{
						canPlayerMove = false;
						break;
					}
				if (canPlayerMove)
				{
					auto list = m_collisionList;
					list.reverse();
					for (auto& o : m_collisionList)
						if (o->checkInteraction(getMousePos()))
						{
							canPlayerMove = false;
							break;
						}
				}
					
			}
			if (m_playerMoveInhibited)
				canPlayerMove = false;

			m_player->followMouse(canPlayerMove);
		}


#ifdef DEBUG
		if (event.key.code == sf::Keyboard::E && event.key.control && dbv_switchClock.getElapsedTime().asMilliseconds() > 200)
		{
			dbv_switchClock.restart();
			m_debugViewActive = !m_debugViewActive;
		}


		if(m_debugViewActive)
			ImGui::SFML::ProcessEvent(event);
#endif
	}
	auto dt = m_tickClock.restart();

#ifdef DEBUG
	debugViewer(dt);
	if (dbv_stealTick && m_debugViewActive)
		return;
#endif

	if(dialog->active)
		dialog->update(dt);
	
	auto tickList = m_renderList;
	for (auto s : tickList)
	{
		if (s->active)
			s->tick(dt);
	}


}

void Engine::render()
{
	if (!m_window.isOpen())
		return;

	m_renderTarget->clear();

	for (auto s : m_renderList)
	{
		if (s->visible)
			s->draw(*m_renderTarget);
	}

	m_renderTarget->draw(m_leftBlackout);
	m_renderTarget->draw(m_rightBlackout);

#ifdef DEBUG
	if (m_awaitingDebugRender && m_debugViewActive)
	{
		ImGui::SFML::Render(m_window);
		m_awaitingDebugRender = false;
	}
	
#endif

	m_window.display();
}

void Engine::manageStates()
{
	States.runCurrent();
}

void Engine::close()
{
	for (auto& th : m_currentWorld->getCurrentLevelObject().loadingThreads)
		th.join();

	m_currentWorld->getCurrentLevelObject().loadingThreads.clear();

	m_renderList.clear();
	m_collisionList.clear();
	m_buttons.clear();
	m_fonts.clear();
	m_window.clear();
	resMan->clear();
	m_window.close();
}

int Engine::getScrW()
{
	return m_screenSize.x;
}

int Engine::getScrH()
{
	return m_screenSize.y;
}

void Engine::addtoRenderList(GfxBase* _sprite)
{
	m_renderList.push_back(_sprite);
}

sf::Vector2f Engine::stringToVec2(std::string& coord)
{
	sf::Vector2f xy;
	size_t pos = 0;
	if (pos = coord.find(", ") == std::string::npos) __debugbreak();
	auto xstr = coord.substr(0, pos);

	coord.erase(0, pos + 2);
	auto ystr = coord;

	xy = { (float)atoi(xstr.c_str()), (float)atoi(ystr.c_str()) };
	return xy;
}

void Engine::removeFromRenderList(GfxBase * item)
{
	int it = 0;
	for (auto& i : m_renderList)
	{
		if (i == item)
		{
			m_renderList.erase(m_renderList.begin() + it);
			break;
		}
		it++;
	}
}

void Engine::showLoadingText(_IN_STR string, bool clear)
{
	auto vSize = m_window.getView().getSize();

	m_loadingText.text.setString(string);
	m_loadingText.text.setCharacterSize(vSize.y/52);
	m_loadingText.text.setPosition(0, 0);
	auto middle = m_loadingText.text.findCharacterPos(string.size() / 2);
	m_loadingText.text.setPosition(vSize.x / 2 - middle.x, vSize.y / 2 - middle.y + vSize.y / 12);

	if (clear)
	{
		m_renderTarget->clear();
		m_renderTarget->draw(m_loadingScreen);
	}
	
	m_renderTarget->draw(m_loadingText);

	m_window.display();
}

void Engine::hideLoadingText()
{
	m_loadingText.visible = false;
}

sf::Vector2f Engine::getViewScale()
{
	auto size = sf::Vector2f(m_window.getSize());
	auto vSize = m_window.getView().getSize();

	sf::Vector2f scale = { vSize.x / size.x, vSize.y / size.y };
	return scale;
}

sf::Vector2f Engine::getMousePos()
{
	sf::Vector2f mPos(sf::Mouse::getPosition(*theEngine->getWindow()));
	auto scale = getViewScale();
	mPos.x *= scale.x;
	mPos.y *= scale.y;
	mPos.x -= m_leftOffset;
	return mPos;
}

sf::Vector2f Engine::worldToScreenCoords(_IN_VEC2 _pos)
{
	auto pos = _pos;
	auto size = sf::Vector2f(m_window.getSize());
	auto vSize = m_window.getView().getSize();

	sf::Vector2f scale = { size.x / vSize.x, size.y / vSize.y };

	pos.x += m_leftOffset;

	return { pos.x*scale.x, pos.y*scale.y };
}

void Engine::sortRenderList()
{
	std::sort(m_renderList.begin(), m_renderList.end(), ZLessThan);
}

bool Engine::testCollision(sf::Vector2f & pos, sf::Vector2f & vel, GfxBase* actor)
{
	for (auto& obj : m_collisionList)
	{
		if (obj->testCollision(pos, vel, actor))
			return true;
	}
	return false;
}

void Engine::checkPlayerBehind(_IN_VEC2 pos, int & Z)
{
	Z = 0;
	for (auto& obj : m_collisionList)
	{
		if (obj->isPlayerBehind(pos) && Z < obj->Zlevel)
			Z = obj->Zlevel + 1;
	}
}

sf::Vector2f Engine::checkWorldGateways(_IN_VEC2 charPos)
{
	return m_currentWorld->checkGateways(charPos);
}

void Engine::debugViewer(sf::Time& dt)
{
	if (m_awaitingDebugRender || m_debugViewActive == false)
		return;

	ImGui::SFML::Update(m_window, dt);
	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.1f, 0.1f, 0.6f });
	ImGui::Begin("DEBUG", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("mousePos = %d, %d", (int)getMousePos().x, (int)getMousePos().y);

	if (ImGui::CollapsingHeader("RenderList"))
	{
		ImGui::Checkbox("Show Visible Only", &dbv_showVisibleOnly);
		int rlIdx = 0;
		for (auto& gfx : m_renderList)
		{
			if (dbv_showVisibleOnly && gfx->visible == false)
				continue;

			ImGui::PushID((gfx->m_UID+"box").c_str());
			ImGui::Checkbox("", &gfx->visible);
			ImGui::PopID();
			ImGui::SameLine();
			if (!gfx->visible)
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.2f, 0.2f, 1.0f });
			else
			{
				bool effectors = false;
				if (auto sp = dynamic_cast<tSprite*>(gfx))
				{
					if (sp->getNumEffectors() != 0u)
					{
						effectors = true;
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.2f, 1.0f, 1.0f });
					}
				}
				if (!effectors)
				{
					if(gfx->active)
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 1.0f, 0.5f, 1.0f });
					else
						ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.5f, 0.5f, 1.0f });
				}
				
			}
				

			ImGui::PushID((gfx->m_UID + "tree").c_str());
			if (ImGui::TreeNode(gfx->m_UID.c_str()))
			{

				if (auto sp = dynamic_cast<tSprite*>(gfx))
				{
					ImGui::Text("pos");
					float x = sp->sprite.getPosition().x;
					float y = sp->sprite.getPosition().y;
					ImGui::Text("(");
					ImGui::SameLine();
					ImGui::PushItemWidth(50);
					if (ImGui::InputFloat(",", &x, 0, 0, 1, ImGuiInputTextFlags_AutoSelectAll) && !sp->getNumEffectors())
					{
						sp->sprite.setPosition(x, y);
					}
						
					ImGui::SameLine();
					if (ImGui::InputFloat(")", &y, 0, 0, 1, ImGuiInputTextFlags_AutoSelectAll) && !sp->getNumEffectors())
					{
						sp->sprite.setPosition(x, y);
					}
					ImGui::PopItemWidth();

					ImGui::Text("rot = %d", (int)sp->sprite.getRotation());

					
				}

				ImGui::TreePop();
			}
			ImGui::PopStyleColor();
			ImGui::PopID();
			rlIdx++;
		}
	}

	auto dbvPos = ImGui::GetWindowPos();
	auto dbvSize = ImGui::GetWindowSize();

	ImGui::End();
	ImGui::PopStyleColor();
	m_awaitingDebugRender = true;

	
	sf::IntRect bounds(dbvPos, dbvSize);
	if (bounds.contains(sf::Mouse::getPosition(m_window)))
		dbv_stealTick = true;
	else
		dbv_stealTick = false;

}




