#include "GS_Level_3.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Level_3::GS_Level_3()
{
}


GS_Level_3::~GS_Level_3()
{
}

void GS_Level_3::enter()
{
	theEngine->setNextState("GS_EXIT");

	if (!theEngine->getPlayer()->checkInventory("Hound"))
	{
		GameObjects::hound.visible = true;
		GameObjects::hound.active = true;
	}
	

	if (theEngine->getPlayer()->checkInventory("Cheese"))
	{
		GameObjects::hound.setInteraction("L3_FeedHound", true, true);
		GameObjects::hound.setCursorType("EYE");
	}
	else
	{
		GameObjects::hound.setInteraction("L3_Hound", true, true);
		GameObjects::hound.setCursorType("EYE");
	}
		

	if (theEngine->dialog->getEntry("L3_Bird")->readOnce)
	{
		auto pBird = (tSprite*)theEngine->getCurrentWorld()->getAsset("starling");
		pBird->visible = false;
		pBird->active = false;
	}
}

void GS_Level_3::run()
{
	if (!dogFed && theEngine->dialog->getEntry("L3_FeedHound")->readOnce && theEngine->dialog->isDialogFinished())
	{
		dogFed = true;
		GameObjects::hound.setInteraction("", false);
		GameObjects::hound.moveTo({ 678, 541 });
		GameObjects::hound.moveTo({ -300,-100 });
		GameObjects::hound.setCursorType("");
	}
}

void GS_Level_3::exit()
{
	GameObjects::hound.visible = false;
	GameObjects::hound.active = false;
}
