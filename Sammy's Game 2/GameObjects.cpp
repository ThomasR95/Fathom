#include "typedefs.h"

#include "GameObjects.h"
#include "engine.h"
#include "resourceManager.h"
#include "Terrain.h"
#include "Dialog.h"

std::vector<Obstacle> GameObjects::obstacles;
Player GameObjects::thePlayer;
World GameObjects::theWorld;
NPC GameObjects::hound;
NPC GameObjects::gardener;
tCursor GameObjects::theCursor;
tSprite* GameObjects::m_blackout = nullptr;

GameObjects::GameObjects()
{
}


GameObjects::~GameObjects()
{
}

void GameObjects::setupVisuals()
{
	auto tex = resMan->getTexture("blank.png");
	auto tex2 = resMan->getTexture("GUI/loading screen.jpg");
	tSprite blank(*tex, { 0,0 });
	tSprite loader(*tex2, { 0,0 });
	theEngine->getWindow()->clear();
	theEngine->getWindow()->draw(blank);
	theEngine->getWindow()->draw(loader);
	theEngine->setLoadingScreen(loader.sprite);
	theEngine->getWindow()->display();

	m_blackout = resMan->addSprite("blackout", "GUI/Blackout.png");
	m_blackout->sprite.setScale({ 100,100 });
	m_blackout->sprite.setColor({ 255,255,255,0 });
	m_blackout->Zlevel = -1000;
	theEngine->addtoRenderList(m_blackout);

	theEngine->dialog = std::make_shared<Dialog>();
	theEngine->dialog->setFont(*theEngine->getFont(1));
	theEngine->dialog->initGraphics("GUI/Dialog box.png", "GUI/Positive dialog button.png", "GUI/Negative dialog button.png", { 414, 2000 }, { 3600, 2600 });

	theCursor.init("GUI/investigate icon.png", "GUI/Unlock icon.png");
	theEngine->setCursor(&theCursor);

	theEngine->showLoadingText("LOADING: CHARACTER SPRITES");

	thePlayer = Player(*resMan->getTexture("Char/Idle N.png"), { 2000,2000 }, { 0,0 }, { 1014,2216 }, tSprite::origin::BOTTOM_CENTRE);
	thePlayer.sprite.setScale({ 0.6f,0.6f });

	thePlayer.Zlevel = 100;
	thePlayer.setPlayerWidth(250);
	thePlayer.setPlayerFeetOffset(-50);
	thePlayer.m_UID = "Player_Char";
	theEngine->addtoRenderList(&thePlayer);
	theEngine->setPlayer(&thePlayer);

	AnimCfg maincfg;
	maincfg.cols = 6;
	maincfg.rows = 2;
	maincfg.frames = 12;
	maincfg.fps = 6;
	maincfg.org = tSprite::BOTTOM_CENTRE;
	std::vector<AnimCfg> cfgs;
	cfgs.resize(5, maincfg);
	cfgs[0].tex = resMan->getTexture("Char/Idle N.png");
	cfgs[1].tex = resMan->getTexture("Char/Idle NE.png");
	cfgs[2].tex = resMan->getTexture("Char/Idle E.png");
	cfgs[3].tex = resMan->getTexture("Char/Idle SE.png");
	cfgs[4].tex = resMan->getTexture("Char/Idle S.png");
	thePlayer.setIdleAnims(cfgs);
	
	maincfg.fps = 9;
	maincfg.cols = 9;
	maincfg.rows = 1;
	maincfg.frames = 9;
	cfgs.clear();
	cfgs.resize(5, maincfg);
	cfgs[0].tex = resMan->getTexture("Char/Walk N.png");
	cfgs[1].tex = resMan->getTexture("Char/Walk NE.png");
	cfgs[1].frames = 10;
	cfgs[1].cols = 10;
	cfgs[1].fps = 10;
	cfgs[2].tex = resMan->getTexture("Char/Walk E.png");
	cfgs[3].tex = resMan->getTexture("Char/Walk SE.png");
	cfgs[3].frames = 10;
	cfgs[3].cols = 10;
	cfgs[3].fps = 10;
	cfgs[4].tex = resMan->getTexture("Char/Walk S.png");
	thePlayer.setWalkAnims(cfgs);


	hound = NPC(*resMan->getTexture("NPCs/Hound idle.png"), resMan->getTexture("NPCs/Hound hitbox.jpg"), { 1220 + 586/2,1451 + 604 }, { 0,0 }, { 586,764 }, tSprite::origin::BOTTOM_CENTRE);
	hound.Zlevel = 100;
	hound.setCollisionEnabled(true);
	hound.setNPCWidth(450);
	hound.setNPCFeetOffset(-200);
	hound.m_UID = "NPCHound";
	theEngine->addtoRenderList(&hound);

	theEngine->addCollisionObject(&hound);

	AnimCfg dogAnmCfg;
	dogAnmCfg.cols = 6;
	dogAnmCfg.rows = 3;
	dogAnmCfg.frames = 16;
	dogAnmCfg.fps = 6;
	dogAnmCfg.org = tSprite::BOTTOM_CENTRE;
	std::vector<AnimCfg> dogcfgs;
	dogcfgs.resize(5, dogAnmCfg);
	dogcfgs[0].tex = resMan->getTexture("NPCs/Hound idle.png");
	dogcfgs[1].tex = resMan->getTexture("NPCs/Hound idle.png");
	dogcfgs[2].tex = resMan->getTexture("NPCs/Hound idle.png");
	dogcfgs[3].tex = resMan->getTexture("NPCs/Hound idle.png");
	dogcfgs[4].tex = resMan->getTexture("NPCs/Hound idle.png");
	hound.setIdleAnims(dogcfgs);

	dogcfgs.clear();
	dogcfgs.resize(5);
	dogcfgs[0] = AnimCfg(resMan->getTexture("NPCs/Hound walk n.png"), 6, 10, 2, 5, { 0,0 }, tSprite::BOTTOM_CENTRE);
	dogcfgs[1] = AnimCfg(resMan->getTexture("NPCs/Hound walk ne.png"), 6, 10, 2, 3, { 0,0 }, tSprite::BOTTOM_CENTRE);
	dogcfgs[2] = AnimCfg(resMan->getTexture("NPCs/Hound walk e.png"), 6, 10, 3, 2, { 0,0 }, tSprite::BOTTOM_CENTRE);
	dogcfgs[3] = AnimCfg(resMan->getTexture("NPCs/Hound walk se.png"), 6, 10, 2, 4, { 0,0 }, tSprite::BOTTOM_CENTRE);
	dogcfgs[4] = AnimCfg(resMan->getTexture("NPCs/Hound walk s.png"), 6, 10, 2, 5, { 0,0 }, tSprite::BOTTOM_CENTRE);
	hound.setWalkAnims(dogcfgs);

	gardener = NPC(*resMan->getTexture("NPCs/Hound idle.png"), resMan->getTexture("NPCs/Hound hitbox.jpg"), { 1220 + 586 / 2,1451 + 604 }, { 0,0 }, { 586,764 }, tSprite::origin::BOTTOM_CENTRE);
	gardener.Zlevel = 100;
	gardener.setCollisionEnabled(true);
	gardener.setNPCWidth(450);
	gardener.setNPCFeetOffset(-50);
	gardener.m_UID = "NPCGardener";
	gardener.setMaxSpeed(800);
	theEngine->addtoRenderList(&gardener);

	AnimCfg gAnmCfg;
	gAnmCfg.cols = 7;
	gAnmCfg.rows = 2;
	gAnmCfg.frames = 9;
	gAnmCfg.fps = 9;
	gAnmCfg.org = tSprite::BOTTOM_CENTRE;
	std::vector<AnimCfg> gCfgs;
	gCfgs.resize(5, gAnmCfg);
	gCfgs[0].tex = resMan->getTexture("NPCs/Gardener monster walk n.png");
	gCfgs[0].fps = 8; gCfgs[0].frames = 8;
	gCfgs[1].tex = resMan->getTexture("NPCs/Gardener monster walk ne.png");
	gCfgs[2].tex = resMan->getTexture("NPCs/Gardener monster walk e.png");
	gCfgs[3].tex = resMan->getTexture("NPCs/Gardener monster walk se.png");
	gCfgs[4].tex = resMan->getTexture("NPCs/Gardener monster walk s.png");
	gCfgs[4].fps = 8; gCfgs[4].frames = 8;
	gardener.setIdleAnims(gCfgs);
	gardener.setWalkAnims(gCfgs);

	theEngine->addCollisionObject(&gardener);

	theWorld.readXml("../images/Levels/Levels.xml");
	theEngine->setCurrentWorld(&theWorld);


	theEngine->sortRenderList();
}
