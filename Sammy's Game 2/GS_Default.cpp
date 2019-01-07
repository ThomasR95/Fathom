#include "GS_Default.h"
#include "resourceManager.h"
#include "GameObjects.h"
#include "iostream"

GS_Default::GS_Default()
{
}


GS_Default::~GS_Default()
{
}

void GS_Default::enter()
{
	theEngine->setNextState("GS_EXIT");
}

void GS_Default::run()
{

}

void GS_Default::exit()
{
}
