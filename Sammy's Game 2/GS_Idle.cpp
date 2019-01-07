#include "GS_Idle.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Idle::GS_Idle()
{
}


GS_Idle::~GS_Idle()
{
}

void GS_Idle::enter()
{
	theEngine->setNextState("GS_EXIT");
}

void GS_Idle::run()
{

}

void GS_Idle::exit()
{
}
